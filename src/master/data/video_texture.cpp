#include "video_texture.h"

VideoTexture::~VideoTexture() {
  SDL_DestroyTexture(texture);
  SDL_DestroySurface(surface);
}

void VideoTexture::setImage(cv::Mat *pframe, SDL_Renderer* render) {
  width = pframe->cols;
  height = pframe->rows;

  surface = SDL_CreateSurfaceFrom(
    pframe->cols, 
    pframe->rows, 
    SDL_PIXELFORMAT_RGBA8888, 
    pframe->data, 
    pframe->step);

  texture = SDL_CreateTextureFromSurface(render, surface);
}