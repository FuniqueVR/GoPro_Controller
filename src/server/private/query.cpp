/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

std::vector<std::pair<std::string, std::string>> GoProController::_queryAllStatus(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/camera/state");
}

std::pair<std::string, std::string> GoProController::_queryStatus(std::string target){
    return _getSingleResponse(target, "/gopro/camera/state");
}

std::vector<std::pair<std::string, std::string>> GoProController::_queryAllHW(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/camera/info");
}

std::pair<std::string, std::string> GoProController::_queryHW(std::string target){
    return _getSingleResponse(target, "/gopro/camera/info");
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
