#include "../inspector.h"
#include <iostream>
#include <string>

bool InspectorWindow::conditional_filter(int32_t mymodel, int32_t setting_id){
    int32_t preset = state->try_get_status_int32_by_id(PRESET_ID);

    if(setting_id == SHUTTER_SPEED_VIDEO_ID){
        int32_t profile = state->try_get_setting_int32_by_id(PROFILES_ID);
        if(profile == 1) return false; // HDR
    }
    else if(setting_id == EXPOSURE_ID){
        int32_t shutter = state->try_get_setting_int32_by_id(SHUTTER_SPEED_VIDEO_ID);
        if(shutter != 0 && preset == 0) return false; // Auto, and video mode
    }
    return true;
}

bool InspectorWindow::conditional_filter_option(int32_t mymodel, int32_t setting_id, int32_t value_index){
    int32_t preset = state->try_get_status_int32_by_id(PRESET_ID);

    int32_t profile = state->try_get_setting_int32_by_id(PROFILES_ID);
    int32_t res = state->try_get_setting_int32_by_id(VIDEO_RESOLUTION_ID);
    int32_t aspect = state->try_get_setting_int32_by_id(VIDEO_ASPECT_RATIO_ID);
    int32_t fps = state->try_get_setting_int32_by_id(FRAMES_PER_SECOND_ID);
    int32_t antif = state->try_get_setting_int32_by_id(ANTI_FLICKER_V2_ID);
    int32_t videolen = state->try_get_setting_int32_by_id(VIDEO_LENS_ID);
    
    int32_t res_id = VIDEO_RESOLUTION_VALUE[res];
    int32_t aspect_id = VIDEO_ASPECT_RATIO_VALUE[aspect];
    int32_t fps_id = FRAMES_PER_SECOND_VALUE[fps];
    int32_t videolen_id = VIDEO_LENS_VALUE[videolen];

    if(setting_id == VIDEO_RESOLUTION_ID){
        int32_t value_id = VIDEO_RESOLUTION_VALUE[value_index];
        if(preset == 0){
            if(profile == 0){ // Standard
                if(aspect_id == 0){ // "4:3"
                    if(value_id != 6) return false;
                }
                else if(aspect_id == 1){ // "16:9"
                    if(fps_id == 0){ // 240
                        // 2.7K, 1080
                        if(value_id != 4 && value_id != 9) return false;
                    }
                    if(fps_id == 1){ // 120
                        // 1080, 4K
                        if(value_id != 9 && value_id != 1) return false;
                    }
                    if(fps_id == 5){ // 60
                        if(videolen_id == 9){ // Hyperview
                            // 4K
                            if(value_id != 1) return false;
                        }else{
                            // 1080, 4K, 5.3K
                            if(value_id != 9 && value_id != 1 && value_id != 100) return false;
                        }
                    }
                    if(fps_id == 8){ // 30
                        // 1080, 4K, 5.3K
                        if(value_id != 9 && value_id != 1 && value_id != 100) return false;
                    }
                    else if(fps_id == 10){ // 24
                        // 4K, 5.3K
                        if(value_id != 1 && value_id != 100) return false;
                    }
                }
                else if(aspect_id == 3){ // "8:7"
                    // 60
                    if(fps_id == 5){
                        // 4K 8:7
                        if(value_id != 108) return false;
                    }else{
                        // 4K 8:7, 5.3K 8:7
                        if(value_id != 107 && value_id != 108) return false;
                    }
                }
                else if(aspect_id == 4){ // "9:16"
                    // 1080 9:16, 4K 9:16
                    if(value_id != 109 && value_id != 110) return false;
                }
            }
            else if(profile == 1){ // HDR
                if(aspect_id == 3){ // "8:7"
                    // 4K 8:7
                    if(value_id != 108) return false;
                }
                else if(aspect_id == 1){ // "16:9"
                    if(fps_id == 5 || videolen_id == 9) { // 60
                        // 4K
                        if(value_id != 1) return false;
                    }else{
                        // 4K, 5.3K
                        if(value_id != 1 && value_id != 100) return false;
                    }
                }
            }
            else if(profile == 2){ // LOG
                if(aspect_id == 3){ // "8:7"
                    if(fps_id == 5){ // 60
                        // 4K 8:7
                        if(value_id != 108) return false;
                    }else{
                        // 5.3K 8:7, 4K 8:7
                        if(value_id != 107 && value_id != 108) return false;
                    }
                }
                else if(aspect_id == 1){ // "16:9"
                    if(fps_id == 1 || videolen_id == 9){ // 120
                        // 4K
                        if(value_id != 1) return false;
                    }else{
                        // 4K, 5.3K
                        if(value_id != 1 && value_id != 100) return false;
                    }
                }
            }
        }
    }
    else if(setting_id == VIDEO_ASPECT_RATIO_ID){
        int32_t value_id = VIDEO_ASPECT_RATIO_VALUE[value_index];
        if(preset == 0){ // Video
            if(profile == 0){ // Standard
                if(value_id > 4) return false;
            }
            else if(profile == 1){ // HDR
                if(value_id != 1 && value_id != 3) return false;
            }
            else if(profile == 2){ // LOG
                if(value_id != 1 && value_id != 3) return false;
            }
        }
    }
    else if(setting_id == FRAMES_PER_SECOND_ID){
        int32_t value_id = FRAMES_PER_SECOND_VALUE[value_index];
        if(preset == 0){ // Video
            if(profile == 0) { // Standard
                if(aspect_id == 0){ // "4:3"
                    // 120
                    if(value_id != 1) return false;
                }
                else if(aspect_id == 1){ // "16:9"
                    if(res_id == 1){ // 4K
                        // 30, 60, 120, 24
                        if(value_id != 8 && value_id != 5 && value_id != 1 && value_id != 10) return false;
                    }
                    else if(res_id == 100){ // 5.3K
                    }
                    else if(res_id == 4){ // 2.7K
                        // 240
                        if(value_id != 0) return false;
                    }else{ // 1080
                        // 30, 60, 120, 240
                        if(value_id != 8 && value_id != 5 && value_id != 1 && value_id != 0) return false;
                    }
                }
                else if(aspect_id == 3){ // "8:7"
                    if(res_id == 107){ // 5.3K 8:3
                        // 24, 30
                        if(value_id != 10 && value_id != 8) return false;
                    }else{ // 4K 8:7
                        // 24, 30, 60
                        if(value_id != 10 && value_id != 8 && value_id != 5) return false;
                    }
                }
                else if(aspect_id == 4){ // "9:16"
                    // 60, 30
                    if(value_id != 5 && value_id != 8) return false;
                }
            }
            else if(profile == 1) { // HDR
                if(aspect_id == 3){ // 8:7
                    // 24, 30
                    if(value_id != 10 && value_id != 8) return false;
                }else{ // 16:9
                    if(res_id == 100 || videolen_id == 9 || videolen_id == 3){ // 5.3K
                        // 24, 30
                        if(value_id != 10 && value_id != 8) return false;
                    }else{ // 4K
                        // 24, 30, 60
                        if(value_id != 10 && value_id != 8 && value_id != 5) return false;
                    }
                }
            }
            else if(profile == 2) { // LOG
                if(aspect_id == 1){ // 16:9
                    if(res_id == 100){ // 5.3K
                        if(videolen_id == 9) { // Hyperview
                            // 30, 24
                            if(value_id != 8 && value_id != 10) return false;
                        }else{
                            // 30, 60, 24
                            if(value_id != 8 && value_id != 5 && value_id != 10) return false;
                        }
                    }else{ // 4K
                        if(videolen_id == 9) { // Hyperview
                            // 30, 60, 24
                            if(value_id != 8 && value_id != 5 && value_id != 10) return false;
                        }else{
                            // 30, 60, 120, 24
                            if(value_id != 8 && value_id != 5 && value_id != 1 && value_id != 10) return false;
                        }
                    }
                }
                else if(aspect_id == 3){ // 8:7
                    // 30, 60, 24
                    if(value_id != 8 && value_id != 5 && value_id != 10) return false;                    
                }
            }
        }
    }
    else if(setting_id == VIDEO_LENS_ID){
        int32_t value_id = VIDEO_LENS_VALUE[value_index];
        if(preset == 0){ // video
            if(profile == 0){ // standard
                if(aspect_id == 1){ // 16:9
                    if(res_id == 1){ // 4K
                        if(fps_id == 1){ // 120
                            // L, W, SV
                            if(value_id != 4 && value_id != 0 && value_id != 3) return false;
                        }else{
                            // L, W, SV, HV
                            if(value_id != 4 && value_id != 0 && value_id != 3 && value_id != 9) return false;
                        }
                    }else if(res_id == 4){ // 2.7K
                        // L, W
                        if(value_id != 4 && value_id != 0) return false;
                    }else if(res_id == 9){ // 1080
                        if(fps_id == 0){ // 240
                            // L, W, SV
                            if(value_id != 4 && value_id != 0 && value_id != 3) return false;
                        }else{
                            // L, W, SV, HV
                            if(value_id != 4 && value_id != 0 && value_id != 3 && value_id != 9) return false;
                        }
                    }else if(res_id == 100){ // 5.3K
                        if(fps_id == 5){ // 60
                            // L, W, SV
                            if(value_id != 4 && value_id != 0 && value_id != 3) return false;
                        }else{
                            // L, W, SV, HV
                            if(value_id != 4 && value_id != 0 && value_id != 3 && value_id != 9) return false;
                        }
                    }
                }else if(aspect_id == 4){ // 9:16
                    
                }else if(aspect_id == 0){ // 4:3
                    
                }else if(aspect_id == 3){ // 8:7
                    
                }
            }
            else if(profile == 1){ // HDR

            }
            else if(profile == 1){ // LOG
                
            }
        }
    }
    else if(setting_id == SHUTTER_SPEED_VIDEO_ID){
        if((mymodel&(ANTI_FLICKER_V2_AVA)) > 0){
            int32_t value_id = SHUTTER_SPEED_VIDEO_VALUE[value_index];
            if(antif == 1){
                if(value_id != 0 && value_id != 51 && value_id != 50 &&
                value_id != 30 && value_id != 49 && value_id != 29 &&
                value_id != 48 && value_id != 28 && value_id != 47 &&
                value_id != 21 && value_id != 46 && value_id != 17 &&
                value_id != 45 && value_id != 12 && value_id != 44){
                    return false;
                }
            }else{
                if(value_id != 0 && value_id != 56 && value_id != 55 &&
                value_id != 31 && value_id != 54 && value_id != 24 &&
                value_id != 53 && value_id != 23 && value_id != 27 &&
                value_id != 22 && value_id != 20 && value_id != 18 &&
                value_id != 15 && value_id != 13 && value_id != 10 &&
                value_id != 8){
                    return false;
                }
            }
        }
    }
    else if(setting_id == PHOTO_LENS_ID){
        int32_t value_id = PHOTO_LENS_VALUE[value_index];
        if(value_id != 31 && value_id != 32) return false;
    }
    return true;
}
