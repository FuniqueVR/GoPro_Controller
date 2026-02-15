/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "GoProController.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>   // For std::chrono::seconds, milliseconds, etc.
#include <thread>   // For std::this_thread::sleep_for
#include <future>
#include <unistd.h>
#include <sys/types.h>

int32_t get_timezone_offset_minutes() {
    time_t t = time(NULL);
    
    struct tm lt = *localtime(&t);
    struct tm gt = *gmtime(&t);
    
    // Convert both to time_t to get the difference
    // Note: mktime assumes 'lt' is local time
    time_t local_t = mktime(&lt);
    
    // We need a trick for GMT because mktime uses local timezone
    // We use the 'timezone' external global variable or manual math:
    struct tm* gmt_ptr = gmtime(&t);
    
    // This is the most portable manual calculation
    time_t now = time(NULL);
    struct tm *loc = localtime(&now);
    // difftime gives difference in seconds
    double diff = difftime(now, mktime(gmtime(&now)));
    
    // However, the simplest cross-platform way is:
    return (int)(-(timezone) / 60); 
    // 'timezone' is a global in <ctime> representing seconds West of UTC
}
std::string getCommand(std::string url){
    return url;
}

void GoProController::_loadRecord(){
    std::string homedir = get_env_var("WS_ROOT");
    if(homedir.size() > 0) homedir += "/";
    homedir += "record.txt";
    std::cout << "Trying load data from: " << homedir << std::endl;
    std::ifstream inFile(homedir.c_str());
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open the file: " << homedir << std::endl;
        return; // Return with an error code
    }
    std::string line;
    while (std::getline(inFile, line)) {
        std::vector<std::string> words = std::vector<std::string>();
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) { // Extracts words separated by any whitespace
            words.push_back(word);
        }
        if(words.size() > 0){
            camera_ips.push_back(words[0]);
        }
        if(words.size() > 1){
            camera_name.insert_or_assign(words[0], words[1]);
        }
    }
    inFile.close();
}

void GoProController::_updateRecord(){
    std::string homedir = get_env_var("WS_ROOT");
    if(homedir.size() > 0) homedir += "/";
    homedir += "record.txt";
    std::cout << "Trying export data to: " << homedir << std::endl;
    std::ofstream outFile( homedir.c_str() );
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open the file." << homedir << std::endl;
        return; // Return with an error code
    }
    for(auto i : camera_ips){
        if(camera_name.count(i)){
            outFile << i << " " << camera_name.at(i)  << "\n";
        }else{
            outFile << i << "\n";
        }
    }
    outFile.close();
}

void GoProController::_setAllPreset(std::vector<std::string> targets, int32_t mode){
    _getAllResponse(targets, "/gopro/camera/presets/load?id=" + std::to_string(mode));
}

void GoProController::_setPreset(std::string target, int32_t mode){
    _getSingleResponse(target, "/gopro/camera/presets/load?id=" + std::to_string(mode));
}

void GoProController::_rebootAll(std::vector<std::string> targets){
    _getAllResponse(targets, "/gp/gpControl/command/system/reset");
}

void GoProController::_reboot(std::string target){
    _getSingleResponse(target, "/gp/gpControl/command/system/reset");
}

void GoProController::_shutdownAll(std::vector<std::string> targets){
    _getAllResponse(targets, "/gp/gpControl/command/system/shutdown");
}

void GoProController::_shutdown(std::string target){
    _getSingleResponse(target, "/gp/gpControl/command/system/shutdown");
}

void GoProController::_keepAliveAll(std::vector<std::string> targets){
    _getAllResponse(targets, "/gopro/camera/keep_alive");
}

void GoProController::_keepAlive(std::string target){
    _getSingleResponse(target, "/gopro/camera/keep_alive");
}

void GoProController::_usbAll(std::vector<std::string> targets, bool ison){
    std::string url = "/gopro/camera/control/wired_usb?p=";
    if(ison) url += "1";
    else url += "0";
    _getAllResponse(targets, url);
}

void GoProController::_usb(std::string target, bool ison){
    std::string url = "/gopro/camera/control/wired_usb?p=";
    if(ison) url += "1";
    else url += "0";
    _getSingleResponse(target, url);
}

void GoProController::_datetimeAll(std::vector<std::string> targets){
    const auto now = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream date;
    std::ostringstream time;
    date << std::put_time(&tm, "%Y_%m_%d");
    time << std::put_time(&tm, "%H_%M_%S");
    int32_t minutes = get_timezone_offset_minutes();

    std::string url = "/gopro/camera/set_date_time?date=";
    url += date.str();
    url += "&time=";
    url += time.str();
    url += "&tzone=";
    url += std::to_string(minutes);
    url += "&dst=0";
    _getAllResponse(targets, url);
}

void GoProController::_datetime(std::string target){
    const auto now = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream date;
    std::ostringstream time;
    date << std::put_time(&tm, "%Y_%m_%d");
    time << std::put_time(&tm, "%H_%M_%S");
    int32_t minutes = get_timezone_offset_minutes();

    std::string url = "/gopro/camera/set_date_time?date=";
    url += date.str();
    url += "&time=";
    url += time.str();
    url += "&tzone=";
    url += std::to_string(minutes);
    url += "&dst=0";
    _getSingleResponse(target, url);
}

void GoProController::_zoomAll(std::vector<std::string> targets, int32_t value){
    std::string url = "/gopro/camera/digital_zoom?percent="
    url += std::to_string(value);
    _getAllResponse(targets, url);
}

void GoProController::_zoom(std::string target, int32_t value){
    std::string url = "/gopro/camera/digital_zoom?percent="
    url += std::to_string(value);
    _getSingleResponse(target, url);
}

void GoProController::_shutterAll(std::vector<std::string> targets, bool ison){
    std::string url = "/gopro/camera/shutter/";
    if(ison) url += "start";
    else url += "stop";
    _getAllResponse(targets, url);
}

void GoProController::_shutter(std::string target, bool ison){
    std::string url = "/gopro/camera/shutter/";
    if(ison) url += "start";
    else url += "stop";
    _getSingleResponse(target, url);
}

std::vector<std::pair<std::string, std::string>> GoProController::_queryAllStatus(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/camera/state");
}

std::pair<std::string, std::string> GoProController::_queryStatus(std::string target){
    return _getSingleResponse(target, "/gopro/camera/state");
}

std::vector<std::pair<std::string, std::string>> GoProController::_setAllSetting(std::vector<std::string> targets, int32_t ID, std::string value){
    std::string url = "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    return _getAllResponse(targets, url);
}

std::pair<std::string, std::string> GoProController::_setSetting(std::string target, int32_t ID, std::string value){
    std::string url = "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    return _getSingleResponse(target, url);
}

void GoProController::_webcamAllMode(std::vector<std::string> targets){
    _getAllResponse(targets, "/gopro/webcam/preview");
}

void GoProController::_webcamMode(std::string target){
    _getSingleResponse(target, "/gopro/webcam/preview");
}

void GoProController::_webcamAllUnMode(std::vector<std::string> targets){
    _getAllResponse(targets, "/gopro/webcam/exit");
}

void GoProController::_webcamUnMode(std::string target){
    _getSingleResponse(target, "/gopro/webcam/exit");
}

void GoProController::_webcamAllOn(std::vector<std::string> targets, int32_t startPort, int32_t res, int32_t fov, bool TS){
    std::string url = "/gopro/webcam/start?res=";
    url += std::to_string(res);
    url += "&fov=";
    url += std::to_string(fov);
    url += "&port=";
    url += std::to_string(startPort);
    if(TS){
        url += "&protocol=TS";
    }else{
        url += "&protocol=RTSP";
    }
    _getAllResponse(targets, url);
}

void GoProController::_webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS){
    std::string url = "/gopro/webcam/start?res=";
    url += std::to_string(res);
    url += "&fov=";
    url += std::to_string(fov);
    url += "&port=";
    url += std::to_string(startPort);
    if(TS){
        url += "&protocol=TS";
    }else{
        url += "&protocol=RTSP";
    }
    _getSingleResponse(target, url);
}

void GoProController::_webcamAllOff(std::vector<std::string> targets){
    _getAllResponse(targets, "/gopro/webcam/stop");
}

void GoProController::_webcamOff(std::string target){
    _getSingleResponse(target, "/gopro/webcam/stop");
}

std::pair<std::string, std::string> GoProController::_webcamStatus(std::string target){
    return _getSingleResponse(target, "/gopro/webcam/status");
}

std::pair<std::string, std::string> GoProController::_webcamVersion(std::string target){
    return _getSingleResponse(target, "/gopro/webcam/version");
}

void GoProController::_previewAllOn(std::vector<std::string> targets, int32_t port){
    _getAllResponse(targets, "/gopro/camera/stream/start?port=" + std::to_string(port));
}

void GoProController::_previewOn(std::string target, int32_t port){
    _getSingleResponse(target, "/gopro/camera/stream/start?port=" + std::to_string(port));
}

void GoProController::_previewAllOff(std::vector<std::string> targets){
    _getAllResponse(targets, "/gopro/camera/stream/stop");
}

void GoProController::_previewOff(std::string target){
    _getSingleResponse(target, "/gopro/camera/stream/stop");
}

std::vector<std::pair<std::string, std::string>> GoProController::_getAllMediaList(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/media/list");
}

std::pair<std::string, std::string> GoProController::_getMediaList(std::string target){
    return _getSingleResponse(target, "/gopro/media/list");
}

std::vector<std::pair<std::string, std::string>> GoProController::_getAllLastMedia(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/media/last_captured");
}

std::pair<std::string, std::string> GoProController::_getLastMedia(std::string target){
    return _getSingleResponse(target, "/gopro/media/last_captured");
}

std::pair<std::string, std::string> GoProController::_getSingleResponse(std::string target, std::string suffix){
    std::string url = GetRemoteURLByIP(target) + suffix;
    return std::pair<std::string, std::string>(target, exec(getCommand(url)));
}

std::vector<std::pair<std::string, std::string>> GoProController::_getAllResponse(std::vector<std::string> targets, std::string suffix){
    std::vector<std::string> urls = std::vector<std::string>();
    std::vector<std::pair<std::string, std::string>> result = std::vector<std::pair<std::string, std::string>>();
    for(int32_t i = 0; i < targets.size(); i++){
        urls.push_back(GetRemoteURLByIP(targets[i]) + suffix);
    }
    std::vector<std::string> res = execs(urls);
    std::cout << "query all: " << res.size() << "/" << targets.size() << std::endl;;
    for(int32_t i = 0; i < targets.size(); i++){
        if(res[i].size() == 0){
            res[i] = "{}";
        }
        result.push_back(std::pair<std::string, std::string>(
            targets[i],
            res[i]
        ));
    }
    return result;
}

