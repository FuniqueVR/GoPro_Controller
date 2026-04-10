#include "../inspector.h"
#include <cstdlib>      // For std::getenv
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

std::filesystem::path get_home_directory() {
    const char* home_dir = nullptr;

#if defined(_WIN32) || defined(_WIN64)
    // Windows: Check USERPROFILE or HOMEDRIVE/HOMEPATH
    home_dir = std::getenv("USERPROFILE");
    if (!home_dir) {
        const char* drive = std::getenv("HOMEDRIVE");
        const char* path = std::getenv("HOMEPATH");
        if (drive && path) {
            return std::filesystem::path(drive) / path;
        }
    }
#else
    // Unix-like systems (Linux, macOS, etc.): Check HOME
    home_dir = std::getenv("HOME");
#endif

    if (home_dir) {
        return std::filesystem::path(home_dir);
    } else {
        // Fallback or throw an error if the home directory cannot be found
        throw std::runtime_error("Could not find home directory");
    }
}

void InspectorWindow::draw_media(){
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 size = ImGui::GetWindowSize();
    ImVec2 button_size = ImVec2(size.x / 2.0F - style.ItemSpacing.x, 0);

    int32_t camera_ip = master->findCamera(state->current_camera_item);
    if(ImGui::InputText("Media Download", &state->current_download_location)){
        state->update_server();
    }
    if(ImGui::Button("All Download", button_size)){
        std::string buffer = state->current_download_location;
        while(buffer.size() > 0 && buffer.at(buffer.size() - 1) == '/'){
            buffer.pop_back();
        }
        if(master->directoryExists(buffer)){
            if(create_date_folder){
                std::string date = getCurrentDateTimeString();
                buffer.append("/" + date);
            }
            fs::create_directories(buffer);
            master->download_last_media(buffer, put_finish);
        }
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Download all exist camera instances");
    ImGui::SameLine();
    if(ImGui::Button("Single Download", button_size)){
        
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Download current select camera instance");

    if(ImGui::Button("Open Home", button_size)){
        std::system((std::string("open \"") + get_home_directory().string() + std::string("\"")).c_str());
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Open file explorer for home directory");
    ImGui::SameLine();
    if(ImGui::Button("Open Select Path", button_size)){
        if(fs::exists(state->current_download_location)){
            std::system((std::string("open \"") + state->current_download_location + std::string("\"")).c_str());
        }
    }

    if(ImGui::Checkbox("Create Date Folder", &create_date_folder)){
        state->update_server();
    }
    if(ImGui::Checkbox("Added Finish File", &put_finish)){
        state->update_server();
    }

    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Open file explorer for path select directory");
    if(camera_ip >= 0){
        std::shared_ptr<CameraInfo> t = master->getCameras()[camera_ip];
        ImGui::LabelText("Server", "%s", t->server.c_str());
        ImGui::LabelText("Last Media", "%s", t->last_media.c_str());
    }
}
