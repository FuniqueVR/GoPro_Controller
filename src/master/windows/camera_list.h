/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include "base_window.h"

class CameraListWindow : public BaseWindow {
public:
    CameraListWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    virtual ~CameraListWindow();

    enum class FilterType {
        None, Connect, Server
    };

    enum class SortType {
        None, Name, IP
    };

    json get_window_data() override;
    void set_window_data(json data) override;
    virtual void render() override;

    virtual void draw_line(const std::shared_ptr<CameraInfo>& c);
    virtual void draw_group(const std::shared_ptr<CameraInfo>& c);
    virtual void item_event(const std::shared_ptr<CameraInfo>& c);
    void onClick(const std::shared_ptr<CameraInfo>& c);

private:
    ImVec2 get_rect_size();
    std::vector<std::shared_ptr<CameraInfo>> get_filtering_result();
    std::string get_filter_string(FilterType type);
    std::string get_sort_string(SortType type);

    int32_t size = 1;
    FilterType filter = FilterType::None;
    SortType sort = SortType::None;

    std::string filter_ip = "";
    bool filter_connect = false;
};