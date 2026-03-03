/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <utility>

struct SDL_Window;
struct ImGuiIO;

/**
 * Helping setup the imgui context
 * Such as style or theme stuff
 */
void setup_imgui();
/**
 * Setup build-in theme
 */
void setup_catppuccin_mocha_theme();
/**
 * Create the imgui context, use it in the begining and out of the loop
 */
void begin_imgui(struct SDL_Window *window, void *sdl_gl_context, const char *glsl_version);
/**
 * Destory the imgui context, use it in the end and out of the loop
 */
void end_imgui();
/**
 * Create the SDL context
 */
std::tuple<struct SDL_Window*, const char*> begin_sdl();
/**
 * Destory the SDL context
 */
void end_sdl(struct SDL_Window *window);
/**
 * Loop begin
 */
void begin_loop();
/**
 * Loop end
 * Clear color and swap the chain and stuff
 */
void end_loop(struct SDL_Window* window, struct ImGuiIO &io);

