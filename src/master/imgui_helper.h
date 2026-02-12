/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <memory>
#include <utility>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <tuple>
#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "../windows/base_window.h"

extern "C" inline void setup_imgui();
extern "C" inline void setup_catppuccin_mocha_theme();
extern "C" inline void begin_imgui(SDL_Window *window, SDL_GLContext sdl_gl_context, const char *glsl_version = (const char *)nullptr);
extern "C" inline void end_imgui();
extern "C" inline std::tuple<SDL_Window*, SDL_GLContext, const char*> begin_sdl();
extern "C" inline void end_sdl(SDL_Window *window, SDL_GLContext sdl_gl_context);
