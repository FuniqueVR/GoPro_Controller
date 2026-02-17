#include "preview_popwin.h"

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
    if(video_texture != NULL) SDL_DestroyTexture(video_texture);
}

void PreviewPopup::trigger(bool value){
    BasePopWindow::trigger(value);
    if(value){
        pipeline = "udpsrc port=8556 "
            "caps=\"video/mpegts, systemstream=(boolean)true, packetsize=(int)188\" "
            "! decodebin "
            "! videoconvert "
            "! video/x-raw, format=BGR "
            "! appsink sync=false drop=true max-buffers=1";
        cap.open(pipeline, cv::CAP_GSTREAMER);
        if (!cap.isOpened()) {
            std::cerr << "Failed to open pipeline!" << std::endl;
            return;
        }
        reader = std::thread([=]() {
            update_decoder();
        });
    }else{
        if (cap.isOpened()) {
            cap.release();
        }
        if(reader.joinable()){
            reader.join();
        }
        while(frame_queue.size() > 0) frame_queue.pop();
        master->preview_end(state->preview_server, state->preview_ip);
    }
}

void PreviewPopup::update_decoder(){
    cv::Mat frame;
    std::cout << "[Preview Decoder] Update decoder start !" << std::endl;
    while(isopen){
        if(cap.read(frame)){
            if(!frame.empty()){
                std::lock_guard<std::mutex> lock(queue_mutex);
                std::cout << "[Preview Decoder] Catch frame !" << std::endl;
                    
                if (frame_queue.size() >= MAX_QUEUE_SIZE) {
                    frame_queue.pop();
                    std::cout << "[Preview Decoder] frame_queue reach limit: " << MAX_QUEUE_SIZE << std::endl;
                }
                
                frame_queue.push(frame.clone());
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
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
        ImGui::Image(video_texture, ImVec2(800, 600));
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
    if(mat.empty()) {
        return;
    }
    if (mat.cols != texture_width || mat.rows != texture_height){
        cv::resize(mat, mat, cv::Size(texture_width, texture_height));
    }

    if(video_texture == NULL){
        video_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_BGR24,
            SDL_TEXTUREACCESS_STREAMING,
            texture_width,
            texture_height
        );
    }
    void* pixels;
    int pitch;
    if (SDL_LockTexture(video_texture, nullptr, &pixels, &pitch) == 0) {
        // Copy frame data to texture
        int row_bytes = mat.cols * mat.elemSize();
        for (int y = 0; y < mat.rows; y++) {
            memcpy(
                (uint8_t*)pixels + y * pitch,
                mat.data + y * row_bytes,
                row_bytes
            );
        }
        SDL_UnlockTexture(video_texture);
    }
    std::cout << "[Preview Decoder] Frame converted !" << std::endl;
}