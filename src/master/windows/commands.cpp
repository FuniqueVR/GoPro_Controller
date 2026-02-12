#include "commands.h"

CommandWindow::CommandWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Command Sender";
}

CommandWindow::~CommandWindow(){
    
}

void CommandWindow::render(){
    ImGui::Begin("Commands", &enable, w_flag);
    {
        if(ImGui::BeginTabBar("Bar##Command_Win")){
            if(ImGui::BeginTabItem("Global Command##Command_Win")){
                render_global();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Camera Command##Command_Win")){
                render_local();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void CommandWindow::render_global(){
    ImGui::Text("Global Controls");
    ImGui::Text("Commands applied to all connected cameras");

    if(ImGui::Button("Scan All")) master->command_only("scan"); ImGui::SameLine();
    ImGui::SetItemTooltip("Scan all websocket server for cameras");
    if(ImGui::Button("Scan Server")) state->command_sender("scan_server");
    ImGui::SetItemTooltip("Scan one websocket server for cameras\nThis will popup a window for you to enter websocket server address to targeting");

    if(ImGui::Button("Add Camera")) state->command_sender("add_camera"); ImGui::SameLine();
    ImGui::SetItemTooltip("Menually add camera address to one websocket server list");
    if(ImGui::Button("Clean Camera")) master->command_only("clean");
    ImGui::SetItemTooltip("Clean all websocket server camera record");

    if(ImGui::Button("Connect All")) master->command_only("usb_on"); ImGui::SameLine();
    ImGui::SetItemTooltip("Tells all cameras usb control on");
    if(ImGui::Button("Disconnect All")) master->command_only("usb_off");
    ImGui::SetItemTooltip("Tells all cameras usb control off");

    if(ImGui::Button("Reboot All")) master->command_only("reboot"); ImGui::SameLine();
    ImGui::SetItemTooltip("Tells all cameras reboot right now");
    if(ImGui::Button("Shutdown All")) master->command_only("shutdown");
    ImGui::SetItemTooltip("Tells all cameras shutdown right now");

    if(ImGui::Button("Record All")) master->command_only("shutter_on"); ImGui::SameLine();
    if(ImGui::Button("Stop All")) master->command_only("shutter_off");

    if(ImGui::Button("Enter Webcam")) master->webcam_only("preview"); ImGui::SameLine();
    if(ImGui::Button("Exit Webcam")) master->webcam_only("exit");

    if(ImGui::Button("Start Webcam")) state->command_sender("start_webcam"); ImGui::SameLine();
    if(ImGui::Button("Sync Time")) master->command_only("datetime");

    if(ImGui::Button("Last Media")) master->media_only("lastmedia");
}

void CommandWindow::render_local(){
    std::lock_guard<std::mutex> lock(master.camera_mtx);
    ImGui::LabelText("Single Camera Control", "Commands applied to selected camera");

    bool should_disabled = current_camera_item.size() < 10 || master.findCamera(current_camera_item) == -1;
    ImGui::BeginDisabled(should_disabled);

    if(ImGui::Button("Record")) master.command_only("", "shutter_on", current_camera_item); ImGui::SameLine();
    if(ImGui::Button("Stop")) master.command_only("", "shutter_off", current_camera_item);

    if(ImGui::Button("Connect")) master.command_only("", "usb_on", current_camera_item); ImGui::SameLine();
    if(ImGui::Button("Disconnect")) master.command_only("", "usb_off", current_camera_item); ImGui::SameLine();
    if(ImGui::Button("Shutdown")) master.command_only("", "usb_on", current_camera_item); ImGui::SameLine();
    if(ImGui::Button("Locate")) master.command_only("", "usb_on", current_camera_item);

    if(ImGui::BeginCombo("Mode##SCC", current_mode_item_string.c_str())){
        for (int n = 0; n < GOPRO_MODE_SIZE; n++)
        {
            std::string curr = GOPRO_MODE_STRING[n];
            std::string curr_b = curr + "##SCC_Option";
            bool is_selected = (current_mode_item_string == curr); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(curr_b.c_str(), is_selected)){
                current_mode_item_string = curr;
                current_mode_item = n;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if(ImGui::Button("Setting Apply All")) {
        popup_execute_win = true;
        execution_type = ExecutionType::SetAll;
        execution_logs.clear();

        for(const auto& i : master.getCameras()){
            execution_logs.insert_or_assign(i->ip, "");
        }

        master.applyAll("", current_setting_items);
    } 
    ImGui::SameLine();
    if(ImGui::Button("Setting Apply All By ID")) {
        popup_execute_win = true;
        execution_type = ExecutionType::Set;
        execution_logs.clear();
        json buffer = json::object();
        int32_t v = current_setting_items[std::to_string(apply_all_item)].get<int32_t>();
        buffer[std::to_string(apply_all_item)] = GET_SETTING_VALUE_BY_ID(apply_all_item)[v];
        
        for(const auto& i : master.getCameras()){
            execution_logs.insert_or_assign(i->ip, "");
        }

        master.applyAll("", buffer);
    }

    if(ImGui::BeginCombo("ID", apply_all_item_string.c_str())){
        for (int n = 0; n < GOPRO_SETTING_SIZE; n++)
        {
            int32_t id = GOPRO_SETTING_IDS[n];
            std::string curr = GET_SETTING_NAME_BY_ID(id);
            bool is_selected = (current_mode_item_string == curr); // You can store your selection however you want, outside or inside your objects
            std::string curr_b = curr + "##Setting_ID_Option";
            if (ImGui::Selectable(curr_b.c_str(), is_selected)){
                apply_all_item_string = curr;
                apply_all_item = GOPRO_SETTING_IDS[n];
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::EndDisabled();
}
