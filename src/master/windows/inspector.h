/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include "base_window.h"

class InspectorWindow : public BaseWindow {
public:
    InspectorWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    virtual ~InspectorWindow();

    json get_window_data() override;
    void set_window_data(json data) override;
    virtual void render() override;

    virtual void draw_header();
    virtual void draw_system();
    virtual void draw_setting();
    virtual void draw_protune();
    virtual void draw_status();
    virtual void draw_media();
    virtual void draw_command_local();
    virtual void draw_command_global();

    void reset_setting_order();
    void reset_status_order();
protected:
    virtual void _draw_setting(std::vector<int32_t>& ordered);
    int32_t _get_current_model();
    bool conditional_filter(int32_t mymodel, int32_t setting_id, int32_t value_index);

private:
    std::vector<int32_t> system_list_ordered;
    std::vector<int32_t> video_setting_list_ordered;
    std::vector<int32_t> photo_setting_list_ordered;
    std::vector<int32_t> video_protune_list_ordered;
    std::vector<int32_t> photo_protune_list_ordered;
    std::vector<int32_t> status_list_ordered;
    bool create_date_folder;
    bool put_finish;
};