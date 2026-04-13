#include "../preview_popwin.h"
#include <format>
#include <cstdlib>

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Move past the replaced section to avoid infinite loops
    }
}

PreviewPopup::PreviewPopup(
    SDL_Renderer* _renderer,
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    renderer = _renderer;
    title = "Preview##Popup";

// LOG all the GStreamer debug info out of console
#ifdef _WIN32
    _putenv("GST_DEBUG=2");  // Level 2 = warnings and errors
    // Or for more detail:
    // _putenv("GST_DEBUG=3");  // Level 3 = info + warnings + errors
#else
    setenv("GST_DEBUG", "2", 1);
#endif
}

PreviewPopup::~PreviewPopup(){
    trigger(false);
}

json PreviewPopup::get_window_data(){
    json data = json::object();
    data["dir"] = dir;
    return data;
}

void PreviewPopup::set_window_data(json data){
    if(data["dir"].is_number_integer()){
        dir = data["dir"].get<int32_t>();
    }
}

void PreviewPopup::trigger(bool value){
    BasePopWindow::trigger(value);
    if(value){
        reader = std::thread([&]() {
            update_decoder();
        });
    }else{
        _stop_thread();
        isopen = false;
        master->preview_end(state->preview_server, state->preview_ip);
    }
}

void PreviewPopup::update(){
    
}

void PreviewPopup::render(){
    cv::Mat frame = get_latest_frame();
    ConvertTexture(frame);

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 display_size = io.DisplaySize;
    ImVec2 unit = ImVec2(display_size.x / 10.0f, display_size.y / 10.0f);
    float left_width;

    ImGui::SetNextWindowPos(ImVec2(unit.x * 0.5F, unit.y * 0.5F), wp_cond);
    ImGui::SetNextWindowSize(ImVec2(unit.x * 9.0F, unit.y * 9.0F), wp_cond);

    /**
     * The resolution we actually want to display on the screen
     * The image is already rotate during ConvertTexture stage
     * So we just have to figure out the display vector
     */
    int32_t target_w, target_h;
    if(dir == 0 || dir == 2){
        // 0° or 180° - width/height unchanged
        target_w = texture_width;
        target_h = texture_height;
    } else {
        // 90° or 270° - width/height swapped
        target_w = texture_height;
        target_h = texture_width;
    }

    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        if(gl_texture != 0){ // We have frame to display
            ImVec2 image_pos = ImGui::GetCursorScreenPos();
            ImVec2 window_size = ImGui::GetWindowSize();

            // Image drawing
            {
                float ratio = (float)target_w / (float)target_h;
                ImVec2 size;
                // First we adjust the ratio
                if(ratio >= 1.0f){ // Normal aspect, w > h
                    size = ImVec2((unit.y * 7.0f) * ratio, unit.y * 7.0f);
                }else{ // h > w
                    size = ImVec2(unit.x * 7.0f, (unit.x * 7.0f) / ratio);
                }
                // Even if we adjust the ratio, the resolution might still out of screen
                if(size.x > (unit.x * 8.0f)){
                    float diff = (unit.x * 8.0f) / size.x;
                    size *= diff;
                }else if(size.y > (unit.y * 8.0f)){
                    float diff = (unit.y * 8.0f) / size.y;
                    size *= diff;
                }
                if(remap){
                    std::cout << "Window size: " << display_size.x << ", " << display_size.y << std::endl;
                    std::cout << "unit size: " << unit.x << ", " << unit.y << std::endl;
                    std::cout << "Image size: " << target_w << ", " << target_h << std::endl;
                    std::cout << "Rescale size: " << size.x << ", " << size.y << std::endl;
                    remap = false;
                }
                left_width = ( (unit.x * 8.5f) - size.x) / 2.0f;
                ImGui::Dummy(ImVec2(left_width, 0.0f));
                ImGui::SameLine();
                ImGui::Image((ImTextureID)(intptr_t)gl_texture, size);
            }
            ImGui::SetCursorScreenPos(image_pos);
            // Go Different camera
            {
                ImGui::BeginChild("Detail##Preview_Camera_Inspector", ImVec2(left_width, 0));

                // Rotating button
                {
                    if(ImGui::Button("<== Rotate##preview_button")){
                        DirChange(true); remap = true;
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Rotate ==>##preview_button")){
                        DirChange(false); remap = true;
                    }
                }

                int32_t s = -1;
                std::lock_guard<std::mutex> lock(master->camera_mtx);
                s = master->findCamera(state->preview_ip);
                if(s != -1){
                    const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
                    std::string display_name = c->name;
                    display_name += " ";
                    display_name += c->ip;
                    if(ImGui::BeginCombo("Camera##Preview_Camera_Selection", display_name.c_str())){
                        for(auto& cam : master->getCameras()){
                            if(!cam->connected) continue;
                            std::string display_name2 = cam->name;
                            display_name2 += " ";
                            display_name2 += cam->ip;
                            bool selected = display_name == display_name2;
                            if(ImGui::Selectable(display_name2.c_str(), selected)){
                                _stop_thread();
                                master->preview_end(state->preview_server, state->preview_ip);
                                master->preview_start(cam->server, cam->ip);
                                state->preview_ip = cam->ip;
                                state->preview_server = cam->server;
                                reader = std::thread([&]() {
                                    update_decoder();
                                });
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
                ImGui::EndChild();
            }
        } else { // No frame QAQ
            ImGui::Dummy(ImVec2(800, 280));
            float win_width = ImGui::GetContentRegionAvail().x;
            std::string msg = "Waiting for video stream...";
            float text_width = ImGui::CalcTextSize(msg.c_str()).x;
            ImGui::SetCursorPosX((win_width - text_width) * 0.5f);
            ImGui::Text("%s", msg.c_str());
            // Animated dots
            static int dots = 0;
            static auto last = std::chrono::steady_clock::now();
            auto now = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() > 500){
                dots = (dots + 1) % 4;
                last = now;
            }
            std::string dot_str(dots, '.');
            ImGui::SameLine();
            ImGui::Text("%s", dot_str.c_str());
        }

        /**
         * Bottom action for the preview 
         */
        if(ImGui::Button("Cancel")){
            trigger(false);
        }
        ImGui::SameLine();
        if(!trying) {
            if(ImGui::Button("Retry")){
                _stop_thread();
                reader = std::thread([&]() {
                    update_decoder();
                });
            }
        }

        ImGui::EndPopup();
    }
}

int32_t PreviewPopup::_get_current_model(json target){
    if(target["model_name"].is_string()){
        std::string model_name = target["model_name"].get<std::string>();
        if(model_name == "MAX 2") return MODEL_MAX2;
        else if(model_name == "HERO13 Black") return MODEL_13;
        else if(model_name == "HERO12 Black") return MODEL_12;
        else if(model_name == "HERO11 Black") return MODEL_11_BLACK;
        else if(model_name == "HERO11 Black Mini") return MODEL_11;
        else if(model_name == "HERO10 Black") return MODEL_10;
        else if(model_name == "HERO9 Black") return MODEL_9;
    }
    return 0;
}

void PreviewPopup::_stop_thread(){
    stream_open = false;
    if (cap.isOpened()) {
        cap.release();
    }
    if(reader.joinable()){
        reader.join();
    }
    if(gl_texture != 0){
        glDeleteTextures(1, &gl_texture);
        gl_texture = 0;
    }
    while(frame_queue.size() > 0) frame_queue.pop();
}

cv::Mat PreviewPopup::get_latest_frame(){
    std::lock_guard<std::mutex> lock(queue_mutex);

    if (frame_queue.empty()) {
        return cv::Mat();
    }
    
    cv::Mat frame = frame_queue.front();
    frame_queue.pop();
    return frame;
}

void PreviewPopup::ConvertTexture(cv::Mat& mat){
    if(mat.empty()) return;

    cv::Mat rotated_frame;
    if(dir == 0){
        rotated_frame = mat;  // No rotation
    }
    else if(dir == 1){
        cv::rotate(mat, rotated_frame, cv::ROTATE_90_CLOCKWISE);
    }
    else if(dir == 2){
        cv::rotate(mat, rotated_frame, cv::ROTATE_180);
    }
    else if(dir == 3){
        cv::rotate(mat, rotated_frame, cv::ROTATE_90_COUNTERCLOCKWISE);
    }
    else {
        rotated_frame = mat;  // Invalid dir, no rotation
    }

    cv::Mat frame;
    int32_t target_w, target_h;
    if(dir == 0 || dir == 2){
        // 0° or 180° - width/height unchanged
        target_w = texture_width;
        target_h = texture_height;
    } else {
        // 90° or 270° - width/height swapped
        target_w = texture_height;
        target_h = texture_width;
    }

    if (rotated_frame.cols != target_w || rotated_frame.rows != target_h){
        cv::resize(rotated_frame, frame, cv::Size(target_w, target_h));
    } else {
        frame = rotated_frame;
    }

    cv::Mat rgb;
    cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);

    if(gl_texture == 0){
        glGenTextures(1, &gl_texture);
        glBindTexture(GL_TEXTURE_2D, gl_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
        // Allocate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
            target_w, target_h,
            0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cout << "[Preview Decoder] Frame converted !" << std::endl;
    } else {
        // ✅ Update texture data (fast path)
        glBindTexture(GL_TEXTURE_2D, gl_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            target_w, target_h,
            GL_RGB, GL_UNSIGNED_BYTE, rgb.data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void PreviewPopup::DirChange(bool increase){
    if(increase){
        dir++;
        if(dir >= 4) dir = 0;
    }else{
        dir--;
        if(dir <= -1) dir = 3;
    }
    glDeleteTextures(1, &gl_texture);
    gl_texture = 0;
    state->update_server();
}
