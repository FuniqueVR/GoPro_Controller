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

#define VIDEO_RESOLUTION_SIZE 27
#define VIDEO_RESOLUTION_NAME "Video Resolution"

enum class VIDEO_RESOLUTION_ {
    V4K,
    V2_7K,
    V2_7K_4_3,
    V1440,
    V1080,
    V720,
    V4K_4_3,
    V5_6K,
    V5K,
    V5K_4_3,
    V5_3K_8_7,
    V5_3K_4_3,
    V4K_8_7,
    V8K,
    V5_3K_21_9,
    V4K_21_9,
    V4K_1_1,
    V900,
    V4K_SPH,
    V5_3K,
    V5_3K_8_7_V2,
    V4K_8_7_V2,
    V4K_9_16_V2,
    V1080_9_16_V2,
    V2_7K_4_3_V2,
    V4K_4_3_V2,
    V5_3K_4_3_V2
};

const static char* VIDEO_RESOLUTION_STRING[] = {
    "4K",
    "2.7K",
    "2.7K 4:3",
    "1440",
    "1080",
    "720",
    "4K 4:3",
    "5.6K",
    "5K",
    "5K 4:3",
    "5.3K 8:7",
    "5.3K 4:3",
    "4K 8:7",
    "8K",
    "5.3K 21:9",
    "4K 21:9",
    "4K 1:1",
    "900",
    "4K SPH",
    "5.3K",
    "5.3K 8:7 V2",
    "4K 8:7 V2",
    "4K 9:16 V2",
    "1080 9:16 V2",
    "2.7K 4:3 V2",
    "4K 4:3 V2",
    "5.3K 4:3 V2"
};

const static int32_t VIDEO_RESOLUTION_VALUE[] = {
    1, 4, 6, 7, 9, 12, 18, 21, 24, 25,
    26, 27, 28, 31, 35, 36, 37, 38, 39, 100,
    107, 108, 109, 110, 111, 112, 113
};

#define FRAMES_PER_SECOND_SIZE 13
#define FRAMES_PER_SECOND_NAME "Frames Per Second"

enum class FRAMES_PER_SECOND_ {
    V240,
    V120,
    V100,
    V90,
    V60,
    V50,
    V30,
    V25,
    V24,
    V200,
    V400,
    V360,
    V300
};

const static char* FRAMES_PER_SECOND_STRING[] = {
    "240",
    "120",
    "100",
    "90",
    "60",
    "50",
    "30",
    "25",
    "24",
    "200",
    "400",
    "360",
    "300"
};

const static int32_t FRAMES_PER_SECOND_VALUE[] = {
    0, 1, 2, 3, 5, 6, 8, 9, 10, 13,
    15, 16, 17
};

#define VIDEO_TIMELAPSE_RATE_SIZE 7
#define VIDEO_TIMELAPSE_RATE_NAME "Video Timelapse Rate"

enum class VIDEO_TIMELAPSE_RATE_ {
    V0_5X,
    V2X,
    V5X,
    V10X,
    V15X,
    V30X,
    AUTO
};

const static char* VIDEO_TIMELAPSE_RATE_STRING[] = {
    "0.5x",
    "2x",
    "5x",
    "10x",
    "15x",
    "30x",
    "Auto"
};

const static int32_t VIDEO_TIMELAPSE_RATE_VALUE[] = {
    0, 1, 2, 3, 4, 5, 6
};

#define AUTO_POWER_DOWN_SIZE 5
#define AUTO_POWER_DOWN_NAME "Auto Power Down"

enum class AUTO_POWER_DOWN_ {
    NEVER,
    V1_MIN,
    V5_MIN,
    V15_MIN,
    V30_MIN
};

const static char* AUTO_POWER_DOWN_STRING[] = {
    "Never",
    "1 Min",
    "5 Min",
    "15 Min",
    "30 Min"
};

const static int32_t AUTO_POWER_DOWN_VALUE[] = {
    0, 1, 4, 6, 7
};

#define GPS_SIZE 2
#define GPS_NAME "GPS"

enum class GPS_ {
    OFF,
    ON
};

const static char* GPS_STRING[] = {
    "Off",
    "On"
};

const static int32_t GPS_VALUE[] = {
    0, 1
};

#define LCD_BRIGHTNESS_SIZE 10
#define LCD_BRIGHTNESS_NAME "LCD Brightness"

enum class LCD_BRIGHTNESS_ {
    V10,
    V20,
    V30,
    V40,
    V50,
    V60,
    V70,
    V80,
    V90,
    V100
};

const static char* LCD_BRIGHTNESS_STRING[] = {
    "10%",
    "20%",
    "30%",
    "40%",
    "50%",
    "60%",
    "70%",
    "80%",
    "90%",
    "100%"
};

const static int32_t LCD_BRIGHTNESS_VALUE[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

#define LED_SIZE 3
#define LED_NAME "LED"

enum class LED_ {
    ALL_OFF,
    ALL_ON,
    FRONT_OFF_ONLY
};

const static char* LED_STRING[] = {
    "All Off",
    "All On",
    "Front Off Only"
};

const static int32_t LED_VALUE[] = {
    0, 1, 2
};

#define VIDEO_ASPECT_RATIO_SIZE 4
#define VIDEO_ASPECT_RATIO_NAME "Video Aspect Ratio"

enum class VIDEO_ASPECT_RATIO_ {
    V4_3,
    V16_9,
    V9_16,
    V8_7
};

const static char* VIDEO_ASPECT_RATIO_STRING[] = {
    "4:3",
    "16:9",
    "9:16",
    "8:7"
};

const static int32_t VIDEO_ASPECT_RATIO_VALUE[] = {
    0, 1, 3, 4
};

#define VIDEO_LENS_SIZE 7
#define VIDEO_LENS_NAME "Video Lens"

enum class VIDEO_LENS_ {
    WIDE,
    LINEAR,
    NARROW,
    MAX_SUPERVIEW,
    LINEAR__HORIZON_LEVELING,
    HYPERVIEW,
    LINEAR__HORIZON_LOCK
};

const static char* VIDEO_LENS_STRING[] = {
    "Wide",
    "Linear",
    "Narrow",
    "Max SuperView",
    "Linear + Horizon Leveling",
    "HyperView",
    "Linear + Horizon Lock"
};

const static int32_t VIDEO_LENS_VALUE[] = {
    0, 4, 6, 3, 8, 9, 10
};

#define PHOTO_LENS_SIZE 4
#define PHOTO_LENS_NAME "Photo Lens"

enum class PHOTO_LENS_ {
    NARROW,
    MAX_SUPERVIEW,
    WIDE,
    LINEAR
};

const static char* PHOTO_LENS_STRING[] = {
    "Narrow",
    "Max SuperView",
    "Wide",
    "Linear"
};

const static int32_t PHOTO_LENS_VALUE[] = {
    19, 100, 101, 102
};

#define TIME_LAPSE_DIGITAL_LENSES_SIZE 4
#define TIME_LAPSE_DIGITAL_LENSES_NAME "Time Lapse Digital Lenses"

enum class TIME_LAPSE_DIGITAL_LENSES_ {
    NARROW,
    MAX_SUPERVIEW,
    WIDE,
    LINEAR
};

const static char* TIME_LAPSE_DIGITAL_LENSES_STRING[] = {
    "Narrow",
    "Max SuperView",
    "Wide",
    "Linear"
};

const static int32_t TIME_LAPSE_DIGITAL_LENSES_VALUE[] = {
    19, 100, 101, 102
};

#define PHOTO_OUTPUT_SIZE 3
#define PHOTO_OUTPUT_NAME "Photo Output"

enum class PHOTO_OUTPUT_ {
    STANDARD,
    RAW,
    HDR
};

const static char* PHOTO_OUTPUT_STRING[] = {
    "Standard",
    "RAW",
    "HDR"
};

const static int32_t PHOTO_OUTPUT_VALUE[] = {
    0, 1, 2
};

#define MEDIA_FORMAT_SIZE 4
#define MEDIA_FORMAT_NAME "Media Format"

enum class MEDIA_FORMAT_ {
    TIME_LAPSE_VIDEO,
    TIME_LAPSE_PHOTO,
    NIGHT_LAPSE_PHOTO,
    NIGHT_LAPSE_VIDEO
};

const static char* MEDIA_FORMAT_STRING[] = {
    "Time Lapse Video",
    "Time Lapse Photo",
    "Night Lapse Photo",
    "Night Lapse Video"
};

const static int32_t MEDIA_FORMAT_VALUE[] = {
    13, 20, 21, 26
};

#define ANTI_FLICKER_SIZE 2
#define ANTI_FLICKER_NAME "Anti-Flicker"

enum class ANTI_FLICKER_ {
    V60HZ,
    V50HZ
};

const static char* ANTI_FLICKER_STRING[] = {
    "60Hz",
    "50Hz"
};

const static int32_t ANTI_FLICKER_VALUE[] = {
    2, 3
};

#define HYPERSMOOTH_SIZE 6
#define HYPERSMOOTH_NAME "Hypersmooth"

enum class HYPERSMOOTH_ {
    OFF,
    LOW,
    HIGH,
    BOOST,
    AUTO_BOOST,
    STANDARD
};

const static char* HYPERSMOOTH_STRING[] = {
    "Off",
    "Low",
    "High",
    "Boost",
    "Auto Boost",
    "Standard"
};

const static int32_t HYPERSMOOTH_VALUE[] = {
    0, 1, 2, 3, 4, 100
};

#define VIDEO_HORIZON_LEVELING_SIZE 3
#define VIDEO_HORIZON_LEVELING_NAME "Video Horizon Leveling"

enum class VIDEO_HORIZON_LEVELING_ {
    OFF,
    ON,
    LOCKED
};

const static char* VIDEO_HORIZON_LEVELING_STRING[] = {
    "Off",
    "On",
    "Locked"
};

const static int32_t VIDEO_HORIZON_LEVELING_VALUE[] = {
    0, 1, 2
};

#define PHOTO_HORIZON_LEVELING_SIZE 3
#define PHOTO_HORIZON_LEVELING_NAME "Photo Horizon Leveling"

enum class PHOTO_HORIZON_LEVELING_ {
    OFF,
    ON,
    LOCKED
};

const static char* PHOTO_HORIZON_LEVELING_STRING[] = {
    "Off",
    "On",
    "Locked"
};

const static int32_t PHOTO_HORIZON_LEVELING_VALUE[] = {
    0, 1, 2
};

#define VIDEO_DURATION_SIZE 4
#define VIDEO_DURATION_NAME "Video Duration"

enum class VIDEO_DURATION_ {
    NO_LIMIT,
    V15_SECONDS,
    V30_SECONDS,
    V1_HOUR
};

const static char* VIDEO_DURATION_STRING[] = {
    "No Limit",
    "15 Seconds",
    "30 Seconds",
    "1 Hour"
};

const static int32_t VIDEO_DURATION_VALUE[] = {
    0, 1, 2, 3
};

#define MAX_LENS_SIZE 2
#define MAX_LENS_NAME "Max Lens"

enum class MAX_LENS_ {
    OFF,
    ON
};

const static char* MAX_LENS_STRING[] = {
    "Off",
    "On"
};

const static int32_t MAX_LENS_VALUE[] = {
    0, 1
};

#define HINDSIGHT_SIZE 3
#define HINDSIGHT_NAME "HindSight"

enum class HINDSIGHT_ {
    OFF,
    V15_SEC,
    V30_SEC
};

const static char* HINDSIGHT_STRING[] = {
    "Off",
    "15 Sec",
    "30 Sec"
};

const static int32_t HINDSIGHT_VALUE[] = {
    0, 2, 3
};

#define SCHEDULED_CAPTURE_SIZE 2
#define SCHEDULED_CAPTURE_NAME "Scheduled Capture"

enum class SCHEDULED_CAPTURE_ {
    OFF,
    ON
};

const static char* SCHEDULED_CAPTURE_STRING[] = {
    "Off",
    "On"
};

const static int32_t SCHEDULED_CAPTURE_VALUE[] = {
    0, 1
};

#define VIDEO_PERFORMANCE_MODE_SIZE 3
#define VIDEO_PERFORMANCE_MODE_NAME "Video Performance Mode"

enum class VIDEO_PERFORMANCE_MODE_ {
    MAXIMUM_VIDEO_PERFORMANCE,
    EXTENDED_BATTERY,
    TRIPOD___STATIONARY_VIDEO
};

const static char* VIDEO_PERFORMANCE_MODE_STRING[] = {
    "Maximum Video Performance",
    "Extended Battery",
    "Tripod / Stationary Video"
};

const static int32_t VIDEO_PERFORMANCE_MODE_VALUE[] = {
    0, 1, 2
};

#define CONTROL_MODE_SIZE 2
#define CONTROL_MODE_NAME "Control Mode"

enum class CONTROL_MODE_ {
    EASY,
    PRO
};

const static char* CONTROL_MODE_STRING[] = {
    "Easy",
    "Pro"
};

const static int32_t CONTROL_MODE_VALUE[] = {
    0, 1
};

#define ENABLE_NIGHT_PHOTO_SIZE 2
#define ENABLE_NIGHT_PHOTO_NAME "Enable Night Photo"

enum class ENABLE_NIGHT_PHOTO_ {
    OFF,
    ON
};

const static char* ENABLE_NIGHT_PHOTO_STRING[] = {
    "Off",
    "On"
};

const static int32_t ENABLE_NIGHT_PHOTO_VALUE[] = {
    0, 1
};

#define WIRELESS_BAND_SIZE 2
#define WIRELESS_BAND_NAME "Wireless Band"

enum class WIRELESS_BAND_ {
    V2_4_GHZ,
    V5_GHZ
};

const static char* WIRELESS_BAND_STRING[] = {
    "2.4 GHz",
    "5 GHz"
};

const static int32_t WIRELESS_BAND_VALUE[] = {
    0, 1
};

#define STAR_TRAILS_LENGTH_SIZE 3
#define STAR_TRAILS_LENGTH_NAME "Star Trails Length"

enum class STAR_TRAILS_LENGTH_ {
    SHORT,
    LONG,
    MAX
};

const static char* STAR_TRAILS_LENGTH_STRING[] = {
    "Short",
    "Long",
    "Max"
};

const static int32_t STAR_TRAILS_LENGTH_VALUE[] = {
    0, 1, 2
};

#define SYSTEM_VIDEO_MODE_SIZE 2
#define SYSTEM_VIDEO_MODE_NAME "System Video Mode"

enum class SYSTEM_VIDEO_MODE_ {
    NTSC,
    PAL
};

const static char* SYSTEM_VIDEO_MODE_STRING[] = {
    "NTSC",
    "PAL"
};

const static int32_t SYSTEM_VIDEO_MODE_VALUE[] = {
    0, 1
};

#define VIDEO_BIT_RATE_SIZE 2
#define VIDEO_BIT_RATE_NAME "Video Bit Rate"

enum class VIDEO_BIT_RATE_ {
    STANDARD,
    HIGH
};

const static char* VIDEO_BIT_RATE_STRING[] = {
    "Standard",
    "High"
};

const static int32_t VIDEO_BIT_RATE_VALUE[] = {
    0, 1
};

#define BIT_DEPTH_SIZE 2
#define BIT_DEPTH_NAME "Bit Depth"

enum class BIT_DEPTH_ {
    V8_BIT,
    V10_BIT
};

const static char* BIT_DEPTH_STRING[] = {
    "8-bit",
    "10-bit"
};

const static int32_t BIT_DEPTH_VALUE[] = {
    0, 2
};

#define PROFILES_SIZE 3
#define PROFILES_NAME "Profiles"

enum class PROFILES_ {
    STANDARD,
    HDR,
    LOG
};

const static char* PROFILES_STRING[] = {
    "Standard",
    "HDR",
    "Log"
};

const static int32_t PROFILES_VALUE[] = {
    0, 1, 2
};

#define MAX_LENS_MOD_SIZE 2
#define MAX_LENS_MOD_NAME "Max Lens Mod"

enum class MAX_LENS_MOD_ {
    OFF,
    ON
};

const static char* MAX_LENS_MOD_STRING[] = {
    "Off",
    "On"
};

const static int32_t MAX_LENS_MOD_VALUE[] = {
    0, 1
};

#define MAX_LENS_MOD_ENABLE_SIZE 2
#define MAX_LENS_MOD_ENABLE_NAME "Max Lens Mod Enable"

enum class MAX_LENS_MOD_ENABLE_ {
    OFF,
    ON
};

const static char* MAX_LENS_MOD_ENABLE_STRING[] = {
    "Off",
    "On"
};

const static int32_t MAX_LENS_MOD_ENABLE_VALUE[] = {
    0, 1
};

#define EASY_NIGHT_PHOTO_SIZE 2
#define EASY_NIGHT_PHOTO_NAME "Easy Night Photo"

enum class EASY_NIGHT_PHOTO_ {
    OFF,
    ON
};

const static char* EASY_NIGHT_PHOTO_STRING[] = {
    "Off",
    "On"
};

const static int32_t EASY_NIGHT_PHOTO_VALUE[] = {
    0, 1
};

#define FRAMING_SIZE 3
#define FRAMING_NAME "Framing"

enum class FRAMING_ {
    WIDESCREEN,
    VERTICAL,
    FULL_FRAME
};

const static char* FRAMING_STRING[] = {
    "Widescreen",
    "Vertical",
    "Full Frame"
};

const static int32_t FRAMING_VALUE[] = {
    0, 1, 3
};

#define _360_PHOTO_FILES_EXTENSION_SIZE 2
#define _360_PHOTO_FILES_EXTENSION_NAME "360 Photo Files Extension"

enum class _360_PHOTO_FILES_EXTENSION_ {
    _360,
    _JPG
};

const static char* _360_PHOTO_FILES_EXTENSION_STRING[] = {
    ".360",
    ".jpg"
};

const static int32_t _360_PHOTO_FILES_EXTENSION_VALUE[] = {
    0, 1
};

#define BEEP_VOLUME_SIZE 4
#define BEEP_VOLUME_NAME "Beep Volume"

enum class BEEP_VOLUME_ {
    OFF,
    LOW,
    MEDIUM,
    HIGH
};

const static char* BEEP_VOLUME_STRING[] = {
    "Off",
    "Low",
    "Medium",
    "High"
};

const static int32_t BEEP_VOLUME_VALUE[] = {
    0, 1, 2, 3
};

#define SETUP_SCREEN_SAVER_SIZE 5
#define SETUP_SCREEN_SAVER_NAME "Setup Screen Saver"

enum class SETUP_SCREEN_SAVER_ {
    NEVER,
    V1_MIN,
    V2_MIN,
    V3_MIN,
    V5_MIN
};

const static char* SETUP_SCREEN_SAVER_STRING[] = {
    "Never",
    "1 Min",
    "2 Min",
    "3 Min",
    "5 Min"
};

const static int32_t SETUP_SCREEN_SAVER_VALUE[] = {
    0, 1, 2, 3, 4
};

#define VIDEO_FRAMING_SIZE 3
#define VIDEO_FRAMING_NAME "Video Framing"

enum class VIDEO_FRAMING_ {
    WIDESCREEN,
    VERTICAL,
    FULL_FRAME
};

const static char* VIDEO_FRAMING_STRING[] = {
    "Widescreen",
    "Vertical",
    "Full Frame"
};

const static int32_t VIDEO_FRAMING_VALUE[] = {
    0, 1, 3
};

#define AUTOMATIC_WI_FI_ACCESS_POINT_SIZE 2
#define AUTOMATIC_WI_FI_ACCESS_POINT_NAME "Automatic Wi-Fi Access Point"

enum class AUTOMATIC_WI_FI_ACCESS_POINT_ {
    OFF,
    ON
};

const static char* AUTOMATIC_WI_FI_ACCESS_POINT_STRING[] = {
    "Off",
    "On"
};

const static int32_t AUTOMATIC_WI_FI_ACCESS_POINT_VALUE[] = {
    0, 1
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
