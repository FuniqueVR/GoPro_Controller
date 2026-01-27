#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void saveServerList(json data);
void saveGUI(json data);
json loadServerList();
json loadGUI();