/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

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
