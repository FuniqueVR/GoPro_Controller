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
#include "camera_other.h"

#define GOPRO_SETTING_SIZE 56
const static int32_t GOPRO_SETTING_IDS[] = {
    VIDEO_RESOLUTION_ID,
    FRAMES_PER_SECOND_ID,
    SHUTTER_SPEED_ID,
    ISO_MAX_ID,
    ISO_MIN_ID,
    VIDEO_LENS_ID,
    VIDEO_ASPECT_RATIO_ID,
    BIT_DEPTH_ID,
    VIDEO_BIT_RATE_ID,
    WHITE_BALANCE_ID,
    SHARPNESS_ID,
    EXPOSURE_ID,
    PHOTO_LENS_ID,
    PHOTO_OUTPUT_ID,
    VIDEO_TIMELAPSE_RATE_ID,
    PHOTO_TIMELAPSE_RATE_ID,
    NIGHTLAPSE_RATE_ID,
    WEBCAM_DIGITAL_LENSES_ID,
    AUTO_POWER_DOWN_ID,
    GPS_ID,
    LCD_BRIGHTNESS_ID,
    LED_ID,
    TIME_LAPSE_DIGITAL_LENSES_ID,
    MEDIA_FORMAT_ID,
    ANTI_FLICKER_ID,
    HYPERSMOOTH_ID,
    VIDEO_HORIZON_LEVELING_ID,
    PHOTO_HORIZON_LEVELING_ID,
    VIDEO_DURATION_ID,
    MULTISHOT_DURATION_ID,
    MAX_LENS_ID,
    HINDSIGHT_ID,
    SCHEDULED_CAPTURE_ID,
    PHOTO_SINGLE_INTERVAL_ID,
    PHOTO_SINGLE_DURATION_ID,
    VIDEO_PERFORMANCE_MODE_ID,
    CONTROL_MODE_ID,
    EASY_MODE_SPEED_ID,
    ENABLE_NIGHT_PHOTO_ID,
    WIRELESS_BAND_ID,
    STAR_TRAILS_LENGTH_ID,
    SYSTEM_VIDEO_MODE_ID,
    PROFILES_ID,
    VIDEO_EASY_MODE_ID,
    LAPSE_MODE_ID,
    MAX_LENS_MOD_ID,
    MAX_LENS_MOD_ENABLE_ID,
    EASY_NIGHT_PHOTO_ID,
    MULTI_SHOT_ASPECT_RATIO_ID,
    FRAMING_ID,
    CAMERA_MODE_ID,
    _360_PHOTO_FILES_EXTENSION_ID,
    BEEP_VOLUME_ID,
    SETUP_SCREEN_SAVER_ID,
    VIDEO_FRAMING_ID,
    AUTOMATIC_WI_FI_ACCESS_POINT_ID
};

const static int32_t GOPRO_SETTING_IDS[] = {
    VIDEO_RESOLUTION_AVA,
    FRAMES_PER_SECOND_AVA,
    SHUTTER_SPEED_AVA,
    ISO_MAX_AVA,
    ISO_MIN_AVA,
    VIDEO_LENS_AVA,
    VIDEO_ASPECT_RATIO_AVA,
    BIT_DEPTH_AVA,
    VIDEO_BIT_RATE_AVA,
    WHITE_BALANCE_AVA,
    SHARPNESS_AVA,
    EXPOSURE_AVA,
    PHOTO_LENS_AVA,
    PHOTO_OUTPUT_AVA,
    VIDEO_TIMELAPSE_RATE_AVA,
    PHOTO_TIMELAPSE_RATE_AVA,
    NIGHTLAPSE_RATE_AVA,
    WEBCAM_DIGITAL_LENSES_AVA,
    AUTO_POWER_DOWN_AVA,
    GPS_AVA,
    LCD_BRIGHTNESS_AVA,
    LED_AVA,
    TIME_LAPSE_DIGITAL_LENSES_AVA,
    MEDIA_FORMAT_AVA,
    ANTI_FLICKER_AVA,
    HYPERSMOOTH_AVA,
    VIDEO_HORIZON_LEVELING_AVA,
    PHOTO_HORIZON_LEVELING_AVA,
    VIDEO_DURATION_AVA,
    MULTISHOT_DURATION_AVA,
    MAX_LENS_AVA,
    HINDSIGHT_AVA,
    SCHEDULED_CAPTURE_AVA,
    PHOTO_SINGLE_INTERVAL_AVA,
    PHOTO_SINGLE_DURATION_AVA,
    VIDEO_PERFORMANCE_MODE_AVA,
    CONTROL_MODE_AVA,
    EASY_MODE_SPEED_AVA,
    ENABLE_NIGHT_PHOTO_AVA,
    WIRELESS_BAND_AVA,
    STAR_TRAILS_LENGTH_AVA,
    SYSTEM_VIDEO_MODE_AVA,
    PROFILES_AVA,
    VIDEO_EASY_MODE_AVA,
    LAPSE_MODE_AVA,
    MAX_LENS_MOD_AVA,
    MAX_LENS_MOD_ENABLE_AVA,
    EASY_NIGHT_PHOTO_AVA,
    MULTI_SHOT_ASPECT_RATIO_AVA,
    FRAMING_AVA,
    CAMERA_MODE_AVA,
    _360_PHOTO_FILES_EXTENSION_AVA,
    BEEP_VOLUME_AVA,
    SETUP_SCREEN_SAVER_AVA,
    VIDEO_FRAMING_AVA,
    AUTOMATIC_WI_FI_ACCESS_POINT_AVA,
};

#define GOPRO_STATUS_SIZE 76
const static int32_t GOPRO_STATUS_IDS[] = {
    BATTERY_PRESENT_ID,
    INTERNAL_BATTERY_BARS_ID,
    OVERHEATING_ID,
    BUSY_ID,
    QUICK_CAPTURE_ID,
    ENCODING_ID,
    LCD_LOCK_ID,
    VIDEO_ENCODING_DURATION_ID,
    WIRELESS_CONNECTIONS_ENABLED_ID,
    PAIRING_STATE_ID,
    LAST_PAIRING_TYPE_ID,
    LAST_PAIRING_SUCESS_ID,
    WIFI_SCAN_STATE_ID,
    LAST_WIFI_SCAN_SUCESS_ID,
    WIFI_PROVISIONING_STATE_ID,
    REMOTE_VERSION_ID,
    REMOTE_CONNECTED_ID,
    CONNECTED_WIFI_SSID_ID,
    ACCESS_POINT_SSID_ID,
    CONNECTED_DEVICE_ID,
    PREVIEW_STREAM_ID,
    PRIMARY_STORAGE_ID,
    REMAINING_PHOTOS_ID,
    REMAINING_VIDEO_TIME_ID,
    PHOTOS_ID,
    VIDEOS_ID,
    OTA_ID,
    PENDING_FW_UPDATE_CANCEL_ID,
    LOCATE_ID,
    TIMELAPSE_INTERVAL_COUNTDOWN_ID,
    SD_CARD_REMAINING_ID,
    PREVIEW_STREAM_AVAILABLE_ID,
    WIFI_BARS_ID,
    ACTIVE_HILIGHTS_ID,
    TIME_SINCE_LAST_HILIGHT_ID,
    MINIMUM_STATUS_POLL_PREIOD_ID,
    LIVEVIEW_EXPOSURE_SELECT_MODE_ID,
    LIVEVIEW_Y_ID,
    LIVEVIEW_X_ID,
    GPS_LOCK_ID,
    AP_MODE_ID,
    INTERNAL_BATTERY_PERCENTAGE_ID,
    MICROPHONE_ACCESSORY_ID,
    ZOOM_LEVEL_ID,
    STATUS_WIRELESS_BAND_ID,
    ZOOM_AVAILABLE_ID,
    MOBILE_FRIENDLY_ID,
    FTU_ID,
    _5GHZ_AVAILABLE_ID,
    READY_ID,
    OTA_CHANGED_ID,
    COLD_ID,
    ROTATION_ID,
    ZOOM_WHILE_ENCODING_ID,
    FLATMODE_ID,
    VIDEO_PRESET_ID,
    PHOTO_PRESET_ID,
    TIMELAPSE_PRESET_ID,
    PRESET_GROUP_ID,
    PRESET_ID,
    PRESET_MODIFIED_ID,
    REMAINING_LIVE_BURSTS_ID,
    LIVE_BURSTS_ID,
    CAPTURE_DELAY_ACTIVE_ID,
    MEDIA_MOD_STATE_ID,
    TIME_WARP_SPEED_ID,
    LENS_TYPE_ID,STATUS_HINDSIGHT_ID,
    SCHEDULED_CAPTURE_PRESET_ID,
    STATUS_SCHEDULED_CAPTURE_ID,
    SD_CARD_WRITE_SPEED_ERROR_ID,
    SD_CARD_ERRORS_ID,
    STATUS_CAMERA_CONTROL_ID,
    USB_CONNECTED_ID,
    USE_CONTROLLED_ID,
    SD_CARD_CAPACITY_ID
};

// Lookup functions
inline const int32_t GET_SETTING_SIZE_BY_ID(int32_t x) {
    switch(x) {
        case 2:     return VIDEO_RESOLUTION_SIZE;
        case 3:     return FRAMES_PER_SECOND_SIZE;
        case 5:     return VIDEO_TIMELAPSE_RATE_SIZE;
        case 13:    return ISO_SIZE;
        case 30:    return PHOTO_TIMELAPSE_RATE_SIZE;
        case 32:    return NIGHTLAPSE_RATE_SIZE;
        case 43:    return WEBCAM_DIGITAL_LENSES_SIZE;
        case 59:    return AUTO_POWER_DOWN_SIZE;
        case 83:    return GPS_SIZE;
        case 88:    return LCD_BRIGHTNESS_SIZE;
        case 91:    return LED_SIZE;
        case 102:   return ISO_SIZE;
        case 108:   return VIDEO_ASPECT_RATIO_SIZE;
        case 115:   return WHITE_BALANCE_SIZE;
        case 117:   return SHARPNESS_SIZE;
        case 118:   return EXPOSURE_SIZE;
        case 121:   return VIDEO_LENS_SIZE;
        case 122:   return PHOTO_LENS_SIZE;
        case 123:   return TIME_LAPSE_DIGITAL_LENSES_SIZE;
        case 125:   return PHOTO_OUTPUT_SIZE;
        case 128:   return MEDIA_FORMAT_SIZE;
        case 134:   return ANTI_FLICKER_SIZE;
        case 135:   return HYPERSMOOTH_SIZE;
        case 145:   return SHUTTER_SPEED_SIZE;
        case 150:   return VIDEO_HORIZON_LEVELING_SIZE;
        case 151:   return PHOTO_HORIZON_LEVELING_SIZE;
        case 156:   return VIDEO_DURATION_SIZE;
        case 157:   return MULTISHOT_DURATION_SIZE;
        case 162:   return MAX_LENS_SIZE;
        case 167:   return HINDSIGHT_SIZE;
        case 168:   return SCHEDULED_CAPTURE_SIZE;
        case 171:   return PHOTO_SINGLE_INTERVAL_SIZE;
        case 172:   return PHOTO_SINGLE_DURATION_SIZE;
        case 173:   return VIDEO_PERFORMANCE_MODE_SIZE;
        case 175:   return CONTROL_MODE_SIZE;
        case 176:   return EASY_MODE_SPEED_SIZE;
        case 177:   return ENABLE_NIGHT_PHOTO_SIZE;
        case 178:   return WIRELESS_BAND_SIZE;
        case 179:   return STAR_TRAILS_LENGTH_SIZE;
        case 180:   return SYSTEM_VIDEO_MODE_SIZE;
        case 182:   return VIDEO_BIT_RATE_SIZE;
        case 183:   return BIT_DEPTH_SIZE;
        case 184:   return PROFILES_SIZE;
        case 186:   return VIDEO_EASY_MODE_SIZE;
        case 187:   return LAPSE_MODE_SIZE;
        case 189:   return MAX_LENS_MOD_SIZE;
        case 190:   return MAX_LENS_MOD_ENABLE_SIZE;
        case 191:   return EASY_NIGHT_PHOTO_SIZE;
        case 192:   return MULTI_SHOT_ASPECT_RATIO_SIZE;
        case 193:   return FRAMING_SIZE;
        case 194:   return CAMERA_MODE_SIZE;
        case 196:   return _360_PHOTO_FILES_EXTENSION_SIZE;
        case 216:   return BEEP_VOLUME_SIZE;
        case 219:   return SETUP_SCREEN_SAVER_SIZE;
        case 232:   return VIDEO_FRAMING_SIZE;
        case 236:   return AUTOMATIC_WI_FI_ACCESS_POINT_SIZE;
        default: return 0;
    }
}

inline const char* GET_SETTING_NAME_BY_ID(int32_t x) {
    switch(x) {
        case 2: return VIDEO_RESOLUTION_NAME;
        case 3: return FRAMES_PER_SECOND_NAME;
        case 5: return VIDEO_TIMELAPSE_RATE_NAME;
        case 13: return ISO_MIN_NAME;
        case 30: return PHOTO_TIMELAPSE_RATE_NAME;
        case 32: return NIGHTLAPSE_RATE_NAME;
        case 43: return WEBCAM_DIGITAL_LENSES_NAME;
        case 59: return AUTO_POWER_DOWN_NAME;
        case 83: return GPS_NAME;
        case 88: return LCD_BRIGHTNESS_NAME;
        case 91: return LED_NAME;
        case 102: return ISO_MAX_NAME;
        case 108: return VIDEO_ASPECT_RATIO_NAME;
        case 115: return WHITE_BALANCE_NAME;
        case 117: return SHARPNESS_NAME;
        case 118: return EXPOSURE_NAME;
        case 121: return VIDEO_LENS_NAME;
        case 122: return PHOTO_LENS_NAME;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_NAME;
        case 125: return PHOTO_OUTPUT_NAME;
        case 128: return MEDIA_FORMAT_NAME;
        case 134: return ANTI_FLICKER_NAME;
        case 135: return HYPERSMOOTH_NAME;
        case 145: return SHUTTER_SPEED_NAME;
        case 150: return VIDEO_HORIZON_LEVELING_NAME;
        case 151: return PHOTO_HORIZON_LEVELING_NAME;
        case 156: return VIDEO_DURATION_NAME;
        case 157: return MULTISHOT_DURATION_NAME;
        case 162: return MAX_LENS_NAME;
        case 167: return HINDSIGHT_NAME;
        case 168: return SCHEDULED_CAPTURE_NAME;
        case 171: return PHOTO_SINGLE_INTERVAL_NAME;
        case 172: return PHOTO_SINGLE_DURATION_NAME;
        case 173: return VIDEO_PERFORMANCE_MODE_NAME;
        case 175: return CONTROL_MODE_NAME;
        case 176: return EASY_MODE_SPEED_NAME;
        case 177: return ENABLE_NIGHT_PHOTO_NAME;
        case 178: return WIRELESS_BAND_NAME;
        case 179: return STAR_TRAILS_LENGTH_NAME;
        case 180: return SYSTEM_VIDEO_MODE_NAME;
        case 182: return VIDEO_BIT_RATE_NAME;
        case 183: return BIT_DEPTH_NAME;
        case 184: return PROFILES_NAME;
        case 186: return VIDEO_EASY_MODE_NAME;
        case 187: return LAPSE_MODE_NAME;
        case 189: return MAX_LENS_MOD_NAME;
        case 190: return MAX_LENS_MOD_ENABLE_NAME;
        case 191: return EASY_NIGHT_PHOTO_NAME;
        case 192: return MULTI_SHOT_ASPECT_RATIO_NAME;
        case 193: return FRAMING_NAME;
        case 194: return CAMERA_MODE_NAME;
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
        case 13: return ISO_STRING;
        case 30: return PHOTO_TIMELAPSE_RATE_STRING;
        case 32: return NIGHTLAPSE_RATE_STRING;
        case 43: return WEBCAM_DIGITAL_LENSES_STRING;
        case 59: return AUTO_POWER_DOWN_STRING;
        case 83: return GPS_STRING;
        case 88: return LCD_BRIGHTNESS_STRING;
        case 91: return LED_STRING;
        case 102: return ISO_STRING;
        case 108: return VIDEO_ASPECT_RATIO_STRING;
        case 115: return WHITE_BALANCE_STRING;
        case 117: return SHARPNESS_STRING;
        case 118: return EXPOSURE_STRING;
        case 121: return VIDEO_LENS_STRING;
        case 122: return PHOTO_LENS_STRING;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_STRING;
        case 125: return PHOTO_OUTPUT_STRING;
        case 128: return MEDIA_FORMAT_STRING;
        case 134: return ANTI_FLICKER_STRING;
        case 135: return HYPERSMOOTH_STRING;
        case 145: return SHUTTER_SPEED_STRING;
        case 150: return VIDEO_HORIZON_LEVELING_STRING;
        case 151: return PHOTO_HORIZON_LEVELING_STRING;
        case 156: return VIDEO_DURATION_STRING;
        case 157: return MULTISHOT_DURATION_STRING;
        case 162: return MAX_LENS_STRING;
        case 167: return HINDSIGHT_STRING;
        case 168: return SCHEDULED_CAPTURE_STRING;
        case 171: return PHOTO_SINGLE_INTERVAL_STRING;
        case 172: return PHOTO_SINGLE_DURATION_STRING;
        case 173: return VIDEO_PERFORMANCE_MODE_STRING;
        case 175: return CONTROL_MODE_STRING;
        case 176: return EASY_MODE_SPEED_STRING;
        case 177: return ENABLE_NIGHT_PHOTO_STRING;
        case 178: return WIRELESS_BAND_STRING;
        case 179: return STAR_TRAILS_LENGTH_STRING;
        case 180: return SYSTEM_VIDEO_MODE_STRING;
        case 182: return VIDEO_BIT_RATE_STRING;
        case 183: return BIT_DEPTH_STRING;
        case 184: return PROFILES_STRING;
        case 186: return VIDEO_EASY_MODE_STRING;
        case 187: return LAPSE_MODE_STRING;
        case 189: return MAX_LENS_MOD_STRING;
        case 190: return MAX_LENS_MOD_ENABLE_STRING;
        case 191: return EASY_NIGHT_PHOTO_STRING;
        case 192: return MULTI_SHOT_ASPECT_RATIO_STRING;
        case 193: return FRAMING_STRING;
        case 194: return CAMERA_MODE_STRING;
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
        case 13: return ISO_VALUE;
        case 30: return PHOTO_TIMELAPSE_RATE_VALUE;
        case 32: return NIGHTLAPSE_RATE_VALUE;
        case 43: return WEBCAM_DIGITAL_LENSES_VALUE;
        case 59: return AUTO_POWER_DOWN_VALUE;
        case 83: return GPS_VALUE;
        case 88: return LCD_BRIGHTNESS_VALUE;
        case 91: return LED_VALUE;
        case 102: return ISO_VALUE;
        case 108: return VIDEO_ASPECT_RATIO_VALUE;
        case 115: return WHITE_BALANCE_VALUE;
        case 117: return SHARPNESS_VALUE;
        case 118: return EXPOSURE_VALUE;
        case 121: return VIDEO_LENS_VALUE;
        case 122: return PHOTO_LENS_VALUE;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_VALUE;
        case 125: return PHOTO_OUTPUT_VALUE;
        case 128: return MEDIA_FORMAT_VALUE;
        case 134: return ANTI_FLICKER_VALUE;
        case 135: return HYPERSMOOTH_VALUE;
        case 145: return SHUTTER_SPEED_VALUE;
        case 150: return VIDEO_HORIZON_LEVELING_VALUE;
        case 151: return PHOTO_HORIZON_LEVELING_VALUE;
        case 156: return VIDEO_DURATION_VALUE;
        case 157: return MULTISHOT_DURATION_VALUE;
        case 162: return MAX_LENS_VALUE;
        case 167: return HINDSIGHT_VALUE;
        case 168: return SCHEDULED_CAPTURE_VALUE;
        case 171: return PHOTO_SINGLE_INTERVAL_VALUE;
        case 172: return PHOTO_SINGLE_DURATION_VALUE;
        case 173: return VIDEO_PERFORMANCE_MODE_VALUE;
        case 175: return CONTROL_MODE_VALUE;
        case 176: return EASY_MODE_SPEED_VALUE;
        case 177: return ENABLE_NIGHT_PHOTO_VALUE;
        case 178: return WIRELESS_BAND_VALUE;
        case 179: return STAR_TRAILS_LENGTH_VALUE;
        case 180: return SYSTEM_VIDEO_MODE_VALUE;
        case 182: return VIDEO_BIT_RATE_VALUE;
        case 183: return BIT_DEPTH_VALUE;
        case 184: return PROFILES_VALUE;
        case 186: return VIDEO_EASY_MODE_VALUE;
        case 187: return LAPSE_MODE_VALUE;
        case 189: return MAX_LENS_MOD_VALUE;
        case 190: return MAX_LENS_MOD_ENABLE_VALUE;
        case 191: return EASY_NIGHT_PHOTO_VALUE;
        case 192: return MULTI_SHOT_ASPECT_RATIO_VALUE;
        case 193: return FRAMING_VALUE;
        case 194: return CAMERA_MODE_VALUE;
        case 196: return _360_PHOTO_FILES_EXTENSION_VALUE;
        case 216: return BEEP_VOLUME_VALUE;
        case 219: return SETUP_SCREEN_SAVER_VALUE;
        case 232: return VIDEO_FRAMING_VALUE;
        case 236: return AUTOMATIC_WI_FI_ACCESS_POINT_VALUE;
        default: return nullptr;
    }
}

inline const int32_t* GET_SETTING_SUPPORT_BY_ID(int32_t x) {
    switch(x) {
        case 2: return VIDEO_RESOLUTION_SUPPORT;
        case 3: return FRAMES_PER_SECOND_SUPPORT;
        case 5: return VIDEO_TIMELAPSE_RATE_SUPPORT;
        case 13: return ISO_SUPPORT;
        case 30: return PHOTO_TIMELAPSE_RATE_SUPPORT;
        case 32: return NIGHTLAPSE_RATE_SUPPORT;
        case 43: return WEBCAM_DIGITAL_LENSES_SUPPORT;
        case 59: return AUTO_POWER_DOWN_SUPPORT;
        case 83: return GPS_SUPPORT;
        case 88: return LCD_BRIGHTNESS_SUPPORT;
        case 91: return LED_SUPPORT;
        case 102: return ISO_SUPPORT;
        case 108: return VIDEO_ASPECT_RATIO_SUPPORT;
        case 115: return WHITE_BALANCE_SUPPORT;
        case 117: return SHARPNESS_SUPPORT;
        case 118: return EXPOSURE_SUPPORT;
        case 121: return VIDEO_LENS_SUPPORT;
        case 122: return PHOTO_LENS_SUPPORT;
        case 123: return TIME_LAPSE_DIGITAL_LENSES_SUPPORT;
        case 125: return PHOTO_OUTPUT_SUPPORT;
        case 128: return MEDIA_FORMAT_SUPPORT;
        case 134: return ANTI_FLICKER_SUPPORT;
        case 135: return HYPERSMOOTH_SUPPORT;
        case 145: return SHUTTER_SPEED_SUPPORT;
        case 150: return VIDEO_HORIZON_LEVELING_SUPPORT;
        case 151: return PHOTO_HORIZON_LEVELING_SUPPORT;
        case 156: return VIDEO_DURATION_SUPPORT;
        case 157: return MULTISHOT_DURATION_SUPPORT;
        case 162: return MAX_LENS_SUPPORT;
        case 167: return HINDSIGHT_SUPPORT;
        case 168: return SCHEDULED_CAPTURE_SUPPORT;
        case 171: return PHOTO_SINGLE_INTERVAL_SUPPORT;
        case 172: return PHOTO_SINGLE_DURATION_SUPPORT;
        case 173: return VIDEO_PERFORMANCE_MODE_SUPPORT;
        case 175: return CONTROL_MODE_SUPPORT;
        case 176: return EASY_MODE_SPEED_SUPPORT;
        case 177: return ENABLE_NIGHT_PHOTO_SUPPORT;
        case 178: return WIRELESS_BAND_SUPPORT;
        case 179: return STAR_TRAILS_LENGTH_SUPPORT;
        case 180: return SYSTEM_VIDEO_MODE_SUPPORT;
        case 182: return VIDEO_BIT_RATE_SUPPORT;
        case 183: return BIT_DEPTH_SUPPORT;
        case 184: return PROFILES_SUPPORT;
        case 186: return VIDEO_EASY_MODE_SUPPORT;
        case 187: return LAPSE_MODE_SUPPORT;
        case 189: return MAX_LENS_MOD_SUPPORT;
        case 190: return MAX_LENS_MOD_ENABLE_SUPPORT;
        case 191: return EASY_NIGHT_PHOTO_SUPPORT;
        case 192: return MULTI_SHOT_ASPECT_RATIO_SUPPORT;
        case 193: return FRAMING_SUPPORT;
        case 194: return CAMERA_MODE_SUPPORT;
        case 196: return _360_PHOTO_FILES_EXTENSION_SUPPORT;
        case 216: return BEEP_VOLUME_SUPPORT;
        case 219: return SETUP_SCREEN_SAVER_SUPPORT;
        case 232: return VIDEO_FRAMING_SUPPORT;
        case 236: return AUTOMATIC_WI_FI_ACCESS_POINT_SUPPORT;
        default: return nullptr;
    }
}

inline const int32_t GET_STATUS_TYPE_BY_ID(int32_t x ){
    switch(x) {
        case BATTERY_PRESENT_ID:                    return (int32_t)BATTERY_PRESENT_TYPE;
        case INTERNAL_BATTERY_BARS_ID:              return (int32_t)INTERNAL_BATTERY_BARS_TYPE;
        case OVERHEATING_ID:                        return (int32_t)OVERHEATING_TYPE;
        case BUSY_ID:                               return (int32_t)BUSY_TYPE;
        case QUICK_CAPTURE_ID:                      return (int32_t)QUICK_CAPTURE_TYPE;
        case ENCODING_ID:                           return (int32_t)ENCODING_TYPE;
        case LCD_LOCK_ID:                           return (int32_t)LCD_LOCK_TYPE;
        case VIDEO_ENCODING_DURATION_ID:            return (int32_t)VIDEO_ENCODING_DURATION_TYPE;
        case WIRELESS_CONNECTIONS_ENABLED_ID:       return (int32_t)WIRELESS_CONNECTIONS_ENABLED_TYPE;
        case PAIRING_STATE_ID:                      return (int32_t)PAIRING_STATE_TYPE;
        case LAST_PAIRING_TYPE_ID:                  return (int32_t)LAST_PAIRING_TYPE_TYPE;
        case LAST_PAIRING_SUCESS_ID:                return (int32_t)LAST_PAIRING_SUCESS_TYPE;
        case WIFI_SCAN_STATE_ID:                    return (int32_t)WIFI_SCAN_STATE_TYPE;
        case LAST_WIFI_SCAN_SUCESS_ID:              return (int32_t)LAST_WIFI_SCAN_SUCESS_TYPE;
        case WIFI_PROVISIONING_STATE_ID:            return (int32_t)WIFI_PROVISIONING_STATE_TYPE;
        case REMOTE_VERSION_ID:                     return (int32_t)REMOTE_VERSION_TYPE;
        case REMOTE_CONNECTED_ID:                   return (int32_t)REMOTE_CONNECTED_TYPE;
        case CONNECTED_WIFI_SSID_ID:                return (int32_t)CONNECTED_WIFI_SSID_TYPE;
        case ACCESS_POINT_SSID_ID:                  return (int32_t)ACCESS_POINT_SSID_TYPE;
        case CONNECTED_DEVICE_ID:                   return (int32_t)CONNECTED_DEVICE_TYPE;
        case PREVIEW_STREAM_ID:                     return (int32_t)PREVIEW_STREAM_TYPE;
        case PRIMARY_STORAGE_ID:                    return (int32_t)PRIMARY_STORAGE_TYPE;
        case REMAINING_PHOTOS_ID:                   return (int32_t)REMAINING_PHOTOS_TYPE;
        case REMAINING_VIDEO_TIME_ID:               return (int32_t)REMAINING_VIDEO_TIME_TYPE;
        case PHOTOS_ID:                             return (int32_t)PHOTOS_TYPE;
        case VIDEOS_ID:                             return (int32_t)VIDEOS_TYPE;
        case OTA_ID:                                return (int32_t)OTA_TYPE;
        case PENDING_FW_UPDATE_CANCEL_ID:           return (int32_t)PENDING_FW_UPDATE_CANCEL_TYPE;
        case LOCATE_ID:                             return (int32_t)LOCATE_TYPE;
        case TIMELAPSE_INTERVAL_COUNTDOWN_ID:       return (int32_t)TIMELAPSE_INTERVAL_COUNTDOWN_TYPE;
        case SD_CARD_REMAINING_ID:                  return (int32_t)SD_CARD_REMAINING_TYPE;
        case PREVIEW_STREAM_AVAILABLE_ID:           return (int32_t)PREVIEW_STREAM_AVAILABLE_TYPE;
        case WIFI_BARS_ID:                          return (int32_t)WIFI_BARS_TYPE;
        case ACTIVE_HILIGHTS_ID:                    return (int32_t)ACTIVE_HILIGHTS_TYPE;
        case TIME_SINCE_LAST_HILIGHT_ID:            return (int32_t)TIME_SINCE_LAST_HILIGHT_TYPE;
        case MINIMUM_STATUS_POLL_PREIOD_ID:         return (int32_t)MINIMUM_STATUS_POLL_PREIOD_TYPE;
        case LIVEVIEW_EXPOSURE_SELECT_MODE_ID:      return (int32_t)LIVEVIEW_EXPOSURE_SELECT_MODE_TYPE;
        case LIVEVIEW_Y_ID:                         return (int32_t)LIVEVIEW_Y_TYPE;
        case LIVEVIEW_X_ID:                         return (int32_t)LIVEVIEW_X_TYPE;
        case GPS_LOCK_ID:                           return (int32_t)GPS_LOCK_TYPE;
        case AP_MODE_ID:                            return (int32_t)AP_MODE_TYPE;
        case INTERNAL_BATTERY_PERCENTAGE_ID:        return (int32_t)INTERNAL_BATTERY_PERCENTAGE_TYPE;
        case MICROPHONE_ACCESSORY_ID:               return (int32_t)MICROPHONE_ACCESSORY_TYPE;
        case ZOOM_LEVEL_ID:                         return (int32_t)ZOOM_LEVEL_TYPE;
        case STATUS_WIRELESS_BAND_ID:               return (int32_t)STATUS_WIRELESS_BAND_TYPE;
        case ZOOM_AVAILABLE_ID:                     return (int32_t)ZOOM_AVAILABLE_TYPE;
        case MOBILE_FRIENDLY_ID:                    return (int32_t)MOBILE_FRIENDLY_TYPE;
        case FTU_ID:                                return (int32_t)FTU_TYPE;
        case _5GHZ_AVAILABLE_ID:                    return (int32_t)_5GHZ_AVAILABLE_TYPE;
        case READY_ID:                              return (int32_t)READY_TYPE;
        case OTA_CHANGED_ID:                        return (int32_t)OTA_CHANGED_TYPE;
        case COLD_ID:                               return (int32_t)COLD_TYPE;
        case ROTATION_ID:                           return (int32_t)ROTATION_TYPE;
        case ZOOM_WHILE_ENCODING_ID:                return (int32_t)ZOOM_WHILE_ENCODING_TYPE;
        case FLATMODE_ID:                           return (int32_t)FLATMODE_TYPE;
        case VIDEO_PRESET_ID:                       return (int32_t)VIDEO_PRESET_TYPE;
        case PHOTO_PRESET_ID:                       return (int32_t)PHOTO_PRESET_TYPE;
        case TIMELAPSE_PRESET_ID:                   return (int32_t)TIMELAPSE_PRESET_TYPE;
        case PRESET_GROUP_ID:                       return (int32_t)PRESET_GROUP_TYPE;
        case PRESET_ID:                             return (int32_t)PRESET_TYPE;
        case PRESET_MODIFIED_ID:                    return (int32_t)PRESET_MODIFIED_TYPE;
        case REMAINING_LIVE_BURSTS_ID:              return (int32_t)REMAINING_LIVE_BURSTS_TYPE;
        case LIVE_BURSTS_ID:                        return (int32_t)LIVE_BURSTS_TYPE;
        case CAPTURE_DELAY_ACTIVE_ID:               return (int32_t)CAPTURE_DELAY_ACTIVE_TYPE;
        case MEDIA_MOD_STATE_ID:                    return (int32_t)MEDIA_MOD_STATE_TYPE;
        case TIME_WARP_SPEED_ID:                    return (int32_t)TIME_WARP_SPEED_TYPE;
        case LENS_TYPE_ID:                          return (int32_t)STATUS_HINDSIGHT_TYPE;
        case SCHEDULED_CAPTURE_PRESET_ID:           return (int32_t)SCHEDULED_CAPTURE_PRESET_TYPE;
        case STATUS_SCHEDULED_CAPTURE_ID:           return (int32_t)STATUS_SCHEDULED_CAPTURE_TYPE;
        case SD_CARD_WRITE_SPEED_ERROR_ID:          return (int32_t)SD_CARD_WRITE_SPEED_ERROR_TYPE;
        case SD_CARD_ERRORS_ID:                     return (int32_t)SD_CARD_ERRORS_TYPE;
        case STATUS_CAMERA_CONTROL_ID:              return (int32_t)STATUS_CAMERA_CONTROL_TYPE;
        case USB_CONNECTED_ID:                      return (int32_t)USB_CONNECTED_TYPE;
        case USE_CONTROLLED_ID:                     return (int32_t)USE_CONTROLLED_TYPE;
        case SD_CARD_CAPACITY_ID:                   return (int32_t)SD_CARD_CAPACITY_TYPE;
        default: return 0;
    }
}

inline const char* GET_STATUS_NAME_BY_ID(int32_t x ){
    switch(x) {
        case BATTERY_PRESENT_ID:                    return BATTERY_PRESENT_NAME;
        case INTERNAL_BATTERY_BARS_ID:              return INTERNAL_BATTERY_BARS_NAME;
        case OVERHEATING_ID:                        return OVERHEATING_NAME;
        case BUSY_ID:                               return BUSY_NAME;
        case QUICK_CAPTURE_ID:                      return QUICK_CAPTURE_NAME;
        case ENCODING_ID:                           return ENCODING_NAME;
        case LCD_LOCK_ID:                           return LCD_LOCK_NAME;
        case VIDEO_ENCODING_DURATION_ID:            return VIDEO_ENCODING_DURATION_NAME;
        case WIRELESS_CONNECTIONS_ENABLED_ID:       return WIRELESS_CONNECTIONS_ENABLED_NAME;
        case PAIRING_STATE_ID:                      return PAIRING_STATE_NAME;
        case LAST_PAIRING_TYPE_ID:                  return LAST_PAIRING_TYPE_NAME;
        case LAST_PAIRING_SUCESS_ID:                return LAST_PAIRING_SUCESS_NAME;
        case WIFI_SCAN_STATE_ID:                    return WIFI_SCAN_STATE_NAME;
        case LAST_WIFI_SCAN_SUCESS_ID:              return LAST_WIFI_SCAN_SUCESS_NAME;
        case WIFI_PROVISIONING_STATE_ID:            return WIFI_PROVISIONING_STATE_NAME;
        case REMOTE_VERSION_ID:                     return REMOTE_VERSION_NAME;
        case REMOTE_CONNECTED_ID:                   return REMOTE_CONNECTED_NAME;
        case CONNECTED_WIFI_SSID_ID:                return CONNECTED_WIFI_SSID_NAME;
        case ACCESS_POINT_SSID_ID:                  return ACCESS_POINT_SSID_NAME;
        case CONNECTED_DEVICE_ID:                   return CONNECTED_DEVICE_NAME;
        case PREVIEW_STREAM_ID:                     return PREVIEW_STREAM_NAME;
        case PRIMARY_STORAGE_ID:                    return PRIMARY_STORAGE_NAME;
        case REMAINING_PHOTOS_ID:                   return REMAINING_PHOTOS_NAME;
        case REMAINING_VIDEO_TIME_ID:               return REMAINING_VIDEO_TIME_NAME;
        case PHOTOS_ID:                             return PHOTOS_NAME;
        case VIDEOS_ID:                             return VIDEOS_NAME;
        case OTA_ID:                                return OTA_NAME;
        case PENDING_FW_UPDATE_CANCEL_ID:           return PENDING_FW_UPDATE_CANCEL_NAME;
        case LOCATE_ID:                             return LOCATE_NAME;
        case TIMELAPSE_INTERVAL_COUNTDOWN_ID:       return TIMELAPSE_INTERVAL_COUNTDOWN_NAME;
        case SD_CARD_REMAINING_ID:                  return SD_CARD_REMAINING_NAME;
        case PREVIEW_STREAM_AVAILABLE_ID:           return PREVIEW_STREAM_AVAILABLE_NAME;
        case WIFI_BARS_ID:                          return WIFI_BARS_NAME;
        case ACTIVE_HILIGHTS_ID:                    return ACTIVE_HILIGHTS_NAME;
        case TIME_SINCE_LAST_HILIGHT_ID:            return TIME_SINCE_LAST_HILIGHT_NAME;
        case MINIMUM_STATUS_POLL_PREIOD_ID:         return MINIMUM_STATUS_POLL_PREIOD_NAME;
        case LIVEVIEW_EXPOSURE_SELECT_MODE_ID:      return LIVEVIEW_EXPOSURE_SELECT_MODE_NAME;
        case LIVEVIEW_Y_ID:                         return LIVEVIEW_Y_NAME;
        case LIVEVIEW_X_ID:                         return LIVEVIEW_X_NAME;
        case GPS_LOCK_ID:                           return GPS_LOCK_NAME;
        case AP_MODE_ID:                            return AP_MODE_NAME;
        case INTERNAL_BATTERY_PERCENTAGE_ID:        return INTERNAL_BATTERY_PERCENTAGE_NAME;
        case MICROPHONE_ACCESSORY_ID:               return MICROPHONE_ACCESSORY_NAME;
        case ZOOM_LEVEL_ID:                         return ZOOM_LEVEL_NAME;
        case STATUS_WIRELESS_BAND_ID:               return STATUS_WIRELESS_BAND_NAME;
        case ZOOM_AVAILABLE_ID:                     return ZOOM_AVAILABLE_NAME;
        case MOBILE_FRIENDLY_ID:                    return MOBILE_FRIENDLY_NAME;
        case FTU_ID:                                return FTU_NAME;
        case _5GHZ_AVAILABLE_ID:                    return _5GHZ_AVAILABLE_NAME;
        case READY_ID:                              return READY_NAME;
        case OTA_CHANGED_ID:                        return OTA_CHANGED_NAME;
        case COLD_ID:                               return COLD_NAME;
        case ROTATION_ID:                           return ROTATION_NAME;
        case ZOOM_WHILE_ENCODING_ID:                return ZOOM_WHILE_ENCODING_NAME;
        case FLATMODE_ID:                           return FLATMODE_NAME;
        case VIDEO_PRESET_ID:                       return VIDEO_PRESET_NAME;
        case PHOTO_PRESET_ID:                       return PHOTO_PRESET_NAME;
        case TIMELAPSE_PRESET_ID:                   return TIMELAPSE_PRESET_NAME;
        case PRESET_GROUP_ID:                       return PRESET_GROUP_NAME;
        case PRESET_ID:                             return PRESET_NAME;
        case PRESET_MODIFIED_ID:                    return PRESET_MODIFIED_NAME;
        case REMAINING_LIVE_BURSTS_ID:              return REMAINING_LIVE_BURSTS_NAME;
        case LIVE_BURSTS_ID:                        return LIVE_BURSTS_NAME;
        case CAPTURE_DELAY_ACTIVE_ID:               return CAPTURE_DELAY_ACTIVE_NAME;
        case MEDIA_MOD_STATE_ID:                    return MEDIA_MOD_STATE_NAME;
        case TIME_WARP_SPEED_ID:                    return TIME_WARP_SPEED_NAME;
        case LENS_TYPE_ID:                          return STATUS_HINDSIGHT_NAME;
        case SCHEDULED_CAPTURE_PRESET_ID:           return SCHEDULED_CAPTURE_PRESET_NAME;
        case STATUS_SCHEDULED_CAPTURE_ID:           return STATUS_SCHEDULED_CAPTURE_NAME;
        case SD_CARD_WRITE_SPEED_ERROR_ID:          return SD_CARD_WRITE_SPEED_ERROR_NAME;
        case SD_CARD_ERRORS_ID:                     return SD_CARD_ERRORS_NAME;
        case STATUS_CAMERA_CONTROL_ID:              return STATUS_CAMERA_CONTROL_NAME;
        case USB_CONNECTED_ID:                      return USB_CONNECTED_NAME;
        case USE_CONTROLLED_ID:                     return USE_CONTROLLED_NAME;
        case SD_CARD_CAPACITY_ID:                   return SD_CARD_CAPACITY_NAME;
        default: return "";
    }
}

inline const int32_t GET_STATUS_SIZE_BY_ID(int32_t x ){
    switch(x) {
        case INTERNAL_BATTERY_BARS_ID:              return INTERNAL_BATTERY_BARS_SIZE;
        case PAIRING_STATE_ID:                      return PAIRING_STATE_SIZE;
        case LAST_PAIRING_TYPE_ID:                  return LAST_PAIRING_TYPE_SIZE;
        case WIFI_SCAN_STATE_ID:                    return WIFI_SCAN_STATE_SIZE;
        case WIFI_PROVISIONING_STATE_ID:            return WIFI_PROVISIONING_STATE_SIZE;
        case PRIMARY_STORAGE_ID:                    return PRIMARY_STORAGE_SIZE;
        case OTA_ID:                                return OTA_SIZE;
        case LIVEVIEW_EXPOSURE_SELECT_MODE_ID:      return LIVEVIEW_EXPOSURE_SELECT_MODE_SIZE;
        case MICROPHONE_ACCESSORY_ID:               return MICROPHONE_ACCESSORY_SIZE;
        case STATUS_WIRELESS_BAND_ID:               return STATUS_WIRELESS_BAND_SIZE;
        case ROTATION_ID:                           return ROTATION_SIZE;
        case MEDIA_MOD_STATE_ID:                    return MEDIA_MOD_STATE_SIZE;
        case TIME_WARP_SPEED_ID:                    return TIME_WARP_SPEED_SIZE;
        case STATUS_CAMERA_CONTROL_ID:              return STATUS_CAMERA_CONTROL_SIZE;
        default: return 0;
    }
}

inline const char** GET_STATUS_STRING_BY_ID(int32_t x ){
    switch(x) {
        case INTERNAL_BATTERY_BARS_ID:              return INTERNAL_BATTERY_BARS_STRING;
        case PAIRING_STATE_ID:                      return PAIRING_STATE_STRING;
        case LAST_PAIRING_TYPE_ID:                  return LAST_PAIRING_TYPE_STRING;
        case WIFI_SCAN_STATE_ID:                    return WIFI_SCAN_STATE_STRING;
        case WIFI_PROVISIONING_STATE_ID:            return WIFI_PROVISIONING_STATE_STRING;
        case PRIMARY_STORAGE_ID:                    return PRIMARY_STORAGE_STRING;
        case OTA_ID:                                return OTA_STRING;
        case LIVEVIEW_EXPOSURE_SELECT_MODE_ID:      return LIVEVIEW_EXPOSURE_SELECT_MODE_STRING;
        case MICROPHONE_ACCESSORY_ID:               return MICROPHONE_ACCESSORY_STRING;
        case STATUS_WIRELESS_BAND_ID:               return STATUS_WIRELESS_BAND_STRING;
        case ROTATION_ID:                           return ROTATION_STRING;
        case MEDIA_MOD_STATE_ID:                    return MEDIA_MOD_STATE_STRING;
        case TIME_WARP_SPEED_ID:                    return TIME_WARP_SPEED_STRING;
        case STATUS_CAMERA_CONTROL_ID:              return STATUS_CAMERA_CONTROL_STRING;
        default: return 0;
    }
}

inline const int32_t* GET_STATUS_VALUE_BY_ID(int32_t x ){
    switch(x) {
        case INTERNAL_BATTERY_BARS_ID:              return INTERNAL_BATTERY_BARS_VALUE;
        case PAIRING_STATE_ID:                      return PAIRING_STATE_VALUE;
        case LAST_PAIRING_TYPE_ID:                  return LAST_PAIRING_TYPE_VALUE;
        case WIFI_SCAN_STATE_ID:                    return WIFI_SCAN_STATE_VALUE;
        case WIFI_PROVISIONING_STATE_ID:            return WIFI_PROVISIONING_STATE_VALUE;
        case PRIMARY_STORAGE_ID:                    return PRIMARY_STORAGE_VALUE;
        case OTA_ID:                                return OTA_VALUE;
        case LIVEVIEW_EXPOSURE_SELECT_MODE_ID:      return LIVEVIEW_EXPOSURE_SELECT_MODE_VALUE;
        case MICROPHONE_ACCESSORY_ID:               return MICROPHONE_ACCESSORY_VALUE;
        case STATUS_WIRELESS_BAND_ID:               return STATUS_WIRELESS_BAND_VALUE;
        case ROTATION_ID:                           return ROTATION_VALUE;
        case MEDIA_MOD_STATE_ID:                    return MEDIA_MOD_STATE_VALUE;
        case TIME_WARP_SPEED_ID:                    return TIME_WARP_SPEED_VALUE;
        case STATUS_CAMERA_CONTROL_ID:              return STATUS_CAMERA_CONTROL_VALUE;
        default: return 0;
    }
}


#endif
