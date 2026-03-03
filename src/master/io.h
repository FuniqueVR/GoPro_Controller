/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void saveServerList(json data);
void saveGUI(json data);
json loadServerList();
json loadGUI();