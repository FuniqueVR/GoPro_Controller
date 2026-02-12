/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char* SERVER_LIST_PATH = "servers.json";
const char* GUI_PATH = "gui.json";

extern "C" inline void saveServerList(json data){
    std::ofstream file(SERVER_LIST_PATH);
    if(file.is_open()){
        file << data.dump();
        file.close();
    }
}

extern "C" inline void saveGUI(json data){
    std::ofstream file(GUI_PATH);
    if(file.is_open()){
        file << data.dump();
        file.close();
    }
}

extern "C" inline json loadServerList() {
    std::ifstream file(SERVER_LIST_PATH);
    if(!file.is_open()){
        std::cerr << "No server config found" << std::endl;
        return json::object();
    }

    std::stringstream buffer;
    std::string fileContents;

    buffer << file.rdbuf();
    fileContents = buffer.str();

    file.close();

    return json::parse(fileContents);
}

extern "C" inline json loadGUI() {
    std::ifstream file(GUI_PATH);
    if(!file.is_open()){
        std::cerr << "No gui config found" << std::endl;
        return json::object();
    }

    std::stringstream buffer;
    std::string fileContents;

    buffer << file.rdbuf();
    fileContents = buffer.str();

    file.close();

    return json::parse(fileContents);
}