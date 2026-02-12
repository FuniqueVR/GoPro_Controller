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
#include "misc/cpp/imgui_stdlib.h"
#include "GoProMaster.h"
#include "IO.h"
#include "state.h"
#include "../common/camera_code.h"
#include "windows/wins.h"
#include "popup/popwins.h"
#include "imgui_helper.h"

std::queue<std::string> command_queue;
std::shared_ptr<GoProMaster> master;
std::shared_ptr<json> gui;
std::shared_ptr<json> servers;
std::shared_ptr<GlobalState> global_state;

std::shared_ptr<CameraListWindow> camera_list_win;
std::shared_ptr<CommandWindow> commands_win;
std::shared_ptr<InspectorWindow> inspector_win;
std::shared_ptr<WebsocketWindow> websocket_win;

std::shared_ptr<BaseWindow> windows_array[] = {
    websocket_win,
    commands_win,
    camera_list_win,
    inspector_win,
};

std::shared_ptr<AddCameraPopup> add_camera_popwin;

std::shared_ptr<BasePopWindow> pop_windows_array[] = {
    add_camera_popwin,
};

std::string popup1_server_ip_buf = "127.0.0.1";
std::string popup1_camera_serial_buf = "1234";
std::string popup1_error = "";
std::string popup2_server_ip_buf = "127.0.0.1";
std::string popup2_error = "";
std::string popup3_server_ip_buf = "127.0.0.1";
std::string popup3_port_buf = "7000";
int32_t popup3_res_buf = 0;
std::string popup3_res_string_buf = "480p";
int32_t popup3_fov_buf = 0;
std::string popup3_fov_string_buf = "Wide";
bool popup3_ts_buf = true;
std::string popup3_error = "";

// All the window flags
bool popup_add_camera_win = false;
bool popup_scan_camera_win = false;
bool popup_start_webcam_win = false;
bool popup_execute_win = false;
ExecutionType execution_type = ExecutionType::SetAll;
std::unordered_map<std::string, std::string> execution_logs = std::unordered_map<std::string, std::string>();

// The secondary thread handle the background update
// This will automatically retry connect to server every 10 seconds.
void background_worker(){
    while(global_state->done){
        std::this_thread::sleep_for(std::chrono::seconds(10));
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

void updateServerList(){
    json data = json::object();
    data["data"] = json::array();
    for(const auto s : master->getServers()){
        if(s){
            data["data"].push_back(s->ip);
        }
    }
    servers->swap(data);
    saveServerList(data);
}

void updateGUIList(){
    (*gui)["websocket_server_window"] = websocket_win->enable;
    (*gui)["camera_list_win"] = camera_list_win->enable;
    (*gui)["commands_win"] = commands_win->enable;
    (*gui)["inspector_win"] = inspector_win->enable;
    saveGUI(*gui);
    ImGui::SaveIniSettingsToDisk("imgui.ini");
}

void pushCommand(std::string command){
    command_queue.push(command);
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
        std::tuple<SDL_Window*, SDL_GLContext, const char*> sdl_ctx = begin_sdl();
        window = std::get<0>(sdl_ctx);
        gl_context = std::get<1>(sdl_ctx);
        glsl_version = std::get<2>(sdl_ctx);
    }

    servers = std::make_shared<json>(loadServerList());
    gui = std::make_shared<json>(loadGUI());
    std::thread bg_thread(background_worker);
    camera_list_win = std::make_shared<CameraListWindow>(gui, global_state, master);
    master->registerCameraSettingFeedback(settingGetterFeedback);
    master->registerCameraLogFeedback(assign_log);
    global_state->update_server = updateServerList;

    if((*servers)["data"].is_array()){
        for(int i = 0; i < (*servers)["data"].size(); i++){
            if((*servers)["data"].at(i).is_string()){
                std::string buffer_ip = (*servers)["data"].at(i).get<std::string>();
                std::string ip = master->addServer(buffer_ip);
                master->reconnect(ip);
            }
        }
    }

    if((*gui)["websocket_server_window"].is_boolean() && (*gui)["websocket_server_window"].get<bool>()){
        websocket_win->enable = true;
    }
    if((*gui)["camera_list_win"].is_boolean() && (*gui)["camera_list_win"].get<bool>()){
        camera_list_win->enable = true;
    }
    if((*gui)["global_command_win"].is_boolean() && (*gui)["global_command_win"].get<bool>()){
        commands_win->enable = true;
    }
    if((*gui)["inspector_win"].is_boolean() && (*gui)["inspector_win"].get<bool>()){
        inspector_win->enable = true;
    }

    setup_imgui();
    setup_catppuccin_mocha_theme();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer backends
    begin_imgui(window, gl_context);

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
                    master->presetSwitch("", 65536);
                    printf("Hotkey F4: Photo Mode\n");
                }
                if (event.key.key == SDLK_F5) {
                    master->presetSwitch("", 0);
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
                if (event.key.key == SDLK_Q && !websocket_win->enable) {
                    websocket_win->enable = true;
                    updateGUIList();
                }
                if (event.key.key == SDLK_W && !commands_win->enable) {
                    commands_win->enable = true;
                    updateGUIList();
                }
                if (event.key.key == SDLK_E && !camera_list_win->enable) {
                    camera_list_win->enable = true;
                    updateGUIList();
                }
                if (event.key.key == SDLK_R && !inspector_win->enable) {
                    inspector_win->enable = true;
                    updateGUIList();
                }
            }

            for(const auto& w : windows_array){
                if(w->enable){
                    w->update();
                }
            }
        }

        // Start the Dear ImGui frame
        begin_loop();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Windows")) {
            bool update_menu = false;
            update_menu = ImGui::MenuItem("Websocket Dashboard (Q)", NULL, &websocket_win->enable);
            update_menu = update_menu || ImGui::MenuItem("Command Sender (W)", NULL, &commands_win->enable);
            update_menu = update_menu || ImGui::MenuItem("Camera List (E)", NULL, &camera_list_win->enable);
            update_menu = update_menu || ImGui::MenuItem("Inspector (R)", NULL, &inspector_win->enable);
            ImGui::Separator();
            //update_menu = update_menu || ImGui::MenuItem("System Style", NULL, &system_style_win);
            ImGui::EndMenu();
            if(update_menu){
                updateGUIList();
            }
        }
        ImGui::EndMainMenuBar();

        for(const auto& w : windows_array){
            if(w->enable){
                w->render();
                if(w->is_close()){
                    updateGUIList();
                }
            }
        }

        for(const auto& w : pop_windows_array){
            if(w->enable){
                w->detect();
            }
        }

        ImVec2 center = ImVec2(0, 0);
        if(popup_add_camera_win){
            popup_add_camera_win = false;
            ImGui::OpenPopup("Add Camera##Popup");
            center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }
        if(popup_scan_camera_win){
            popup_scan_camera_win = false;
            ImGui::OpenPopup("Scan Camera##Popup");
            center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }
        if(popup_start_webcam_win){
            popup_start_webcam_win = false;
            ImGui::OpenPopup("Start Webcam##Popup");
            center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }
        if(popup_execute_win){
            popup_execute_win = false;
            ImGui::OpenPopup("Execute Command##Popup");
            center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }

        if(ImGui::BeginPopupModal("Add Camera##Popup", NULL, wp_flag)){
            bool updated = false;
            updated = ImGui::InputText("Server IP", &popup1_server_ip_buf);
            updated = ImGui::InputText("Camera IP", &popup1_camera_serial_buf);
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", popup1_error.c_str());
            if(updated){
                popup1_error.clear();
            }
            if (ImGui::Button("Confirm")) {
                bool pass = true;
                if(master->findServer(popup1_server_ip_buf) == -1){
                    popup1_error = "Server does not exist.";
                    pass = false;
                }
                if(master->findCamera(GetRemoteIPBySerial(popup1_camera_serial_buf)) != -1){
                    popup1_error = "Camera already added.";
                    pass = false;
                }

                if(pass){
                    master->command_only(popup1_server_ip_buf, "add", std::string(popup1_camera_serial_buf));
                }
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if(ImGui::BeginPopupModal("Scan Camera##Popup", NULL, wp_flag)){
            bool updated = false;
            updated = ImGui::InputText("Server IP", &popup2_server_ip_buf);
            ImGui::Text("You can leave it empty for broadcast to all websocket server");
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", popup2_error.c_str());
            if(updated){
                popup2_error.clear();
            }
            if (ImGui::Button("Confirm")) {
                bool pass = true;
                if(master->findServer(popup2_server_ip_buf) == -1 && sizeof(popup2_server_ip_buf) == 0){
                    popup2_error = "Server does not exist.";
                    pass = false;
                }

                if(pass){
                    if(sizeof(popup2_server_ip_buf) == 0){
                        master->command_only("scan");
                    }else{
                        master->command_only(popup2_server_ip_buf, "scan", "");
                    }
                }
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();               
        }
        if(ImGui::BeginPopupModal("Start Webcam##Popup", NULL, wp_flag)){
            bool updated = false;
            updated = ImGui::InputText("Server IP", &popup3_server_ip_buf);
            updated = ImGui::InputText("Port Start", &popup3_port_buf);
            updated = ImGui::Checkbox("Use TS", &popup3_ts_buf);
            if(ImGui::BeginCombo("Res", popup3_res_string_buf.c_str())){
                for(int32_t n = 0; n < WEBCAM_START_RES_SIZE; n++){
                    std::string option = WEBCAM_START_RES_STRING[n];
                    if(option.size() == 0) continue;
                    bool is_selected = (popup3_res_string_buf == option); // You can store your selection however you want, outside or inside your objects
                    std::string option_r = option + ("##WebcamOption_" + std::string(WEBCAM_START_RES_NAME)); 
                    if (ImGui::Selectable(option_r.c_str(), is_selected))
                    {
                        updated = true;
                        popup3_res_buf = n; // Change index
                        popup3_res_string_buf = option;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus(); 
                }
                ImGui::EndCombo();
            }
            if(ImGui::BeginCombo("Fov", popup3_fov_string_buf.c_str())){
                for(int32_t n = 0; n < WEBCAM_START_FOV_SIZE; n++){
                    std::string option = WEBCAM_START_FOV_STRING[n];
                    if(option.size() == 0) continue;
                    bool is_selected = (popup3_fov_string_buf == option); // You can store your selection however you want, outside or inside your objects
                    std::string option_r = option + ("##WebcamOption_" + std::string(WEBCAM_START_FOV_NAME)); 
                    if (ImGui::Selectable(option.c_str(), is_selected))
                    {
                        updated = true;
                        popup3_fov_buf = n; // Change index
                        popup3_fov_string_buf = option;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus(); 
                }
                ImGui::EndCombo();
            }
            
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", popup3_error.c_str());
            if(updated){
                popup3_error.clear();
            }
            if(ImGui::Button("Confirm")){
                
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if(ImGui::BeginPopupModal("Execute Command##Popup", NULL, wp_flag)){
            int32_t index = 0;
            for(auto i = execution_logs.begin(); i != execution_logs.end(); ++i){
                std::string label = i->first;
                std::string data = i->second;
                label += "##Log_TreeNode_" + std::to_string(index);
                if(ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen)){
                    ImGui::Text("%s", data.c_str());
                    ImGui::TreePop();
                }
                index++;
            }

            if(ImGui::Button("Confirm")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
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
    end_sdl(window, gl_context);
}
