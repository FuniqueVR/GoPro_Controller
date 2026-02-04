/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
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
#include "GoProMaster.h"
#include "IO.h"
#include "../common/camera_code.h"

// Simple Video Placeholder
// TODO: Real Implementation
#include <opencv2/opencv.hpp>

bool done = false;
GoProMaster master;
json gui;
json servers;
char server_ip_buf[64] = "192.168.10.2";
char popup1_server_ip_buf[64] = "192.168.10.2";
char popup1_camera_serial_buf[64] = "1234";
char popup1_error[64] = "";
char popup2_server_ip_buf[64] = "192.168.10.2";
char popup2_error[64] = "";

std::string websocket_server_selection = "";
std::string camera_selection = "";
static const char* current_mode_item = "PHOTO##SCC";
static const char* current_preset_item = NULL;

// Current select camera setting
ConvertSetting current_setting_items;
bool current_setting_items_bind = false;
// Current select camera IP address
std::string current_camera_item = "";

// All the window flags
bool websocket_server_window = false;
bool camera_list_win = false;
bool global_command_win = false;
bool local_command_win = false;
bool inspector_win = false;
bool record_win = false;

// The secondary thread handle the background update
// This will automatically retry connect to server every 10 seconds.
void background_worker(){
    while(done){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void setup_catppuccin_mocha_theme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Catppuccin Mocha Palette
    // --------------------------------------------------------
    const ImVec4 base       = ImVec4(0.117f, 0.117f, 0.172f, 1.0f); // #1e1e2e
    const ImVec4 mantle     = ImVec4(0.109f, 0.109f, 0.156f, 1.0f); // #181825
    const ImVec4 surface0   = ImVec4(0.200f, 0.207f, 0.286f, 1.0f); // #313244
    const ImVec4 surface1   = ImVec4(0.247f, 0.254f, 0.337f, 1.0f); // #3f4056
    const ImVec4 surface2   = ImVec4(0.290f, 0.301f, 0.388f, 1.0f); // #4a4d63
    const ImVec4 overlay0   = ImVec4(0.396f, 0.403f, 0.486f, 1.0f); // #65677c
    const ImVec4 overlay2   = ImVec4(0.576f, 0.584f, 0.654f, 1.0f); // #9399b2
    const ImVec4 text       = ImVec4(0.803f, 0.815f, 0.878f, 1.0f); // #cdd6f4
    const ImVec4 subtext0   = ImVec4(0.639f, 0.658f, 0.764f, 1.0f); // #a3a8c3
    const ImVec4 mauve      = ImVec4(0.796f, 0.698f, 0.972f, 1.0f); // #cba6f7
    const ImVec4 peach      = ImVec4(0.980f, 0.709f, 0.572f, 1.0f); // #fab387
    const ImVec4 yellow     = ImVec4(0.980f, 0.913f, 0.596f, 1.0f); // #f9e2af
    const ImVec4 green      = ImVec4(0.650f, 0.890f, 0.631f, 1.0f); // #a6e3a1
    const ImVec4 teal       = ImVec4(0.580f, 0.886f, 0.819f, 1.0f); // #94e2d5
    const ImVec4 sapphire   = ImVec4(0.458f, 0.784f, 0.878f, 1.0f); // #74c7ec
    const ImVec4 blue       = ImVec4(0.533f, 0.698f, 0.976f, 1.0f); // #89b4fa
    const ImVec4 lavender   = ImVec4(0.709f, 0.764f, 0.980f, 1.0f); // #b4befe

    // Main window and backgrounds
    colors[ImGuiCol_WindowBg]             = base;
    colors[ImGuiCol_ChildBg]              = base;
    colors[ImGuiCol_PopupBg]              = surface0;
    colors[ImGuiCol_Border]               = surface1;
    colors[ImGuiCol_BorderShadow]         = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_FrameBg]              = surface0;
    colors[ImGuiCol_FrameBgHovered]       = surface1;
    colors[ImGuiCol_FrameBgActive]        = surface2;
    colors[ImGuiCol_TitleBg]              = mantle;
    colors[ImGuiCol_TitleBgActive]        = surface0;
    colors[ImGuiCol_TitleBgCollapsed]     = mantle;
    colors[ImGuiCol_MenuBarBg]            = mantle;
    colors[ImGuiCol_ScrollbarBg]          = surface0;
    colors[ImGuiCol_ScrollbarGrab]        = surface2;
    colors[ImGuiCol_ScrollbarGrabHovered] = overlay0;
    colors[ImGuiCol_ScrollbarGrabActive]  = overlay2;
    colors[ImGuiCol_CheckMark]            = green;
    colors[ImGuiCol_SliderGrab]           = sapphire;
    colors[ImGuiCol_SliderGrabActive]     = blue;
    colors[ImGuiCol_Button]               = surface0;
    colors[ImGuiCol_ButtonHovered]        = surface1;
    colors[ImGuiCol_ButtonActive]         = surface2;
    colors[ImGuiCol_Header]               = surface0;
    colors[ImGuiCol_HeaderHovered]        = surface1;
    colors[ImGuiCol_HeaderActive]         = surface2;
    colors[ImGuiCol_Separator]            = surface1;
    colors[ImGuiCol_SeparatorHovered]     = mauve;
    colors[ImGuiCol_SeparatorActive]      = mauve;
    colors[ImGuiCol_ResizeGrip]           = surface2;
    colors[ImGuiCol_ResizeGripHovered]    = mauve;
    colors[ImGuiCol_ResizeGripActive]     = mauve;
    colors[ImGuiCol_Tab]                  = surface0;
    colors[ImGuiCol_TabHovered]           = surface2;
    colors[ImGuiCol_TabActive]            = surface1;
    colors[ImGuiCol_TabUnfocused]         = surface0;
    colors[ImGuiCol_TabUnfocusedActive]   = surface1;
    colors[ImGuiCol_DockingPreview]       = sapphire;
    colors[ImGuiCol_DockingEmptyBg]       = base;
    colors[ImGuiCol_PlotLines]            = blue;
    colors[ImGuiCol_PlotLinesHovered]     = peach;
    colors[ImGuiCol_PlotHistogram]        = teal;
    colors[ImGuiCol_PlotHistogramHovered] = green;
    colors[ImGuiCol_TableHeaderBg]        = surface0;
    colors[ImGuiCol_TableBorderStrong]    = surface1;
    colors[ImGuiCol_TableBorderLight]     = surface0;
    colors[ImGuiCol_TableRowBg]           = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_TableRowBgAlt]        = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]       = surface2;
    colors[ImGuiCol_DragDropTarget]       = yellow;
    colors[ImGuiCol_NavHighlight]         = lavender;
    colors[ImGuiCol_NavWindowingHighlight]= ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
    colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
    colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.0f, 0.0f, 0.0f, 0.35f);
    colors[ImGuiCol_Text]                 = text;
    colors[ImGuiCol_TextDisabled]         = subtext0;

    // Rounded corners
    style.WindowRounding    = 6.0f;
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 4.0f;

    // Padding and spacing
    style.WindowPadding     = ImVec2(8.0f, 8.0f);
    style.FramePadding      = ImVec2(5.0f, 3.0f);
    style.ItemSpacing       = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing  = ImVec2(4.0f, 4.0f);
    style.IndentSpacing     = 21.0f;
    style.ScrollbarSize     = 14.0f;
    style.GrabMinSize       = 10.0f;

    // Borders
    style.WindowBorderSize  = 1.0f;
    style.ChildBorderSize   = 1.0f;
    style.PopupBorderSize   = 1.0f;
    style.FrameBorderSize   = 0.0f;
    style.TabBorderSize     = 0.0f;
}

void updateServerList(){
    json data = json::object();
    data["data"] = json::array();
    for(const auto s : master.getServers()){
        if(s){
            data["data"].push_back(s->ip);
        }
    }
    servers = data;
    saveServerList(data);
}

int main(int, char**)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
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

    for(int i = 0; i < GOPRO_SETTING_SIZE; i++){
        int32_t id = GOPRO_SETTING_IDS[i];
        current_setting_items.values[i] = 0;
    }

    if(servers["data"].is_array()){
        for(int i = 0; i < servers["data"].size(); i++){
            if(servers["data"].at(i).is_string()){
                std::string buffer_ip = servers["data"].at(i).get<std::string>();
                std::string ip = master.addServer(server_ip_buf);
                master.reconnect(ip);
            }
        }
    }

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
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDpiScaleFonts = true;
    io.ConfigDpiScaleViewports = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    setup_catppuccin_mocha_theme();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
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
            bool update_menu = false;
            update_menu = ImGui::MenuItem("Websocket Dashboard", NULL, &websocket_server_window);
            update_menu = update_menu || ImGui::MenuItem("Camera List", NULL, &camera_list_win);
            update_menu = update_menu || ImGui::MenuItem("Global Command", NULL, &global_command_win);
            update_menu = update_menu || ImGui::MenuItem("Local Command", NULL, &local_command_win);
            update_menu = update_menu || ImGui::MenuItem("Inspector", NULL, &inspector_win);
            update_menu = update_menu || ImGui::MenuItem("Record", NULL, &record_win);
            ImGui::EndMenu();
            if(update_menu){
                gui["websocket_server_window"] = websocket_server_window;
                gui["camera_list_win"] = camera_list_win;
                gui["global_command_win"] = global_command_win;
                gui["local_command_win"] = local_command_win;
                gui["inspector_win"] = inspector_win;
                gui["record_win"] = record_win;
                saveGUI(gui);
                ImGui::SaveIniSettingsToDisk("imgui.ini");
            }
        }
        ImGui::EndMainMenuBar();

        //ImGui::ShowDemoWindow();

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
                std::string ip = master.addServer(server_ip_buf);
                master.reconnect(ip);
                updateServerList();
            }
            ImGui::SameLine();
            if (ImGui::Button("Remove Server")) {
                master.clean(std::string(server_ip_buf));
                updateServerList();
            }
            ImGui::SameLine();
            if (ImGui::Button("Disconnect Server")) {
                master.disconnect(std::string(server_ip_buf));
            }

            if (ImGui::Button("Reconnect All")) {
                master.reconnectAll();
            }
            ImGui::SameLine();
            if (ImGui::Button("Disconnect All")) {
                master.disconnectAll();
            }
            ImGui::SameLine();
            if (ImGui::Button("Clean")) {
                master.cleanAll();
                updateServerList();
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
            {
                for(const auto& c : master.getCameras()){
                    if(c){
                        std::lock_guard<std::mutex> lock(master.camera_mtx);
                        bool selected = c->ip == current_camera_item;
                        if(ImGui::Selectable(c->ip.c_str(), selected)){
                            // User select interaction
                            current_setting_items_bind = false;
                            current_camera_item = c->ip;
                            std::cout << "Select camera: " << c->ip << std::endl;
                            master.query_only(c->server, "get", c->ip);
                            //current_setting_items_bind = master.getSettingsFromCamera(*c, current_setting_items);
                        }
                    }
                }
            }
            ImGui::End();
        }

        if(global_command_win) {
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("Group Command");
            {
                ImGui::LabelText("Global Controls", "Commands applied to all connected cameras");

                if(ImGui::Button("Scan All")) master.command_only("scan"); ImGui::SameLine();
                if(ImGui::Button("Add Server")) master.stopRecordingAll();

                if(ImGui::Button("Add Camera")) ImGui::OpenPopup("Add Camera"); ImGui::SameLine();
                if(ImGui::Button("Clean Camera")) master.command_only("clean");

                if(ImGui::Button("Connect All")) master.command_only("usb_on"); ImGui::SameLine();
                if(ImGui::Button("Disconnect All")) master.command_only("usb_off");

                if(ImGui::Button("Reboot All")) master.command_only("reboot"); ImGui::SameLine();
                if(ImGui::Button("Shutdown All")) master.command_only("shutdown");

                if(ImGui::Button("Record All")) master.command_only("shutter_on"); ImGui::SameLine();
                if(ImGui::Button("Stop All")) master.command_only("shutter_off");

                if(ImGui::Button("Enter Webcam")) master.webcam_only("preview"); ImGui::SameLine();
                if(ImGui::Button("Exit Webcam")) master.webcam_only("exit");
            }
            ImGui::End();
        }

        if(local_command_win) {
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("Camera Command##SCC");
            {
                ImGui::LabelText("Single Camera Control", "Commands applied to selected camera");

                bool should_disabled = current_camera_item.size() < 10 || master.findCamera(current_camera_item) == -1;
                ImGui::BeginDisabled(should_disabled);

                if(ImGui::Button("Record")) master.startRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Stop")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Setting Apply All")) master.applyAll(current_camera_item, current_setting_items);

                if(ImGui::Button("Connect")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Disconnect")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Shutdown")) master.stopRecordingAll(); ImGui::SameLine();
                if(ImGui::Button("Locate")) master.stopRecordingAll();

                const char* mode_items[] = { "PHOTO##SCC", "VIDEO##SCC", "WEBCAM##SCC"};
                if(ImGui::BeginCombo("Mode##SCC", current_mode_item)){
                    for (int n = 0; n < 3; n++)
                    {
                        bool is_selected = (current_mode_item == mode_items[n]); // You can store your selection however you want, outside or inside your objects
                        if (ImGui::Selectable(mode_items[n], is_selected))
                            current_mode_item = mode_items[n];
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                    }
                    ImGui::EndCombo();
                }
                
                const char* preset_items[] = { };
                if(ImGui::BeginCombo("Preset##SCC", current_preset_item)){
                    ImGui::EndCombo();
                }

                ImGui::EndDisabled();
            }
            ImGui::End();
        }

        if(inspector_win) {
            //ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("Inspector");
            {
                int32_t camera_ip = master.findCamera(current_camera_item);
                bool should_disabled = current_camera_item.size() < 10 || camera_ip == -1 || !current_setting_items_bind;
                ImGui::BeginDisabled(should_disabled);

                for(int i = 0; i < GOPRO_SETTING_SIZE; i++){
                    int id = GOPRO_SETTING_IDS[i];
                    std::string name = GET_SETTING_NAME_BY_ID(id);
                    if(name.size() == 0) continue;
                    name += "##InspectorTitle";
                    int32_t select_id = current_setting_items.values[i];
                    const char* select_string = GET_SETTING_STRING_BY_ID(id)[select_id];

                    if(ImGui::BeginCombo(name.c_str(), select_string)){
                        for (int n = 0; n < GET_SETTING_SIZE_BY_ID(id); n++)
                        {
                            std::string option = GET_SETTING_STRING_BY_ID(id)[n];
                            if(option.size() == 0) continue;
                            option += ("##InspectorOption_" + name); 
                            bool is_selected = (current_mode_item == option); // You can store your selection however you want, outside or inside your objects
                            if (ImGui::Selectable(option.c_str(), is_selected))
                            {
                                current_setting_items.values[i] = n;
                            }
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                        }
                        ImGui::EndCombo();
                    }
                }
                ImGui::EndDisabled();
            }
            ImGui::End();
        }

        if(record_win){
            ImGui::SetNextWindowContentSize(ImVec2(600, 400));
            ImGui::Begin("Record");
            ImGui::End();
        }

        if(ImGui::BeginPopupModal("Add Camera")){
            ImGui::InputText("Server IP", popup1_server_ip_buf, IM_ARRAYSIZE(popup1_server_ip_buf));
            ImGui::InputText("Camera IP", popup1_camera_serial_buf, IM_ARRAYSIZE(popup1_camera_serial_buf));
            ImGui::TextColored(ImVec4(1, 0, 0, 1), popup1_error);
            if (ImGui::Button("Confirm")) {
                bool pass = true;
                if(master.findServer(popup1_server_ip_buf) == -1){
                    strncpy(popup1_error, "Server does not exist.", sizeof(popup1_error) - 1);
                    popup1_error[sizeof(popup1_error) - 1] = 0;
                    pass = false;
                }
                if(master.findCamera(GetRemoteIPBySerial(popup1_camera_serial_buf)) != -1){
                    strncpy(popup1_error, "Camera already added.", sizeof(popup1_error) - 1);
                    popup1_error[sizeof(popup1_error) - 1] = 0;
                    pass = false;
                }

                if(pass){
                    master.command_only(popup1_server_ip_buf, "add", popup1_camera_serial_buf);
                }
            }
            ImGui::EndPopup();
        }
        
        if(ImGui::BeginPopupModal("Scan Camera")){
            ImGui::InputText("Server IP", popup2_server_ip_buf, IM_ARRAYSIZE(popup2_server_ip_buf));
            ImGui::Text("You can leave it empty for broadcast to all websocket server");
            ImGui::TextColored(ImVec4(1, 0, 0, 1), popup2_error);
            if (ImGui::Button("Confirm")) {
                bool pass = true;
                if(master.findServer(popup2_server_ip_buf) == -1 && sizeof(popup2_server_ip_buf) == 0){
                    strncpy(popup2_error, "Server does not exist.", sizeof(popup2_error) - 1);
                    popup2_error[sizeof(popup2_error) - 1] = 0;
                    pass = false;
                }

                if(pass){
                    master.command_only(popup2_server_ip_buf, "scan", "");
                }
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

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        SDL_GL_SwapWindow(window);
    }

    master.setdone();
    master.disconnectAll();
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
