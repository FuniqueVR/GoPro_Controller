/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
 * 
 * AUTO-GENERATED FILE - DO NOT EDIT MANUALLY
 * Generated from GoPro OpenGoPro HTTP API Documentation
 * Source: gopro_settings.json
*/
#pragma once
#ifndef CAMERACODE_GENERATED_H
#define CAMERACODE_GENERATED_H
#include <cinttypes>
#include "camera_setting.h"
#include "camera_status.h"

#define GOPRO_SETTING_SIZE 54
const static int32_t GOPRO_SETTING_IDS[] = {
    2,3,5,30,32,43,59,83,88,91,
    108,121,122,123,125,128,134,135,150,151,
    156,157,162,167,168,171,172,173,175,176,
    177,178,179,180,182,183,184,186,187,189,
    190,191,192,193,194,196,216,219,223,227,
    232,233,234,236
};

#define GOPRO_STATUS_SIZE 82
const static int32_t GOPRO_STATE_IDS[] = {
    1,2,6,8,9,10,11,13,17,19,
    20,21,22,23,24,26,27,28,29,30,
    31,32,33,34,35,38,39,41,42,45,
    49,54,55,56,58,59,60,65,66,67,
    68,69,70,74,75,76,77,78,79,81,
    82,83,85,86,88,89,93,94,95,96,
    97,98,99,100,101,102,103,104,105,106,
    107,108,110,111,112,113,114,115,116,117,
    118,122
};

// Lookup functions
inline const int32_t GET_SETTING_SIZE_BY_ID(int32_t x) {
    switch(x) {
        case 2: return VIDEO_RESOLUTION_SIZE;
        case 3: return FRAMES_PER_SECOND_SIZE;
        case 5: return VIDEO_TIMELAPSE_RATE_SIZE;
        case 59: return AUTO_POWER_DOWN_SIZE;
        case 83: return GPS_SIZE;
        case 88: return LCD_BRIGHTNESS_SIZE;
        case 91: return LED_SIZE;
        case 108: return VIDEO_ASPECT_RATIO_SIZE;
        case 121: return VIDEO_LENS_SIZE;
        case 122: return PHOTO_LENS_SIZE;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_SIZE;
        case 125: return PHOTO_OUTPUT_SIZE;
        case 128: return MEDIA_FORMAT_SIZE;
        case 134: return ANTI_FLICKER_SIZE;
        case 135: return HYPERSMOOTH_SIZE;
        case 150: return VIDEO_HORIZON_LEVELING_SIZE;
        case 151: return PHOTO_HORIZON_LEVELING_SIZE;
        case 156: return VIDEO_DURATION_SIZE;
        case 162: return MAX_LENS_SIZE;
        case 167: return HINDSIGHT_SIZE;
        case 168: return SCHEDULED_CAPTURE_SIZE;
        case 173: return VIDEO_PERFORMANCE_MODE_SIZE;
        case 175: return CONTROL_MODE_SIZE;
        case 177: return ENABLE_NIGHT_PHOTO_SIZE;
        case 178: return WIRELESS_BAND_SIZE;
        case 179: return STAR_TRAILS_LENGTH_SIZE;
        case 180: return SYSTEM_VIDEO_MODE_SIZE;
        case 182: return VIDEO_BIT_RATE_SIZE;
        case 183: return BIT_DEPTH_SIZE;
        case 184: return PROFILES_SIZE;
        case 189: return MAX_LENS_MOD_SIZE;
        case 190: return MAX_LENS_MOD_ENABLE_SIZE;
        case 191: return EASY_NIGHT_PHOTO_SIZE;
        case 193: return FRAMING_SIZE;
        case 196: return _360_PHOTO_FILES_EXTENSION_SIZE;
        case 216: return BEEP_VOLUME_SIZE;
        case 219: return SETUP_SCREEN_SAVER_SIZE;
        case 232: return VIDEO_FRAMING_SIZE;
        case 236: return AUTOMATIC_WI_FI_ACCESS_POINT_SIZE;
        default: return 0;
    }
}

inline const char* GET_SETTING_NAME_BY_ID(int32_t x) {
    switch(x) {
        case 2: return VIDEO_RESOLUTION_NAME;
        case 3: return FRAMES_PER_SECOND_NAME;
        case 5: return VIDEO_TIMELAPSE_RATE_NAME;
        case 59: return AUTO_POWER_DOWN_NAME;
        case 83: return GPS_NAME;
        case 88: return LCD_BRIGHTNESS_NAME;
        case 91: return LED_NAME;
        case 108: return VIDEO_ASPECT_RATIO_NAME;
        case 121: return VIDEO_LENS_NAME;
        case 122: return PHOTO_LENS_NAME;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_NAME;
        case 125: return PHOTO_OUTPUT_NAME;
        case 128: return MEDIA_FORMAT_NAME;
        case 134: return ANTI_FLICKER_NAME;
        case 135: return HYPERSMOOTH_NAME;
        case 150: return VIDEO_HORIZON_LEVELING_NAME;
        case 151: return PHOTO_HORIZON_LEVELING_NAME;
        case 156: return VIDEO_DURATION_NAME;
        case 162: return MAX_LENS_NAME;
        case 167: return HINDSIGHT_NAME;
        case 168: return SCHEDULED_CAPTURE_NAME;
        case 173: return VIDEO_PERFORMANCE_MODE_NAME;
        case 175: return CONTROL_MODE_NAME;
        case 177: return ENABLE_NIGHT_PHOTO_NAME;
        case 178: return WIRELESS_BAND_NAME;
        case 179: return STAR_TRAILS_LENGTH_NAME;
        case 180: return SYSTEM_VIDEO_MODE_NAME;
        case 182: return VIDEO_BIT_RATE_NAME;
        case 183: return BIT_DEPTH_NAME;
        case 184: return PROFILES_NAME;
        case 189: return MAX_LENS_MOD_NAME;
        case 190: return MAX_LENS_MOD_ENABLE_NAME;
        case 191: return EASY_NIGHT_PHOTO_NAME;
        case 193: return FRAMING_NAME;
        case 196: return _360_PHOTO_FILES_EXTENSION_NAME;
        case 216: return BEEP_VOLUME_NAME;
        case 219: return SETUP_SCREEN_SAVER_NAME;
        case 232: return VIDEO_FRAMING_NAME;
        case 236: return AUTOMATIC_WI_FI_ACCESS_POINT_NAME;
        default: return "";
    }
}

inline const char** GET_SETTING_STRING_BY_ID(int32_t x) {
    switch(x) {
        case 2: return VIDEO_RESOLUTION_STRING;
        case 3: return FRAMES_PER_SECOND_STRING;
        case 5: return VIDEO_TIMELAPSE_RATE_STRING;
        case 59: return AUTO_POWER_DOWN_STRING;
        case 83: return GPS_STRING;
        case 88: return LCD_BRIGHTNESS_STRING;
        case 91: return LED_STRING;
        case 108: return VIDEO_ASPECT_RATIO_STRING;
        case 121: return VIDEO_LENS_STRING;
        case 122: return PHOTO_LENS_STRING;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_STRING;
        case 125: return PHOTO_OUTPUT_STRING;
        case 128: return MEDIA_FORMAT_STRING;
        case 134: return ANTI_FLICKER_STRING;
        case 135: return HYPERSMOOTH_STRING;
        case 150: return VIDEO_HORIZON_LEVELING_STRING;
        case 151: return PHOTO_HORIZON_LEVELING_STRING;
        case 156: return VIDEO_DURATION_STRING;
        case 162: return MAX_LENS_STRING;
        case 167: return HINDSIGHT_STRING;
        case 168: return SCHEDULED_CAPTURE_STRING;
        case 173: return VIDEO_PERFORMANCE_MODE_STRING;
        case 175: return CONTROL_MODE_STRING;
        case 177: return ENABLE_NIGHT_PHOTO_STRING;
        case 178: return WIRELESS_BAND_STRING;
        case 179: return STAR_TRAILS_LENGTH_STRING;
        case 180: return SYSTEM_VIDEO_MODE_STRING;
        case 182: return VIDEO_BIT_RATE_STRING;
        case 183: return BIT_DEPTH_STRING;
        case 184: return PROFILES_STRING;
        case 189: return MAX_LENS_MOD_STRING;
        case 190: return MAX_LENS_MOD_ENABLE_STRING;
        case 191: return EASY_NIGHT_PHOTO_STRING;
        case 193: return FRAMING_STRING;
        case 196: return _360_PHOTO_FILES_EXTENSION_STRING;
        case 216: return BEEP_VOLUME_STRING;
        case 219: return SETUP_SCREEN_SAVER_STRING;
        case 232: return VIDEO_FRAMING_STRING;
        case 236: return AUTOMATIC_WI_FI_ACCESS_POINT_STRING;
        default: return nullptr;
    }
}

inline const int32_t* GET_SETTING_VALUE_BY_ID(int32_t x) {
    switch(x) {
        case 2: return VIDEO_RESOLUTION_VALUE;
        case 3: return FRAMES_PER_SECOND_VALUE;
        case 5: return VIDEO_TIMELAPSE_RATE_VALUE;
        case 59: return AUTO_POWER_DOWN_VALUE;
        case 83: return GPS_VALUE;
        case 88: return LCD_BRIGHTNESS_VALUE;
        case 91: return LED_VALUE;
        case 108: return VIDEO_ASPECT_RATIO_VALUE;
        case 121: return VIDEO_LENS_VALUE;
        case 122: return PHOTO_LENS_VALUE;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_VALUE;
        case 125: return PHOTO_OUTPUT_VALUE;
        case 128: return MEDIA_FORMAT_VALUE;
        case 134: return ANTI_FLICKER_VALUE;
        case 135: return HYPERSMOOTH_VALUE;
        case 150: return VIDEO_HORIZON_LEVELING_VALUE;
        case 151: return PHOTO_HORIZON_LEVELING_VALUE;
        case 156: return VIDEO_DURATION_VALUE;
        case 162: return MAX_LENS_VALUE;
        case 167: return HINDSIGHT_VALUE;
        case 168: return SCHEDULED_CAPTURE_VALUE;
        case 173: return VIDEO_PERFORMANCE_MODE_VALUE;
        case 175: return CONTROL_MODE_VALUE;
        case 177: return ENABLE_NIGHT_PHOTO_VALUE;
        case 178: return WIRELESS_BAND_VALUE;
        case 179: return STAR_TRAILS_LENGTH_VALUE;
        case 180: return SYSTEM_VIDEO_MODE_VALUE;
        case 182: return VIDEO_BIT_RATE_VALUE;
        case 183: return BIT_DEPTH_VALUE;
        case 184: return PROFILES_VALUE;
        case 189: return MAX_LENS_MOD_VALUE;
        case 190: return MAX_LENS_MOD_ENABLE_VALUE;
        case 191: return EASY_NIGHT_PHOTO_VALUE;
        case 193: return FRAMING_VALUE;
        case 196: return _360_PHOTO_FILES_EXTENSION_VALUE;
        case 216: return BEEP_VOLUME_VALUE;
        case 219: return SETUP_SCREEN_SAVER_VALUE;
        case 232: return VIDEO_FRAMING_VALUE;
        case 236: return AUTOMATIC_WI_FI_ACCESS_POINT_VALUE;
        default: return nullptr;
    }
}

#endif
