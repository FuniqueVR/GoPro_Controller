#pragma once
#include <cinttypes>
#include <string>

#include "imgui.h"
#include <opencv2/opencv.hpp>
#include <SDL3/SDL.h>

class VideoTexture {
 private:
  int32_t width = 0, height = 0;
  SDL_Texture* texture = nullptr;
  SDL_Surface* surface = nullptr;

 public:
  ~VideoTexture();
  void setImage(cv::Mat *frame, SDL_Renderer* render);
};