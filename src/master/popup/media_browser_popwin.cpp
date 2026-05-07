#include "media_browser_popwin.h"

MediaBrowserPopup::MediaBrowserPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Media Browser##Popup";
}


MediaBrowserPopup::~MediaBrowserPopup(){
    
}


void MediaBrowserPopup::render(){
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 display_size = io.DisplaySize;
    ImVec2 unit = ImVec2(display_size.x / 12.0f, display_size.y / 12.0f);
    float unit_width;
    float unit_height;

    ImGui::SetNextWindowPos(ImVec2(unit.x * 1.0F, unit.y * 1.0F), wp_cond);
    ImGui::SetNextWindowSize(ImVec2(unit.x * 10.0F, unit.y * 10.0F), wp_cond);
    
    unit_width = (unit.x * 10.0F) / 2.0F - style.ItemSpacing.x;
    unit_height = unit.y * 9.0F;

    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        {
            int32_t s = master->findCamera(state->current_camera_server, state->current_camera_item);
            if(s >= 0) {
                const CameraInfo ci = master->getCamera_Clone(s);
                ImGui::BeginChild("Body##Media_Browser_ChildWin_Body", ImVec2(unit.x * 10.0F - (style.ItemSpacing.x * 2), unit.y * 7.5F));
                {
                    draw_body(ci);
                }
                ImGui::EndChild();
                ImGui::BeginChild("Header##Media_Browser_ChildWin_Header", ImVec2(unit.x * 10.0F - (style.ItemSpacing.x * 2), unit.y * 1.6F));
                {
                    draw_header(ci);
                }
                ImGui::EndChild();
            }
        }    
        ImGui::EndPopup();
    }
}

void MediaBrowserPopup::draw_header(const CameraInfo& c){
    bool is_enable = c.ip.size() > 0;
    if(ImGui::Button("Cancel")){

    }
    ImGui::BeginDisabled(!is_enable);
    ImGui::SameLine();
    if(ImGui::Button("Download")){
        
    }
    ImGui::SameLine();
    if(ImGui::Button("Download To")){
        
    }
    ImGui::SameLine();
    if(ImGui::Button("Delete")){
        
    }
    ImGui::SameLine();
    if(ImGui::Button("Delete All")){
        
    }
    ImGui::EndDisabled();
}

void MediaBrowserPopup::draw_body(const CameraInfo& c){
    if(c.ip.size() == 0) return;
}

bool MediaBrowserPopup::draw_item(const MediaInfo& mi){
    return false;
}