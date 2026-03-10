#include "preview_popwin.h"
#include <format>
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
        if(reader.joinable()){
            reader.join();
        }
        if (cap.isOpened()) {
            cap.release();
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
    int32_t retry = 0;
    int32_t s = -1;
    const int32_t MAX_RETRY = 10;

    std::cout << "===== OpenCV Build Info =====" << std::endl;
    std::cout << cv::getBuildInformation() << std::endl;
    std::cout << "=============================" << std::endl;

    std::cout << "[Preview Decoder] Update decoder start !" << " " << stream_open << " " << (retry < MAX_RETRY) << std::endl;
    {
        std::lock_guard<std::mutex> lock(master->camera_mtx);
        s = master->findCamera(state->preview_ip);
        if(s == -1){
            std::cout << "[Preview Decoder] Cannot find camera: " << state->preview_ip << std::endl;
            return;
        }
        const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
        json buffer_setting = json::object();
        master->getSettingsFromCamera(*c, buffer_setting);
        if(buffer_setting["2"].is_number_integer()){
            int32_t res = buffer_setting["2"].get<int32_t>();
            const int32_t* detail = VIDEO_RESOLUTION_RES[res];
            texture_width = detail[0];
            texture_height = detail[1];
        }else{
            std::cout << "[Preview Decoder] Cannot get resolution setting from camera: " << state->preview_ip << std::endl;
            std::cout << "[Preview Decoder] Fallback to default HD resolution " << std::endl;
            texture_width = 1920;
            texture_height = 1080;
        }
    }

    if(gl_texture != 0){
        glDeleteTextures(1, &gl_texture);
        gl_texture = 0;
    }

    bool g = false;
    while(!stream_open && retry < MAX_RETRY){
        const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
        retry++;
        std::cout << "[Preview Decoder] Attempt " << retry << "/" << MAX_RETRY << " opening pipeline..." << std::endl;

        if(!g){
            pipeline = 
                "udpsrc address={0} port=8554 buffer-size=41943040 "
                "! queue max-size-buffers=0 max-size-bytes=0 max-size-time=2000000000 "
                "! tsdemux " 
                "! h264parse "
                "! decodebin "
                "! videoconvert "
                "! video/x-raw,format=BGR "
                "! appsink sync=false drop=true max-buffers=2";
            replaceAll(pipeline, "{0}", c->server.c_str());
            cap.open(pipeline, cv::CAP_GSTREAMER);
            std::cout << "[Preview Decoder] Pipeline use:" << std::endl << pipeline << std::endl;
        }
        g = true;

        if(cap.isOpened()){
            cv::Mat test;
            cap.grab();
            if(cap.retrieve(test) && !test.empty()){
                stream_open = true;
                std::cout << "[Preview Decoder] Pipeline opened successfully! " << test.cols << "x" << test.rows << std::endl;
            } else {
                std::cout << "[Preview Decoder] Pipeline opened but no frames yet, retrying..." << std::endl;
                cap.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        } else {
            std::cout << "[Preview Decoder] Failed to open pipeline, retrying in 1s..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        if(!isopen) break;
    }

    if(!stream_open){
        std::cerr << "[Preview Decoder] Could not open pipeline after " << MAX_RETRY << " attempts!" << std::endl;
        return;
    }
    
    cv::Mat frame;
    std::cout << "[Preview Decoder] Decoding started!" << std::endl;

    while(stream_open){
        if(!cap.isOpened()){
            std::cerr << "[Preview Decoder] Pipeline closed unexpectedly!" << std::endl;
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

    ImGui::SetNextWindowPos(ImVec2(unit.x * 0.5F, unit.y * 0.5F), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(unit.x * 9.0F, unit.y * 9.0F), ImGuiCond_Always);

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
        if(gl_texture != 0){
            if(ImGui::Button("<== Rotate##preview_button")){
                DirChange(true); remap = true;
            }
            ImGui::SameLine();
            if(ImGui::Button("Rotate ==>##preview_button")){
                DirChange(false); remap = true;
            }
            float ratio = (float)target_w / (float)target_h;
            ImVec2 size;
            if(target_w >= target_h){
                size = ImVec2(unit.x * 7, (unit.x * 7) / ratio);
            }else{
                size = ImVec2((unit.y * 7) * ratio, unit.y * 7);
            }
            ImGui::Image((ImTextureID)(intptr_t)gl_texture, size);
        } else {
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

        if(ImGui::Button("Cancel")){
            trigger(false);
        }
        ImGui::EndPopup();
    }
    if(remap){
      // Let's recreate the map here  
    }
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
