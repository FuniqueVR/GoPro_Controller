/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

void GoProController::previewOn(std::string target, int32_t port){
    if(target.size() > 0) _previewOn(target, port); 
    else _previewAllOn(camera_ips, port);
}

void GoProController::previewOff(std::string target){
    if(target.size() > 0) _previewOff(target); 
    else _previewAllOff(camera_ips);
}
