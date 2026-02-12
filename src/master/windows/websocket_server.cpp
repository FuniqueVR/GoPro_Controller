/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "websocket_server.h"

WebsocketWindow::WebsocketWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Websocket Dashboard";
}

WebsocketWindow::~WebsocketWindow(){
    
}

void WebsocketWindow::render(){
    ImGui::Begin("Websocket Dashboard", &enable, w_flag);
    {
        ImGui::Text("Hotkeys:");
        ImGui::BulletText("F2: Start Recording");
        ImGui::BulletText("F3: Stop Recording");
        ImGui::BulletText("F4: Switch to Photo Mode");
        ImGui::BulletText("F5: Switch to Video Mode");

        ImGui::Separator();

        ImGui::Text("Manual Control:");
        if (ImGui::Button("Start Rec (F2)")) master->command_only("shutter_on"); ImGui::SameLine();
        ImGui::SetItemTooltip("Keyboard shortcut for broadcasting record signal to all connected cameras");
        if (ImGui::Button("Stop Rec (F3)")) master->command_only("shutter_off");
        ImGui::SetItemTooltip("Keyboard shortcut for broadcasting stop signal to all connected cameras");
        if (ImGui::Button("Photo Mode (F4)")) master->presetSwitch("", 65536);  ImGui::SameLine();
        ImGui::SetItemTooltip("Keyboard shortcut for broadcasting switch to photo mode signal to all connected cameras");
        if (ImGui::Button("Video Mode (F5)")) master->presetSwitch("", 0);
        ImGui::SetItemTooltip("Keyboard shortcut for broadcasting switch to video mode signal to all connected cameras");

        ImGui::Separator();

        ImGui::Text("Server Connections:");
        ImGui::InputText("Server IP", &server_ip_buf);
        ImGui::SetItemTooltip("Enter the server ip address here, example will be 127.0.0.1 or 192.168.61.123");
        if (ImGui::Button("Add Server")) {
            std::string ip = master->addServer(server_ip_buf);
            master->reconnect(ip);
            state->update_server();
        }
        ImGui::SetItemTooltip("Trying to connect server base on address you enter");

        ImGui::SameLine();
        if (ImGui::Button("Remove Server")) {
            master->clean(std::string(server_ip_buf));
            state->update_server();
        }
        ImGui::SetItemTooltip("Trying to remove server base on address you enter, You must disconnect the server before hit remove by the way.");

        ImGui::SameLine();
        if (ImGui::Button("Disconnect Server")) {
            master->disconnect(std::string(server_ip_buf));
        }
        ImGui::SetItemTooltip("Trying to disconnect server base on address you enter");

        if (ImGui::Button("Reconnect All")) {
            master->reconnectAll();
        }
        ImGui::SameLine();
        if (ImGui::Button("Disconnect All")) {
            master->disconnectAll();
        }
        ImGui::SameLine();
        if (ImGui::Button("Clean")) {
            master->cleanAll();
            state->update_server();
        }

        // Server List Table
        if (ImGui::BeginTable("Servers", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("IP Address");
            ImGui::TableSetupColumn("Status");
            ImGui::TableSetupColumn("Last Message");
            ImGui::TableHeadersRow();

            for (const auto& s : master->getServers()) {
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
    }
    ImGui::End();
}