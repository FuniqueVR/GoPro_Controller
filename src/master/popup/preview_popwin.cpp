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
    }else{
        if (cap.isOpened()) {
            cap.release();
        }
    }
}

void PreviewPopup::render(){
    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        ImGui::Image()
        ImGui::EndPopup();
    }
}

cv::Mat PreviewPopup::get_frame(){
    cv::Mat frame;
    if (cap.read(frame)) {
        return frame;
    }
    return cv::Mat();
}

void PreviewPopup::ConvertTexture(cv::Mat& mat){
    cv::resize(mat, mat, cv::size(texture_width, texture_height));
    video_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_BGR24,
        SDL_TEXTUREACCESS_STREAMING,
        texture_width,
        texture_height
    );

    if (SDL_LockTexture(video_texture, nullptr, &pixels, &pitch) == 0) {
        // Copy frame data to texture
        memcpy(pixels, frame.data, frame.total() * frame.elemSize());
        SDL_UnlockTexture(video_texture);
    }
}