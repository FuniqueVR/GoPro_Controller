/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <future>

GoProController::GoProController() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    _loadRecord();
}

GoProController::~GoProController() {
    curl_global_cleanup();
    for(auto& thread : scan_workers){
        if (thread.joinable()) {
            thread.join();
        }
    }
}
