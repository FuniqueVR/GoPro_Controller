/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

std::vector<SingleResponse> GoProController::_getAllMediaList(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/media/list");
}

SingleResponse GoProController::_getMediaList(std::string target){
    return _getSingleResponse(target, "/gopro/media/list");
}

std::vector<SingleResponse> GoProController::_getAllLastMedia(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/media/last_captured");
}

SingleResponse GoProController::_getLastMedia(std::string target){
    return _getSingleResponse(target, "/gopro/media/last_captured");
}
