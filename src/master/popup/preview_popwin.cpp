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

}

void PreviewPopup::trigger(bool value){
    BasePopWindow::trigger(value);
    if(value){
        pipeline = "udpsrc port=8556 ! tsdemux ! h264parse ! avdec_h264 ! videoconvert ! appsink";
        cap.open(pipeline, cv::CAP_GSTREAMER);
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
    }
}

void PreviewPopup::update_decoder(){
    cv::Mat frame;
    while(enable){
        if(cap.read(frame)){
            if(!frame.empty()){
                std::lock_guard<std::mutex> lock(queue_mutex);
                    
                if (frame_queue.size() >= MAX_QUEUE_SIZE) {
                    frame_queue.pop();
                }
                
                frame_queue.push(frame.clone());
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
}

void PreviewPopup::update(){
    
}

void PreviewPopup::render(){
    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
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
    return frame;
}

void PreviewPopup::ConvertTexture(cv::Mat& mat){
    if(mat.empty()) return;
    if (mat.cols != texture_width || mat.rows != texture_height){
        cv::resize(mat, mat, cv::Size(texture_width, texture_height));
    }
    video_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_BGR24,
        SDL_TEXTUREACCESS_STREAMING,
        texture_width,
        texture_height
    );
    void* pixels;
    int pitch;
    if (SDL_LockTexture(video_texture, nullptr, &pixels, &pitch) == 0) {
        // Copy frame data to texture
        memcpy(pixels, mat.data, mat.total() * mat.elemSize());
        SDL_UnlockTexture(video_texture);
    }
}