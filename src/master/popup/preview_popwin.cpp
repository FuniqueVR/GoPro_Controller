#include "preview_popwin.h"
#include <format>
#include <cstdlib>
#include "../../common/camera_setting.h"

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
        stream_open = false;
        isopen = false;
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
        master->preview_end(state->preview_server, state->preview_ip);
    }
}

void PreviewPopup::update_decoder(){
    stream_open = false;
    trying = true;
    int32_t s = -1;
    int32_t model;

    if(first){
        std::cout << "===== OpenCV Build Info =====" << std::endl;
        std::cout << cv::getBuildInformation() << std::endl;
        std::cout << "=============================" << std::endl;
        first = false;
    }

    {
        {
            std::lock_guard<std::mutex> lock(master->camera_mtx);
            s = master->findCamera(state->preview_ip);
        }

        if(s == -1){
            std::cout << "[Preview Decoder] Cannot find camera: " << state->preview_ip << std::endl;
            trying = false;
            return;
        }
        const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
        model = _get_current_model(c->hw);
        json buffer_setting = json::object();
        master->getSettingsFromCamera(*c, buffer_setting);
    }

    if(gl_texture != 0){
        glDeleteTextures(1, &gl_texture);
        gl_texture = 0;
    }

    bool g = false;
    const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
    if(!g){
        pipeline = 
            "udpsrc port=8554 timeout=1000000000 "
            "! watchdog timeout=1000 "
            "! queue max-size-buffers=0 max-size-bytes=0 max-size-time=1000000000 "
            // Treet incoming UDP byte array as TS format
            "! tsdemux "
            /**
             * We nned to use general decodebin, because GOPRO model might have use
             * H265 or H264, We have no way to find out except fetch info from the incoming camera
             * But i'm too lazy for that. so i just want GStreamer do the detection for me
             */
            "! decodebin "
            // Convert to video feed
            "! videoconvert "
            // Leave video only and drop audio data
            "! video/x-raw,format=BGR "
            // Output to application use sink
            "! appsink sync=false drop=true max-buffers=1";
        //replaceAll(pipeline, "{0}", c->server.c_str());
        cap.open(pipeline, cv::CAP_GSTREAMER);
        std::cout << "[Preview Decoder] Pipeline use:" << std::endl << pipeline << std::endl;
    }
    //g = true;

    if(cap.isOpened()){
        cv::Mat test;
        for(int i = 0; i < MAX_ATTEMPT; i++) {
            std::cout << "[Preview Decoder] Try cap.grab" << std::endl;
            if(cap.grab()) {
                std::cout << "[Preview Decoder] cap.retrieve" << std::endl;
                if(cap.retrieve(test) && !test.empty()){
                    stream_open = true;
                    std::cout << "[Preview Decoder] Pipeline opened successfully! " << test.cols << "x" << test.rows << std::endl;
                    texture_width = test.cols;
                    texture_height = test.rows;
                    std::cout << "[Preview Decoder] Get resolution setting from camera: " << texture_width << ", " << texture_height << std::endl;
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(33)); // Match ~30fps
        }
        if(!stream_open){
            std::cout << "[Preview Decoder] No valid frames in first " << MAX_ATTEMPT << " attempts, releasing..." << std::endl;
        }
    } else {
        std::cout << "[Preview Decoder] Failed to open pipeline, retrying in 1s..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    if(!stream_open){
        std::cerr << "[Preview Decoder] Could not open pipeline" << std::endl;
        trying = false;
        return;
    }
    
    cv::Mat frame;
    std::cout << "[Preview Decoder] Decoding started!" << std::endl;

    while(stream_open){
        if(!cap.isOpened()){
            std::cerr << "[Preview Decoder] Pipeline closed unexpectedly!" << std::endl;
            trying = false;
            break;
        }

        if(cap.read(frame)){
            if(!frame.empty()){
                std::lock_guard<std::mutex> lock(queue_mutex);
                if(frame_queue.size() >= MAX_QUEUE_SIZE){
                    frame_queue.pop();
                }
                frame_queue.push(frame.clone());
            }
        } else {
            // cap.read failed - stream might have dropped
            std::cerr << "[Preview Decoder] cap.read() failed, stream dropped?" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if(!isopen) break;
    }

    std::cout << "[Preview Decoder] Update decoder end !" << std::endl;
    trying = false;
}

void PreviewPopup::update(){
    
}

void PreviewPopup::render(){
    cv::Mat frame = get_latest_frame();
    ConvertTexture(frame);
    bool remap = false;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 display_size = io.DisplaySize;
    ImVec2 unit = ImVec2(display_size.x / 10.0f, display_size.y / 10.0f);

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
                if(ratio >= 1.0f){ // Normal aspect, w > h
                    size = ImVec2((unit.y * 7.0f) * ratio, unit.y * 7.0f);
                }else{ // h > w
                    size = ImVec2(unit.x * 7.0f, (unit.x * 7.0f) / ratio);
                }
                ImGui::Image((ImTextureID)(intptr_t)gl_texture, size);
            }
            ImGui::SetCursorScreenPos(image_pos);
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
        ImGui::BeginDisabled(trying);
        if(ImGui::Button("Retry")){
            stream_open = false;
            isopen = false;
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
            reader = std::thread([&]() {
                update_decoder();
            });
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }
    if(remap){
      // Let's recreate the map here  
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
