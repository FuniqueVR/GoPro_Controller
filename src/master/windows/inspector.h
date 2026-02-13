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

    virtual void render() override;
    virtual void draw_setting();
    virtual void draw_status();
    virtual void draw_media();
private:

};