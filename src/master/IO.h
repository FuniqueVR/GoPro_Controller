#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

extern "C" void saveServerList(json data);
extern "C" void saveGUI(json data);
extern "C" json loadServerList();
extern "C" json loadGUI();
