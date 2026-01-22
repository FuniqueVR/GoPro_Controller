#pragma once
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string GetRemoteIP(std::string serial){
    if(serial.size() != 3){
        return "";
    }

    return std::string("http://172.2") + serial[0] + std::string(".1") + serial[1] + serial[2] + std::string(".51:8080");
}

std::string exec(std::string cmd) {
    // Define a buffer size for reading output chunks
    char buffer[128];
    std::string result = "";
    // Use unique_ptr to ensure the pipe is closed automatically (C++11 or later)
    // The popen function is named _popen on Windows
    #ifdef _WIN32
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    #else
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    #endif

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read the output a chunk at a time until the end
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}