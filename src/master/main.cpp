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
#include "GoProMaster.h"
#include "io.h"
#include "data/state_action.h"
#include "../common/camera_code.h"
#include "windows/wins.h"
#include "popup/popwins.h"
#include "imgui_helper.h"

#define WIN_INIT(a, b, c, d) \
a = std::make_shared<b>(gui, global_state, master); \
c[d] = a; \

#define WIN_INIT2(a, b, r, c, d) \
a = std::make_shared<b>(r, gui, global_state, master); \
c[d] = a; \

std::queue<std::string> command_queue = std::queue<std::string>();
std::shared_ptr<GoProMaster> master = std::make_shared<GoProMaster>();
std::shared_ptr<json> gui;
std::shared_ptr<json> servers;
std::shared_ptr<GlobalState> global_state = std::make_shared<GlobalState>();

std::shared_ptr<CameraListWindow> camera_list_win;
std::shared_ptr<CommandWindow> commands_win;
std::shared_ptr<InspectorWindow> inspector_win;
std::shared_ptr<WebsocketWindow> websocket_win;
std::shared_ptr<StyleSetting> style_setting_win;
std::shared_ptr<BaseWindow> windows_array[5];

std::shared_ptr<AddCameraPopup> add_camera_popwin;
std::shared_ptr<ScanCameraPopup> scan_camera_popwin;
std::shared_ptr<StartWebcamPopup> start_webcam_popwin;
std::shared_ptr<PreviewPopup> preview_popwin;
std::shared_ptr<BasePopWindow> pop_windows_array[4];

// All the window flags
ExecutionType execution_type = ExecutionType::SetAll;
std::unordered_map<std::string, std::string> execution_logs = std::unordered_map<std::string, std::string>();

// The secondary thread handle the background update
// This will automatically retry connect to server every 10 seconds.
void background_worker(){
    while(!global_state->done){
        while(!command_queue.empty()){
            std::string cmd = command_queue.front();
            command_queue.pop();
            if(cmd == "add_camera"){
                add_camera_popwin->trigger(true);
                std::cout << "Detect add_camera popup" << std::endl;
            }
            else if(cmd == "scan_camera"){
                scan_camera_popwin->trigger(true);
                std::cout << "Detect scan_camera popup" << std::endl;
            }
            else if(cmd == "start_webcam"){
                start_webcam_popwin->trigger(true);
                std::cout << "Detect start_webcam popup" << std::endl;
            }
            else if(cmd == "preview_start"){
                preview_popwin->trigger(true);
                std::cout << "Detect preview popup" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void assign_log(std::string key, std::string value){
    if(execution_logs.count(key)){
        std::string b = execution_logs.at(key);
        execution_logs.insert_or_assign(key, b + "\n" + value);
    }else{
        execution_logs.insert_or_assign(key, value);
    }
}

void settingGetterFeedback(std::string ip, json setting){
    if(global_state->current_camera_item == ip){
        global_state->current_setting_items = setting;
        global_state->current_setting_items_bind = true;
    }
}
void statusGetterFeedback(std::string ip, json status){
    if(global_state->current_camera_item == ip){
        global_state->current_status_items = status;
        global_state->current_status_items_bind = true;
    }
}

void updateServerList(){
    json data = json::object();
    data["data"] = json::array();
    for(const auto& s : master->getServers()){
        if(s){
            data["data"].push_back(s->ip);
        }
    }
    data["global"] = get_global_state_data(*global_state);
    data["window"] = json::object();
    data["window"]["camera_list_win"] = camera_list_win->get_window_data();
    data["window"]["commands_win"] = commands_win->get_window_data();
    data["window"]["inspector_win"] = inspector_win->get_window_data();
    data["window"]["websocket_win"] = websocket_win->get_window_data();
    data["popwin"]["style_setting_win"] = style_setting_win->get_window_data();
    data["popwin"]["preview_popwin"] = preview_popwin->get_window_data();
    saveServerList(data);
    servers->swap(data);
}

void updateGUIList(){
    (*gui)["websocket_server_window"] = websocket_win->is_enable();
    (*gui)["camera_list_win"] = camera_list_win->is_enable();
    (*gui)["commands_win"] = commands_win->is_enable();
    (*gui)["inspector_win"] = inspector_win->is_enable();
    (*gui)["style_setting_win"] = style_setting_win->is_enable();
    saveGUI(*gui);
    ImGui::SaveIniSettingsToDisk("imgui.ini");
}

void pushCommand(const char* cmd){
    command_queue.push(cmd);
}

int main(int, char**)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    
    // Setup SDL
    SDL_Window* window;
    SDL_GLContext gl_context;
    const char* glsl_version;
    {
        std::tuple<SDL_Window*, const char*> sdl_ctx;
        begin_sdl(sdl_ctx);
        window = std::get<0>(sdl_ctx);
        gl_context = SDL_GL_CreateContext(window);
        glsl_version = std::get<1>(sdl_ctx);
        SDL_GL_MakeCurrent(window, gl_context);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    servers = std::make_shared<json>(loadServerList());
    gui = std::make_shared<json>(loadGUI());
    // Win
    WIN_INIT(websocket_win, WebsocketWindow, windows_array, 0);
    WIN_INIT(commands_win, CommandWindow, windows_array, 1);
    WIN_INIT(camera_list_win, CameraListWindow, windows_array, 2);
    WIN_INIT(inspector_win, InspectorWindow, windows_array, 3);
    WIN_INIT(style_setting_win, StyleSetting, windows_array, 4);
    // Popup
    WIN_INIT(add_camera_popwin, AddCameraPopup, pop_windows_array, 0);
    WIN_INIT(scan_camera_popwin, ScanCameraPopup, pop_windows_array, 1);
    WIN_INIT(start_webcam_popwin, StartWebcamPopup, pop_windows_array, 2);
    WIN_INIT2(preview_popwin, PreviewPopup, renderer, pop_windows_array, 3);
    // Register event for master
    master->registerCameraSettingFeedback(settingGetterFeedback);
    master->registerCameraStatusFeedback(statusGetterFeedback);
    master->registerCameraLogFeedback(assign_log);
    global_state->update_server = updateServerList;
    global_state->command_sender = pushCommand;
    std::thread bg_thread(background_worker);
    // Init the windows
    init_state_setup(servers, gui, global_state, master, windows_array, pop_windows_array);

    setup_imgui();
    setup_catppuccin_mocha_theme();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer backends
    begin_imgui(window, gl_context, glsl_version);

    // Main loop
    while (!global_state->done)
    {
        // Poll and handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                global_state->done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                global_state->done = true;
            
            // Hotkeys
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_F2) {
                    master->command_only("shutter_on");
                    printf("Hotkey F2: Start Recording\n");
                }
                if (event.key.key == SDLK_F3) {
                    master->command_only("shutter_off");
                    printf("Hotkey F3: Stop Recording\n");
                }
                if (event.key.key == SDLK_F4) {
                    master->presetSwitch("", "", 65536);
                    printf("Hotkey F4: Photo Mode\n");
                }
                if (event.key.key == SDLK_F5) {
                    master->presetSwitch("", "", 0);
                    printf("Hotkey F5: Video Mode\n");
                }
                if (event.key.key == SDLK_F11) {
                    uint32_t flag = SDL_GetWindowFlags(window);
                    if ((flag & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN) {
                        SDL_SetWindowFullscreen(window, false);
                        printf("Window using exclusive (true) fullscreen mode\n");
                    } else {
                        SDL_SetWindowFullscreen(window, true);
                        printf("Window in windowed mode\n");
                    }
                }
                if (event.key.key == SDLK_Q) {
                    websocket_win->trigger(!websocket_win->is_enable());
                    updateGUIList();
                }
                if (event.key.key == SDLK_W) {
                    commands_win->trigger(!commands_win->is_enable());
                    updateGUIList();
                }
                if (event.key.key == SDLK_E) {
                    camera_list_win->trigger(!camera_list_win->is_enable());
                    updateGUIList();
                }
                if (event.key.key == SDLK_R) {
                    inspector_win->trigger(!inspector_win->is_enable());
                    updateGUIList();
                }
                if (event.key.key == SDLK_T) {
                    style_setting_win->trigger(!style_setting_win->is_enable());
                    updateGUIList();
                }
            }
        }

        for(auto& w : windows_array){
            if(w && w->is_enable()){
                w->update();
            }
        }
        for(auto& w : pop_windows_array){
            if(w && w->is_open()){
                w->update();
            }
        }

        // Start the Dear ImGui frame
        begin_loop();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Windows")) {
            bool update_menu = false;
            if(ImGui::MenuItem("Websocket Dashboard (Q)", NULL, websocket_win->is_enable())){
                websocket_win->trigger(!websocket_win->is_enable());
                update_menu = true;
            }
            if(ImGui::MenuItem("Command Sender (W)", NULL, commands_win->is_enable())){
                commands_win->trigger(!commands_win->is_enable());
                update_menu = true;
            }
            if(ImGui::MenuItem("Camera List (E)", NULL, camera_list_win->is_enable())){
                camera_list_win->trigger(!camera_list_win->is_enable());
                update_menu = true;
            }
            if(ImGui::MenuItem("Inspector (R)", NULL, inspector_win->is_enable())){
                inspector_win->trigger(!inspector_win->is_enable());
                update_menu = true;
            }
            if(ImGui::MenuItem("Style (T)", NULL, style_setting_win->is_enable())){
                style_setting_win->trigger(!style_setting_win->is_enable());
                update_menu = true;
            }
            ImGui::Separator();
            //update_menu = update_menu || ImGui::MenuItem("System Style", NULL, &system_style_win);
            ImGui::EndMenu();
            if(update_menu){
                updateGUIList();
            }
        }
        ImGui::EndMainMenuBar();

        for(auto& w : windows_array){
            if(w && w->is_enable()){
                w->render();
                if(w->is_close()){
                    w->trigger(false);
                    updateGUIList();
                }
            }
        }

        for(auto& w : pop_windows_array){
            if(w){
                w->detect();
            }
        }

        for(auto& w : pop_windows_array){
            if(w){
                w->render();
            }
        }
        // Rendering
        ImGui::Render();
        end_loop(window, io);
    }

    master->setdone();
    master->disconnectAll();
    if(bg_thread.joinable()){
        bg_thread.join();
    }
    // Cleanup
    end_imgui();
    SDL_GL_DestroyContext(gl_context);
    end_sdl(window);

    return 0;
}
