#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>
#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "../common/iphelper.h"
#include "GoProMaster.h"
#include "IO.h"

// Simple Video Placeholder
// TODO: Real Implementation
#include <opencv2/opencv.hpp>

bool done = false;
GoProMaster master;
json gui;
json servers;
char server_ip_buf[64] = "192.168.10.2";

// The secondary thread handle the background update
// This will automatically retry connect to server every 10 seconds.
void background_worker(){
    while(done){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int main(int, char**)
{
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("GoPro Controller Master", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_ShowWindow(window);

    servers = loadServerList();
    gui = loadGUI();
    std::thread bg_thread(background_worker);

    // Main loop
    bool websocket_server_window = false;
    bool camera_list_win = false;
    bool global_command_win = false;
    bool local_command_win = false;
    bool inspector_win = false;
    bool record_win = false;

    bool popup_add_camera = false;
    bool popup_scan_camera = false;

    if(gui["websocket_server_window"].is_boolean() && gui["websocket_server_window"].get<bool>()){
        websocket_server_window = true;
    }
    if(gui["camera_list_win"].is_boolean() && gui["camera_list_win"].get<bool>()){
        camera_list_win = true;
    }
    if(gui["global_command_win"].is_boolean() && gui["global_command_win"].get<bool>()){
        global_command_win = true;
    }
    if(gui["local_command_win"].is_boolean() && gui["local_command_win"].get<bool>()){
        local_command_win = true;
    }
    if(gui["inspector_win"].is_boolean() && gui["inspector_win"].get<bool>()){
        inspector_win = true;
    }
    if(gui["record_win"].is_boolean() && gui["record_win"].get<bool>()){
        record_win = true;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while (!done)
    {
        // Poll and handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            
            // Hotkeys
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_F2) {
                    master.startRecordingAll();
                    printf("Hotkey F2: Start Recording\n");
                }
                if (event.key.key == SDLK_F3) {
                    master.stopRecordingAll();
                    printf("Hotkey F3: Stop Recording\n");
                }
                if (event.key.key == SDLK_F4) {
                    master.setModePhotoAll();
                    printf("Hotkey F4: Photo Mode\n");
                }
                if (event.key.key == SDLK_F5) {
                    master.setModeVideoAll();
                    printf("Hotkey F5: Video Mode\n");
                }
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Windows")) {
            ImGui::MenuItem("Websocket Dashboard", NULL, &websocket_server_window);
            ImGui::MenuItem("Camera List", NULL, &camera_list_win);
            ImGui::MenuItem("Global Command", NULL, &global_command_win);
            ImGui::MenuItem("Local Command", NULL, &local_command_win);
            ImGui::MenuItem("Inspector", NULL, &inspector_win);
            ImGui::MenuItem("Record", NULL, &record_win);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        // 1. Dashboard Window
        if(websocket_server_window) {
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("Websocket Dashboard");

            ImGui::Text("Hotkeys:");
            ImGui::BulletText("F2: Start Recording");
            ImGui::BulletText("F3: Stop Recording");
            ImGui::BulletText("F4: Switch to Photo Mode");
            ImGui::BulletText("F5: Switch to Video Mode");

            ImGui::Separator();

            ImGui::Text("Manual Control:");
            if (ImGui::Button("Start Rec (F2)")) master.startRecordingAll(); ImGui::SameLine();
            if (ImGui::Button("Stop Rec (F3)")) master.stopRecordingAll();
            if (ImGui::Button("Photo Mode (F4)")) master.setModePhotoAll(); ImGui::SameLine();
            if (ImGui::Button("Video Mode (F5)")) master.setModeVideoAll();

            ImGui::Separator();

            ImGui::Text("Server Connections:");
            ImGui::InputText("Server IP", server_ip_buf, IM_ARRAYSIZE(server_ip_buf));
            if (ImGui::Button("Add Server")) {
                master.addServer(server_ip_buf);
                master.connectAll();
            }
            ImGui::SameLine();
            if (ImGui::Button("Reconnect All")) {
                master.connectAll();
            }

            // Server List Table
            if (ImGui::BeginTable("Servers", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("IP Address");
                ImGui::TableSetupColumn("Status");
                ImGui::TableSetupColumn("Last Message");
                ImGui::TableHeadersRow();

                for (const auto& s : master.getServers()) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", s->ip.c_str());
                    ImGui::TableSetColumnIndex(1);
                    if (s->connected) 
                        ImGui::TextColored(ImVec4(0,1,0,1), "Connected");
                    else 
                        ImGui::TextColored(ImVec4(1,0,0,1), "Disconnected");
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", s->last_message.c_str());
                }
                ImGui::EndTable();
            }

            ImGui::End();
        }

        if(camera_list_win) {
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("GoPro Dashboard");
            ImGui::End();
        }

        if(global_command_win) {
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("Group Command");
            {
                ImGui::LabelText("Global Controls", "Commands applied to all connected cameras");

                if(ImGui::Button("Scan All")) master.startRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Add Server")) master.stopRecordingAll();

                if(ImGui::Button("Add Camera")) master.startRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Clean Camera")) master.stopRecordingAll();

                if(ImGui::Button("Connect All")) master.startRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Disconnect All")) master.stopRecordingAll();

                if(ImGui::Button("Reboot")) master.startRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Shutdown")) master.stopRecordingAll();

                if(ImGui::Button("Record All")) master.startRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Stop All")) master.stopRecordingAll();
            }
            ImGui::End();
        }

        if(local_command_win) {
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("SCC#Camera Command");
            {
                ImGui::LabelText("Single Camera Control", "Commands applied to selected camera");

                if(ImGui::Button("Record")) master.startRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Stop")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Setting Apply All")) master.stopRecordingAll();

                if(ImGui::Button("Connect")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Disconnect")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Shutdown")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Locate")) master.stopRecordingAll();

                if(ImGui::BeginListBox("SCC#Mode")){
                    ImGui::Selectable("SCC_Mode#PHOTO");
                    ImGui::Selectable("SCC_Mode#VIDEO");
                    ImGui::Selectable("SCC_Mode#WEBCAM");
                    ImGui::EndListBox();
                }

                if(ImGui::BeginListBox("SCC#Preset")){
                    ImGui::EndListBox();
                }
            }
            ImGui::End();
        }

        if(inspector_win) {
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("Inspector");
            ImGui::End();
        }

        if(record_win){

        }

        if(popup_add_camera){
            ImGui::BeginPopupModal("Add Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::InputText("Camera IP", server_ip_buf, IM_ARRAYSIZE(server_ip_buf));
            if (ImGui::Button("Add")) {
                master.addServer(server_ip_buf);
                master.connectAll();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if(popup_scan_camera){
            ImGui::BeginPopupModal("Scan Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::InputText("Camera IP", server_ip_buf, IM_ARRAYSIZE(server_ip_buf));
            if (ImGui::Button("Add")) {
                master.addServer(server_ip_buf);
                master.connectAll();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }


    if(bg_thread.joinable()){
        bg_thread.join();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
