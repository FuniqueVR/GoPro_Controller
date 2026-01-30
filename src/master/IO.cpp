#include "IO.h"
#include <iostream>
#include <fstream>
#include <string>

const char* SERVER_LIST_PATH = "servers.json";
const char* GUI_PATH = "gui.json";

void saveServerList(json data){
    std::ofstream file(SERVER_LIST_PATH);
    if(file.is_open()){
        file << data.dump();
        file.close();
    }
}

void saveGUI(json data){
    std::ofstream file(GUI_PATH);
    if(file.is_open()){
        file << data.dump();
        file.close();
    }
}

json loadServerList(){
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

json loadGUI(){
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
