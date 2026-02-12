/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include "base_window.h"

class WebsocketWindow : public BaseWindow {
public:
    WebsocketWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    virtual ~WebsocketWindow();

    virtual void render() override;

private:
    std::string server_ip_buf = "127.0.0.1";
};