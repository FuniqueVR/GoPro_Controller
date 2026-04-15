/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

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
