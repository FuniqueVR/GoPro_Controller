/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include "../windows/base_window.h"

extern "C" void init_state_setup(
    std::shared_ptr<json> servers,
    std::shared_ptr<json> gui,
    std::shared_ptr<GlobalState> global_state,
    std::shared_ptr<GoProMaster> master,
    std::shared_ptr<BaseWindow> windows[]
);
extern "C" json get_global_state_data(GlobalState& data);
extern "C" void set_global_state_data(GlobalState& data, json refs);