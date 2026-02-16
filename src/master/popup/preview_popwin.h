#pragma once
#include "base_pop_window.h"
#include <SDL3/SDL.h>
#include <opencv2/opencv.hpp>

class PreviewPopup : public BasePopWindow {
public:
    PreviewPopup(
        SDL_Renderer* _renderer,
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~PreviewPopup();

    virtual void trigger(bool value) override;
    virtual void render() override;
private:
    cv::VideoCapture cap;
    std::string pipeline;
    SDL_Texture* video_texture = nullptr;
    SDL_Renderer* renderer = nullptr;
    int32_t texture_width = 1280;
    int32_t texture_height = 720;

    cv::Mat get_frame();
    void ConvertTexture(cv::Mat& mat);
};
