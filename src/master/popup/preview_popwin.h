#pragma once
#include <queue>
#include <mutex>
#include <thread>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "Wtsapi32.lib") 
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Ole32.lib")
#endif
#include <SDL3/SDL.h>
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include "base_pop_window.h"

class PreviewPopup : public BasePopWindow {
public:
    PreviewPopup(
        SDL_Renderer* _renderer,
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~PreviewPopup();

    virtual void trigger(bool value) override;
    virtual void update_decoder();
    virtual void update() override;
    virtual void render() override;
private:
    cv::VideoCapture cap;
    std::string pipeline;
    std::queue<cv::Mat> frame_queue;
    std::mutex queue_mutex;
    std::thread reader;
    const size_t MAX_QUEUE_SIZE = 10;

    bool stream_open = false;
    GLuint gl_texture = 0;
    SDL_Renderer* renderer = NULL;
    int32_t texture_width = 1920;
    int32_t texture_height = 1080;

    cv::Mat get_latest_frame();
    void ConvertTexture(cv::Mat& mat);
};
