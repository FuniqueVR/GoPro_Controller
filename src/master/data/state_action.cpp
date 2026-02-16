/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "state_action.h"

void init_state_setup(
    std::shared_ptr<json> servers,
    std::shared_ptr<json> gui,
    std::shared_ptr<GlobalState> global_state,
    std::shared_ptr<GoProMaster> master,
    std::shared_ptr<BaseWindow> windows[]
){
    if((*servers)["data"].is_array()){
        for(int i = 0; i < (*servers)["data"].size(); i++){
            if((*servers)["data"].at(i).is_string()){
                std::string buffer_ip = (*servers)["data"].at(i).get<std::string>();
                std::string ip = master->addServer(buffer_ip);
                master->reconnect(ip);
            }
        }
    }
    if((*servers)["global"].is_object()){
        set_global_state_data(*global_state, (*servers)["global"]);
    }
    if((*servers)["window"].is_object()){
        json websocket_win_json = (*servers)["window"]["websocket_win"];
        if(websocket_win_json.is_object()){
            windows[0]->set_window_data(websocket_win_json);
        }
        json commands_win_json = (*servers)["window"]["commands_win"];
        if(commands_win_json.is_object()){
            windows[1]->set_window_data(commands_win_json);
        }
        json camera_list_win_json = (*servers)["window"]["camera_list_win"];
        if(camera_list_win_json.is_object()){
            windows[2]->set_window_data(camera_list_win_json);
        }
        json inspector_win_json = (*servers)["window"]["inspector_win"];
        if(inspector_win_json.is_object()){
            windows[3]->set_window_data(inspector_win_json);
        }
    }

    if((*gui)["websocket_server_window"].is_boolean() && (*gui)["websocket_server_window"].get<bool>()){
        windows[0]->enable = true;
        windows[0]->trigger(true);
        std::cout << "Detect websocket_server_window gui is on" << std::endl;
    }
    if((*gui)["commands_win"].is_boolean() && (*gui)["commands_win"].get<bool>()){
        windows[1]->enable = true;
        windows[1]->trigger(true);
    }
    if((*gui)["camera_list_win"].is_boolean() && (*gui)["camera_list_win"].get<bool>()){
        windows[2]->enable = true;
        windows[2]->trigger(true);
    }
    if((*gui)["inspector_win"].is_boolean() && (*gui)["inspector_win"].get<bool>()){
        windows[3]->enable = true;
        windows[3]->trigger(true);
    }
}

json get_global_state_data(GlobalState& data){
    json r = json::object();
    return r;
}

void set_global_state_data(GlobalState& data, json refs){
    
}