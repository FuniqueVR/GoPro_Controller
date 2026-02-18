#include "preview_popwin.h"
#include "../../common/camera_setting.h"

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

void PreviewPopup::trigger(bool value){
    BasePopWindow::trigger(value);
    if(value){
        reader = std::thread([=]() {
            update_decoder();
        });
    }else{
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
        master->preview_end(state->preview_server, state->preview_ip);
    }
}

void PreviewPopup::update_decoder(){

    stream_open = false;
    int32_t retry = 0;
    const int32_t MAX_RETRY = 30;

    std::cout << "[Preview Decoder] Update decoder start !" << " " << stream_open << " " << (retry < MAX_RETRY) << std::endl;

    {
        std::lock_guard<std::mutex> lock(master->camera_mtx);
        int32_t s = master->findCamera(state->preview_ip);
        if(s == -1){
            std::cout << "[Preview Decoder] Cannot find camera: " << state->preview_ip << std::endl;
            return;
        }
        const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
        json buffer_setting = json::object();
        master->getSettingsFromCamera(c, buffer_setting);
        if(buffer_setting["2"].is_number_integer()){
            int32_t res = buffer_setting["2"].get<int32_t>();
            int32_t detail[2] = VIDEO_RESOLUTION_RES[res];
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

    while(!stream_open && retry < MAX_RETRY){
        retry++;
        std::cout << "[Preview Decoder] Attempt " << retry << "/" << MAX_RETRY << " opening pipeline..." << std::endl;

        pipeline =
            "udpsrc port=8556 buffer-size=2097152 "
            "! queue max-size-buffers=0 max-size-bytes=0 max-size-time=2000000000 "
            "! tsparse "
            "! decodebin "
            "! videoconvert "
            "! video/x-raw,format=BGR "
            "! appsink sync=false drop=true max-buffers=2";

        cap.open(pipeline, cv::CAP_GSTREAMER);

        if(cap.isOpened()){
            // ✅ Try to grab one frame to confirm it actually works
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
    }

    std::cout << "[Preview Decoder] Update decoder end !" << std::endl;
}

void PreviewPopup::update(){
    
}

void PreviewPopup::render(){
    cv::Mat frame = get_latest_frame();
    ConvertTexture(frame);

    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        
        if(gl_texture != 0){
            ImGui::Image((ImTextureID)(intptr_t)gl_texture, ImVec2(800, 600));
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
}

cv::Mat PreviewPopup::get_latest_frame(){
    std::lock_guard<std::mutex> lock(queue_mutex);

    if (frame_queue.empty()) {
        return cv::Mat();
    }
    
    cv::Mat frame = frame_queue.front();
    frame_queue.pop();
    std::cout << "[Preview Decoder] Last frame received !" << std::endl;
    return frame;
}

void PreviewPopup::ConvertTexture(cv::Mat& mat){
    if(mat.empty()) return;

    cv::Mat frame;
    if (mat.cols != texture_width || mat.rows != texture_height){
        cv::resize(mat, frame, cv::Size(texture_width, texture_height));
    } else {
        frame = mat;
    }

    cv::Mat rgb;
    cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);

    if(gl_texture == 0){
        glGenTextures(1, &gl_texture);
        glBindTexture(GL_TEXTURE_2D, gl_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Allocate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
            texture_width, texture_height,
            0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cout << "[Preview Decoder] Frame converted !" << std::endl;
    } else {
        // ✅ Update texture data (fast path)
        glBindTexture(GL_TEXTURE_2D, gl_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                        texture_width, texture_height,
                        GL_RGB, GL_UNSIGNED_BYTE, rgb.data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}