#include "../inspector.h"


void InspectorWindow::draw_status(){
    int move_from = -1, move_to = -1;
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        float x = ImGui::GetCursorPosX();
        int32_t id = status_list_ordered[i];
        const int32_t type = GET_STATUS_TYPE_BY_ID(id);
        std::string name = GET_STATUS_NAME_BY_ID(id);
        if (name.size() == 0) continue;

        name += "##InspectorTitle_status";
        std::string pp = "##InspectorTitle_status_option_" + std::string(GET_STATUS_NAME_BY_ID(id));

        if(type == (int32_t)CAMERA_STATUS_TYPE::OPTION){
            size_t size = GET_STATUS_SIZE_BY_ID(id);
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t select_index = state->current_status_items[std::to_string(id)].get<int32_t>();
            const char** select_string_list = GET_STATUS_STRING_BY_ID(id);
            std::string select_value_display = select_string_list[select_index];
            pp = std::to_string(select_index) + " " + select_value_display + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::BOOLEAN){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t value = state->current_status_items[std::to_string(id)].get<int32_t>();
            std::string display = "";
            if(value == 1) display = "true";
            else display = "false";
            pp = display + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::INT){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t value = state->current_status_items[std::to_string(id)].get<int32_t>();
            pp = std::to_string(value) + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::LONG){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int64_t value = state->current_status_items[std::to_string(id)].get<int64_t>();
            pp = std::to_string(value) + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::STRING){
            if (!state->current_status_items[std::to_string(id)].is_string()) continue;
            std::string value = state->current_status_items[std::to_string(id)].get<std::string>();
            pp = value + pp;
        }else {
            std::cout << "Type is unknown: " << type << std::endl;
            continue;   
        }

        ImGui::Selectable(pp.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
        src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
        if(ImGui::BeginDragDropSource(src_flags)){
            if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                ImGui::Text("Moving \"%s\"", name.c_str());
            ImGui::SetDragDropPayload("INSPECTOR_STATUS", &i, sizeof(int));
            ImGui::EndDragDropSource();
        }

        if(ImGui::BeginDragDropTarget()){
            ImGuiDragDropFlags target_flags = 0;
            target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
            target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INSPECTOR_STATUS", target_flags))
            {
                move_from = *(const int*)payload->Data;
                move_to = i;
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(x + ImGui::CalcItemWidth() + ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::TextUnformatted(GET_STATUS_NAME_BY_ID(id));
    }
    if (move_from != -1 && move_to != -1)
    {
        const int32_t tmp = status_list_ordered[move_from];
        status_list_ordered[move_from] = status_list_ordered[move_to];
        status_list_ordered[move_to] = tmp;
        state->update_server();
    }
}
