/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

extern "C" inline void saveServerList(json data);
extern "C" inline void saveGUI(json data);
extern "C" inline json loadServerList();
extern "C" inline json loadGUI();
