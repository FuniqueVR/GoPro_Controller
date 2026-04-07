#pragma once
#ifndef CAMERA_SETTING
#define CAMERA_SETTING
#include <cinttypes>
#include <array>
#include "camera_model.h"

#pragma region Resolution
#define VIDEO_RESOLUTION_ID 2
#define VIDEO_RESOLUTION_SIZE 27
#define VIDEO_RESOLUTION_NAME "Video Resolution"
#define VIDEO_RESOLUTION_AVA MODEL_MAX2_ALL
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
    1, // 4K
    4, // 2.7K
    6, // 2.7K 4:3
    7, // 1440
    9, // 1080
    12, // 720
    18, // 4K 4:3
    21, // 5.6K
    24, // 5K
    25,// 5K 4:3
    26, // 5.3K 8:7
    27, // 5.3K 4:3
    28, // 4K 8:7
    31, // 8K
    35, // 5.3K 21:9
    36, // 4K 21:9
    37, // 4K 1:1
    38, // 900
    39, // 4K SPH
    100,// 5.3K
    107, // 5.3K 8:7 V2
    108, // 4K 8:7 V2
    109, // 4K 9:16 V2
    110, // 1080 9:16 V2
    111, // 2.7K 4:3 V2
    112, // 4K 4:3 V2
    113// 5.3K 4:3 V
};
const static int32_t VIDEO_RESOLUTION_SUPPORT[] = {
    MODEL_MAX2_ALL, // 4K
    MODEL_13_ALL, // 2.7K
    MODEL_11_ALL, // 2.7K 4:3
    MODEL_9, // 1440
    MODEL_MAX2_ALL, // 1080
    MODEL_13, // 720
    MODEL_12_ALL, // 4K 4:3
    MODEL_MAX2, // 5.6K
    MODEL_9, // 5K
    MODEL_10, // 5K 4:3
    MODEL_11S, // 5.3K 8:7
    MODEL_11S, // 5.3K 4:3
    MODEL_11S, // 4K 8:7
    MODEL_MAX2, // 8K
    MODEL_13, // 5.3K 21:9
    MODEL_13, // 4K 21:9
    MODEL_13, // 4K 1:1
    MODEL_13, // 900
    MODEL_MAX2, // 4K SPH
    MODEL_13_ALL, // 5.3K
    MODEL_13|MODEL_12, // 5.3K 8:7 V2
    MODEL_13|MODEL_12, // 4K 8:7 V2
    MODEL_MAX2|MODEL_13|MODEL_12, // 4K 9:16 V2
    MODEL_MAX2|MODEL_13|MODEL_12, // 1080 9:16 V2
    MODEL_13|MODEL_12, // 2.7K 4:3 V2
    MODEL_13|MODEL_12, // 4K 4:3 V2
    MODEL_13 // 5.3K 4:3 V
}
const static int32_t VIDEO_RESOLUTION_RES[][2] = {
    {3840, 2160},
    {2704, 1520},
    {2704, 2028},
    {1920, 1440},
    {1920, 1080},
    {1280, 720}, 
    {3840, 2880},
    {5568, 3128},
    {5120, 2880},
    {5120, 3840},
    {5312, 4648},
    {5312, 3984},
    {4000, 3500},
    {7680, 4320},
    {5312, 2272},
    {3840, 1645},
    {3840, 3840},
    {1600, 900}, 
    {3840, 1920},
    {5312, 2988},
    {5312, 4648},
    {4000, 3500},
    {2160, 3840},
    {1080, 1920},
    {2704, 2028},
    {3840, 2880},
    {5312, 3984} 
};
#pragma endregion

#pragma region FPS
#define FRAMES_PER_SECOND_ID 3
#define FRAMES_PER_SECOND_SIZE 13
#define FRAMES_PER_SECOND_NAME "Frames Per Second"
#define FRAMES_PER_SECOND_AVA MODEL_MAX2_ALL
const static char* FRAMES_PER_SECOND_STRING[] = {
    "240.0",
    "120.0",
    "100.0",
    "90.0",
    "60.0",
    "50.0",
    "30.0",
    "25.0",
    "24.0",
    "200.0",
    "400.0",
    "360.0",
    "300.0",
};
const static int32_t FRAMES_PER_SECOND_VALUE[] = {
    0, // 240.0
    1, // 120.0
    2, // 100.0
    3, // 90.0
    5, // 60.0
    6, // 50.0
    8, // 30.0
    9, // 25.0
    10, // 24.0
    13, // 200.0
    15, // 400.0
    16, // 360.0
    17, // 300.0
};
const static int32_t FRAMES_PER_SECOND_SUPPORT[] = {
    MODEL_13_ALL, // 240.0
    MODEL_13_ALL, // 120.0
    MODEL_MAX2_ALL, // 100.0
    MODEL_MAX2, // 90.0
    MODEL_MAX2_ALL, // 60.0
    MODEL_MAX2_ALL, // 50.0
    MODEL_MAX2_ALL, // 30.0
    MODEL_MAX2_ALL, // 25.0
    MODEL_MAX2_ALL, // 24.0
    MODEL_13_ALL, // 200.0
    MODEL_13, // 400.0
    MODEL_13, // 360.0
    MODEL_13, // 300.0
};
#pragma endregion

#pragma region Video Time Lapse
#define VIDEO_TIMELAPSE_RATE_ID 5
#define VIDEO_TIMELAPSE_RATE_SIZE 12
#define VIDEO_TIMELAPSE_RATE_NAME "Video Timelapse Rate"
#define VIDEO_TIMELAPSE_RATE_AVA MODEL_MAX2_ALL
const static char* VIDEO_TIMELAPSE_RATE_STRING[] = {
    "0.5 Sec",
    "1 Sec",
    "2 Sec",
    "5 Sec",
    "10 Sec",
    "30 Sec",
    "60 Sec",
    "2 Min",
    "5 Min",
    "30 Min",
    "60 Min",
    "30 Sec",
};
const static int32_t VIDEO_TIMELAPSE_RATE_VALUE[] = {
    0, // 0.5 Sec
    1, // 1 Sec
    2, // 2 Sec
    3, // 5 Sec
    4, // 10 Sec
    5, // 30 Sec
    6, // 60 Sec
    7, // 2 Min
    8, // 5 Min
    9, // 30 Min
    10, // 60 Min
    11, // 30 Sec
};
const static int32_t VIDEO_TIMELAPSE_RATE_SUPPORT[] = {
    MODEL_MAX2_ALL, // 0.5 Sec
    MODEL_MAX2_ALL, // 1 Sec
    MODEL_MAX2_ALL, // 2 Sec
    MODEL_MAX2_ALL, // 5 Sec
    MODEL_MAX2_ALL, // 10 Sec
    MODEL_MAX2_ALL, // 30 Sec
    MODEL_MAX2_ALL, // 60 Sec
    MODEL_MAX2_ALL, // 2 Min
    MODEL_MAX2_ALL, // 5 Min
    MODEL_MAX2_ALL, // 30 Min
    MODEL_MAX2_ALL, // 60 Min
    MODEL_MAX2|MODEL_13|MODEL_12, // 30 Sec
};
#pragma endregion

#pragma region ISO Min 1
#define ISO_MIN_1_ID 102
#define ISO_MIN_1_SIZE 8
#define ISO_MIN_1_NAME "ISO Minimum"
#define ISO_MIN_1_AVA MODEL_MAX2_ALL
const static char* ISO_MIN_1_STRING[] = {
    "Auto",
    "6400",
    "3200",
    "1600",
    "800",
    "400",
    "200",
    "100",
};
const static int32_t ISO_VALUE[] = {
    9, 0, 3, 1, 4, 2, 7, 8
};
#pragma endregion

#pragma region ISO Max 1
#define ISO_MAX_1_ID 13
#pragma endregion

#pragma region ISO Min 2
#define ISO_MIN_2_ID 75
#pragma endregion

#pragma region ISO Max 2
#define ISO_MAX_2_ID 37
#pragma endregion

#pragma region ISO Min 3
#define ISO_MIN_3_ID 76
#pragma endregion

#pragma region ISO Max 3
#define ISO_MAX_3_ID 24
#pragma endregion

#pragma region Photo Time Lapse
#define PHOTO_TIMELAPSE_RATE_ID 30
#define PHOTO_TIMELAPSE_RATE_SIZE 12
#define PHOTO_TIMELAPSE_RATE_NAME "Photo Timelapse Rate"
#define PHOTO_TIMELAPSE_RATE_AVA MODEL_MAX2_ALL&(~MODEL_11)
const static char* PHOTO_TIMELAPSE_RATE_STRING[] = {
    "3 Sec",
    "60 Min",
    "30 Min",
    "5 Min",
    "2 Min",
    "60 Sec",
    "30 Sec",
    "10 Sec",
    "5 Sec",
    "2 Sec",
    "1 Sec",
    "0.5 Sec",
};
const static int32_t PHOTO_TIMELAPSE_RATE_VALUE[] = {
    11, // 3 Sec
    100, // 60 Min
    101, // 30 Min
    102, // 5 Min
    103, // 2 Min
    104, // 60 Sec
    105, // 30 Sec
    106, // 10 Sec
    107, // 5 Sec
    108, // 2 Sec
    109, // 1 Sec
    110, // 0.5 Sec
};
const static int32_t PHOTO_TIMELAPSE_RATE_SUPPORT[] = {
    MODEL_MAX2|MODEL_13|MODEL_12, // 3 Sec
    MODEL_MAX2_ALL&(~MODEL_11), // 60 Min
    MODEL_MAX2_ALL&(~MODEL_11), // 30 Min
    MODEL_MAX2_ALL&(~MODEL_11), // 5 Min
    MODEL_MAX2_ALL&(~MODEL_11), // 2 Min
    MODEL_MAX2_ALL&(~MODEL_11), // 60 Sec
    MODEL_MAX2_ALL&(~MODEL_11), // 30 Sec
    MODEL_MAX2_ALL&(~MODEL_11), // 10 Sec
    MODEL_MAX2_ALL&(~MODEL_11), // 5 Sec
    MODEL_MAX2_ALL&(~MODEL_11), // 2 Sec
    MODEL_13_ALL&(~MODEL_11), // 1 Sec
    MODEL_13_ALL&(~MODEL_11), // 0.5 Sec
};
#pragma endregion

#pragma region Night Lapse Rate
#define NIGHTLAPSE_RATE_ID 32
#define NIGHTLAPSE_RATE_SIZE 12
#define NIGHTLAPSE_RATE_NAME "Nightlapse Rate"
#define NIGHTLAPSE_RATE_AVA MODEL_MAX2_ALL
const static char* NIGHTLAPSE_RATE_STRING[] = {
    "4 Sec",
    "5 Sec",
    "10 Sec",
    "15 Sec",
    "20 Sec",
    "30 Sec",
    "60 Sec",
    "2 Min",
    "5 Min",
    "30 Min",
    "60 Min",
    "Auto",
};
const static int32_t NIGHTLAPSE_RATE_VALUE[] = {
    4, // 4 Sec
    5, // 5 Sec
    10, // 10 Sec
    15, // 15 Sec
    20, // 20 Sec
    30, // 30 Sec
    100, // 60 Sec
    120, // 2 Min
    300, // 5 Min
    1800, // 30 Min
    3600, // 60 Min
    3601, // Auto
};
const static int32_t NIGHTLAPSE_RATE_SUPPORT[] = {
    MODEL_MAX2_ALL, // 4 Sec
    MODEL_MAX2_ALL, // 5 Sec
    MODEL_MAX2_ALL, // 10 Sec
    MODEL_MAX2_ALL, // 15 Sec
    MODEL_MAX2_ALL, // 20 Sec
    MODEL_MAX2_ALL, // 30 Sec
    MODEL_MAX2_ALL, // 60 Sec
    MODEL_MAX2_ALL, // 2 Min
    MODEL_MAX2_ALL, // 5 Min
    MODEL_MAX2_ALL, // 30 Min
    MODEL_MAX2_ALL, // 60 Min
    MODEL_MAX2_ALL, // Auto
};
#pragma endregion

#pragma region Webcam Digital Lenses
#define WEBCAM_DIGITAL_LENSES_ID 43
#define WEBCAM_DIGITAL_LENSES_SIZE 4
#define WEBCAM_DIGITAL_LENSES_NAME "Webcam Digital Lenses"
#define WEBCAM_DIGITAL_LENSES_AVA MODEL_MAX2_ALL
const static char* WEBCAM_DIGITAL_LENSES_STRING[] = {
    "Wide",
    "Narrow",
    "Superview",
    "Linear",
};
const static int32_t WEBCAM_DIGITAL_LENSES_VALUE[] = {
    0, // Wide
    2, // Narrow
    3, // Superview
    4, // Linear
};
const static int32_t WEBCAM_DIGITAL_LENSES_SUPPORT[] = {
    MODEL_MAX2_ALL, // Wide
    MODEL_MAX2_ALL, // Narrow
    MODEL_MAX2_ALL, // Superview
    MODEL_MAX2_ALL, // Linear
};
#pragma endregion

#pragma region Auto Power Down
#define AUTO_POWER_DOWN_ID 59
#define AUTO_POWER_DOWN_SIZE 7
#define AUTO_POWER_DOWN_NAME "Auto Power Down"
#define AUTO_POWER_DOWN_AVA MODEL_MAX2_ALL
const static char* AUTO_POWER_DOWN_STRING[] = {
    "Never",
    "1 Min",
    "5 Min",
    "15 Min",
    "30 Min",
    "8 Sec",
    "30 Sec",
};
const static int32_t AUTO_POWER_DOWN_VALUE[] = {
    MODEL_MAX2_ALL, // Never
    MODEL_MAX2_ALL&(~(MODEL_10|MODEL_9)), // 1 Min
    MODEL_MAX2_ALL, // 5 Min
    6, // 15 Min
    7, // 30 Min
    11, // 8 Sec
    12, // 30 Sec
};
const static int32_t AUTO_POWER_DOWN_SUPPORT[] = {
    0, // Never
    1, // 1 Min
    4, // 5 Min
    6, // 15 Min
    7, // 30 Min
    11, // 8 Sec
    12, // 30 Sec
};
#pragma endregion

#pragma region GPS
#define GPS_ID 83
#define GPS_SIZE 2
#define GPS_NAME "GPS"
#define GPS_AVA MODEL_MAX2_ALL&(~(MODEL_12|MODEL_11))
const static char* GPS_STRING[] = {
    "Off",
    "On"
};
const static int32_t GPS_VALUE[] = {
    0, // Off 
    1, // On
};
const static int32_t GPS_SUPPORT[] = {
    MODEL_MAX2_ALL&(~(MODEL_12|MODEL_11)), // Off
    MODEL_MAX2_ALL&(~(MODEL_12|MODEL_11)), // On
};
#pragma endregion

#pragma region LCE Brightness
#define LCD_BRIGHTNESS_ID 88
#define LCD_BRIGHTNESS_SIZE 10
#define LCD_BRIGHTNESS_NAME "LCD Brightness"
#define LCD_BRIGHTNESS_AVA MODEL_MAX2_ALL&(~MODEL_11)
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
    "100%",
};
const static int32_t LCD_BRIGHTNESS_VALUE[] = {
    10, // 10%
    20, // 20%
    30, // 30%
    40, // 40%
    50, // 50%
    60, // 60%
    70, // 70%
    80, // 80%
    90, // 90%
    100, // 100%
};
const static int32_t LCD_BRIGHTNESS_SUPPORT[] = {
    MODEL_MAX2_ALL&(~MODEL_11), // 10%
    MODEL_MAX2_ALL&(~MODEL_11), // 20%
    MODEL_MAX2_ALL&(~MODEL_11), // 30%
    MODEL_MAX2_ALL&(~MODEL_11), // 40%
    MODEL_MAX2_ALL&(~MODEL_11), // 50%
    MODEL_MAX2_ALL&(~MODEL_11), // 60%
    MODEL_MAX2_ALL&(~MODEL_11), // 70%
    MODEL_MAX2_ALL&(~MODEL_11), // 80%
    MODEL_MAX2_ALL&(~MODEL_11), // 90%
    MODEL_MAX2_ALL&(~MODEL_11), // 100%
};
#pragma endregion

#pragma region LED
#define LED_ID 91
#define LED_SIZE 6
#define LED_NAME "LED"
#define LED_AVA MODEL_MAX2_ALL
const static char* LED_STRING[] = {
    "Off",
    "On",
    "All On",
    "All Off",
    "Front Off Only",
    "Back Only",
};
const static int32_t LED_VALUE[] = {
    0, // Off
    2, // On
    3, // All On
    4, // All Off
    5, // Front Off Only
    100, // Back Only
};
const static int32_t LED_SUPPORT[] = {
    MODEL_11, // Off
    MODEL_11, // On
    MODEL_MAX2_ALL&(~MODEL_11), // All On
    MODEL_MAX2_ALL&(~(MODEL_11|MODEL_13)), // All Off
    MODEL_MAX2_ALL&(~(MODEL_11|MODEL_13)), // Front Off Only
    MODEL_13, // Back Only
};
#pragma endregion

#pragma region Video Aspect Ratio
#define VIDEO_ASPECT_RATIO_ID 108
#define VIDEO_ASPECT_RATIO_SIZE 6
#define VIDEO_ASPECT_RATIO_NAME "Video Aspect Ratio"
#define VIDEO_ASPECT_RATIO_AVA MODEL_MAX2|MODEL_13|MODEL_12_BLACK
const static char* VIDEO_ASPECT_RATIO_STRING[] = {
    "4:3",
    "16:9",
    "8:7",
    "9:16",
    "21:9",
    "1:1",
};
const static int32_t VIDEO_ASPECT_RATIO_VALUE[] = {
    0, // 4:3
    1, // 16:9
    3, // 8:7
    4, // 9:16
    5, // 21:9
    6, // 1:1
};
const static int32_t VIDEO_ASPECT_RATIO_SUPPORT[] = {
    MODEL_MAX2|MODEL_13|MODEL_12_BLACK, // 4:3
    MODEL_MAX2|MODEL_13|MODEL_12_BLACK, // 16:9
    MODEL_13|MODEL_12_BLACK, // 8:7
    MODEL_MAX2|MODEL_13|MODEL_12_BLACK, // 9:16
    MODEL_13, // 21:9
    MODEL_13, // 1:1
};
#pragma endregion

#pragma region White Balance
#define WHITE_BALANCE_ID 115
#define WHITE_BALANCE_SIZE 11
#define WHITE_BALANCE_NAME "White Balance"
const static char* WHITE_BALANCE_STRING[] = {
    "6500K",
    "6000K",
    "5500K",
    "5000K",
    "4500K",
    "Auto",
    "Native",
    "4000K",
    "3200K",
    "2800K",
    "2300K",
};
const static int32_t WHITE_BALANCE_VALUE[] = {
    3, 
    7, 
    2, 
    12, 
    11, 
    0, 
    4, 
    5, 
    10, 
    9, 
    ,
};
#pragma endregion

#pragma region Sharpness
#define SHARPNESS_ID 117
#define SHARPNESS_SIZE 3
#define SHARPNESS_NAME "Sharpness"
#define SHARPNESS_AVA
const static char* SHARPNESS_STRING[] = {
    "High",
    "Midium",
    "Low"
};
const static int32_t SHARPNESS_VALUE[] = {
    0, 1, 2
};
#pragma endregion

#pragma region Exposure
#define EXPOSURE_ID 118
#define EXPOSURE_SIZE 9
#define EXPOSURE_NAME "Exposure"
#define EXPOSURE_AVA
const static char* EXPOSURE_STRING[] = {
    "-2.0",
    "-1.5",
    "-1.0",
    "-0.5",
    "0.0",
    "0.5",
    "1.0",
    "1.5",
    "2.0",
};
const static int32_t EXPOSURE_VALUE[] = {
    8, 7, 6, 5, 4, 3, 2, 1, 0
};
#pragma endregion

#pragma region Video Lens
#define VIDEO_LENS_ID 121
#define VIDEO_LENS_SIZE 13
#define VIDEO_LENS_NAME "Video Lens"
#define VIDEO_LENS_AVA MODEL_MAX2_ALL
const static char* VIDEO_LENS_STRING[] = {
    "Wide",
    "Narrow",
    "Superview",
    "Linear",
    "Max Superview",
    "Linear + Horizon Leveling",
    "HyperView",
    "Linear + Horizon Lock",
    "Max HyperView",
    "Ultra SuperView",
    "Ultra Wide",
    "Ultra Linear",
    "Ultra HyperView",
};
const static int32_t VIDEO_LENS_VALUE[] = {
    0, // Wide",
    2, // Narrow",
    3, // Superview",
    4, // Linear",
    7, // Max Superview",
    8, // Linear + Horizon Leveling",
    9, // HyperView",
    10, // Linear + Horizon Lock",
    11, // Max HyperView",
    12, // Ultra SuperView",
    13, // Ultra Wide",
    14, // Ultra Linear",
    104, // Ultra HyperView",
};
const static int32_t VIDEO_LENS_SUPPORT[] = {
    MODEL_MAX2_ALL, // Wide",
    2, // Narrow",
    3, // Superview",
    MODEL_MAX2_ALL, // Linear",
    7, // Max Superview",
    8, // Linear + Horizon Leveling",
    9, // HyperView",
    10, // Linear + Horizon Lock",
    MODEL_MAX2|MODEL_12, // Max HyperView",
    MODEL_13, // Ultra SuperView",
    MODEL_MAX2|MODEL_13, // Ultra Wide",
    MODEL_13, // Ultra Linear",
    MODEL_13, // Ultra HyperView",
};
#pragma endregion

#define PHOTO_LENS_ID 122
#define PHOTO_LENS_SIZE 17
#define PHOTO_LENS_NAME "Photo Lens"

const static char* PHOTO_LENS_STRING[] = {
    "Wide 12 MP",
    "Linear 12 MP",
    "9MP Wide",
    "Narrow",
    "Wide 23 MP",
    "Linear 23 MP",
    "Wide 27 MP",
    "Linear 27 MP",
    "9MP Linear",
    "13MP Linear",
    "13MP Wide",
    "13MP Ultra Wide",
    "Ultra Wide 12 MP",
    "13MP Ultra Linear",
    "Max SuperView",
    "Wide",
    "Linear"
};

const static int32_t PHOTO_LENS_VALUE[] = {
    0, 10, 15, 19, 27, 28, 31, 32, 37, 38, 39, 40, 41, 44,
    100, 101, 102
};

#define TIME_LAPSE_DIGITAL_LENSES_ID 123
#define TIME_LAPSE_DIGITAL_LENSES_SIZE 6
#define TIME_LAPSE_DIGITAL_LENSES_NAME "Time Lapse Digital Lenses"

const static char* TIME_LAPSE_DIGITAL_LENSES_STRING[] = {
    "Narrow",
    "Wide 27 MP",
    "Linear 27 MP",
    "Max SuperView",
    "Wide",
    "Linear"
};

const static int32_t TIME_LAPSE_DIGITAL_LENSES_VALUE[] = {
    19, 31, 32, 100, 101, 102
};

#define PHOTO_OUTPUT_ID 125
#define PHOTO_OUTPUT_SIZE 4
#define PHOTO_OUTPUT_NAME "Photo Output"

const static char* PHOTO_OUTPUT_STRING[] = {
    "Standard",
    "RAW",
    "HDR",
    "SuperPhoto"
};

const static int32_t PHOTO_OUTPUT_VALUE[] = {
    0, 1, 2, 3
};

#define MEDIA_FORMAT_ID 128
#define MEDIA_FORMAT_SIZE 4
#define MEDIA_FORMAT_NAME "Media Format"

const static char* MEDIA_FORMAT_STRING[] = {
    "Time Lapse Video",
    "Time Lapse Photo",
    "Night Lapse Photo",
    "Night Lapse Video"
};

const static int32_t MEDIA_FORMAT_VALUE[] = {
    13, 20, 21, 26
};

#define ANTI_FLICKER_ID 134
#define ANTI_FLICKER_SIZE 4
#define ANTI_FLICKER_NAME "Anti-Flicker"

const static char* ANTI_FLICKER_STRING[] = {
    "NTSC",
    "PAL",
    "60Hz",
    "50Hz"
};

const static int32_t ANTI_FLICKER_VALUE[] = {
    0, 1, 2, 3
};

#define HYPERSMOOTH_ID 135
#define HYPERSMOOTH_SIZE 6
#define HYPERSMOOTH_NAME "Hypersmooth"

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

#define SHUTTER_SPEED_ID 145
#define SHUTTER_SPEED_SIZE 22
#define SHUTTER_SPEED_NAME "Shutter Speed"

const static char* SHUTTER_SPEED_STRING[] = {
    "Auto",
    "1/24",
    "1/25",
    "1/30",
    "1/48",
    "1/50",
    "1/60",
    "1/96",
    "1/100",
    "1/120",
    "1/192",
    "1/200",
    "1/240",
    "1/384",
    "1/400",
    "1/480",
    "1/800",
    "1/900",
    "1/1600",
    "1/1920",
    "1/3200",
    "1/3840",
};

const static int32_t SHUTTER_SPEED_VALUE[] = {
    0, 3, 4, 5, 6, 7, 8, 11, 12, 13, 16, 17, 18, 25, 21, 22, 28,
    23, 29, 24, 30, 31
};

#define VIDEO_HORIZON_LEVELING_ID 150
#define VIDEO_HORIZON_LEVELING_SIZE 3
#define VIDEO_HORIZON_LEVELING_NAME "Video Horizon Leveling"

const static char* VIDEO_HORIZON_LEVELING_STRING[] = {
    "Off",
    "On",
    "Locked"
};

const static int32_t VIDEO_HORIZON_LEVELING_VALUE[] = {
    0, 1, 2
};

#define PHOTO_HORIZON_LEVELING_ID 151
#define PHOTO_HORIZON_LEVELING_SIZE 3
#define PHOTO_HORIZON_LEVELING_NAME "Photo Horizon Leveling"

const static char* PHOTO_HORIZON_LEVELING_STRING[] = {
    "Off",
    "On",
    "Locked"
};

const static int32_t PHOTO_HORIZON_LEVELING_VALUE[] = {
    0, 1, 2
};

#define VIDEO_DURATION_ID 156
#define VIDEO_DURATION_SIZE 11
#define VIDEO_DURATION_NAME "Video Duration"

const static char* VIDEO_DURATION_STRING[] = {
    "15 Sec",
    "30 Sec",
    "1 Min",
    "5 Min",
    "15 Min",
    "30 Min",
    "1 Hour",
    "2 Hour",
    "3 Hour",
    "5 Sec",
    "No Limit",
};

const static int32_t VIDEO_DURATION_VALUE[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100
};

#define MULTISHOT_DURATION_ID 157
#define MULTISHOT_DURATION_SIZE 11
#define MULTISHOT_DURATION_NAME "Multi Shot Duration"

const static char* MULTISHOT_DURATION_STRING[] = {
    "Off",
    "15 Sec",
    "30 Sec",
    "1 Min",
    "5 Min",
    "15 Min",
    "30 Min",
    "1 Hour",
    "2 Hour",
    "3 Hour",
    "No Limit",
};

const static int32_t MULTISHOT_DURATION_VALUE[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100
};

#define MAX_LENS_ID 162
#define MAX_LENS_SIZE 2
#define MAX_LENS_NAME "Max Lens"

const static char* MAX_LENS_STRING[] = {
    "Off",
    "On"
};

const static int32_t MAX_LENS_VALUE[] = {
    0, 1
};

#define HINDSIGHT_ID 167
#define HINDSIGHT_SIZE 3
#define HINDSIGHT_NAME "HindSight"

const static char* HINDSIGHT_STRING[] = {
    "15 Sec",
    "30 Sec",
    "Off",
};

const static int32_t HINDSIGHT_VALUE[] = {
    2, 3, 4
};

#define SCHEDULED_CAPTURE_ID 168
#define SCHEDULED_CAPTURE_SIZE 2
#define SCHEDULED_CAPTURE_NAME "Scheduled Capture"

const static char* SCHEDULED_CAPTURE_STRING[] = {
    "Off",
    "On"
};

const static int32_t SCHEDULED_CAPTURE_VALUE[] = {
    0, 1
};

#define PHOTO_SINGLE_INTERVAL_ID 171
#define PHOTO_SINGLE_INTERVAL_SIZE 10
#define PHOTO_SINGLE_INTERVAL_NAME "Photo Single Interval"

const static char* PHOTO_SINGLE_INTERVAL_STRING[] = {
    "Off",
    "0.5s",
    "1s",
    "2s",
    "5s",
    "10s",
    "30s",
    "60s",
    "120s",
    "3s",
};

const static int32_t PHOTO_SINGLE_INTERVAL_VALUE[] = {
    0, 2, 3, 4, 5, 6, 7, 8, 9, 10
};

#define PHOTO_SINGLE_DURATION_ID 172
#define PHOTO_SINGLE_DURATION_SIZE 10
#define PHOTO_SINGLE_DURATION_NAME "Photo Single Duration"

const static char* PHOTO_SINGLE_DURATION_STRING[] = {
    "Off",
    "15 Sec",
    "30 Sec",
    "1 Min",
    "5 Min",
    "15 Min",
    "30 Min",
    "1 Hour",
    "2 Hour",
    "3 Hour",
};

const static int32_t PHOTO_SINGLE_DURATION_VALUE[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

#define VIDEO_PERFORMANCE_MODE_ID 173
#define VIDEO_PERFORMANCE_MODE_SIZE 3
#define VIDEO_PERFORMANCE_MODE_NAME "Video Performance Mode"

const static char* VIDEO_PERFORMANCE_MODE_STRING[] = {
    "Maximum Video Performance",
    "Extended Battery",
    "Tripod / Stationary Video"
};

const static int32_t VIDEO_PERFORMANCE_MODE_VALUE[] = {
    0, 1, 2
};

#define CONTROL_MODE_ID 175
#define CONTROL_MODE_SIZE 2
#define CONTROL_MODE_NAME "Control Mode"

const static char* CONTROL_MODE_STRING[] = {
    "Easy",
    "Pro"
};

const static int32_t CONTROL_MODE_VALUE[] = {
    0, 1
};

#define EASY_MODE_SPEED_ID 176
#define EASY_MODE_SPEED_SIZE 88
#define EASY_MODE_SPEED_NAME "Easy Mode Speed"

const static char* EASY_MODE_SPEED_STRING[] = {
    "8X Ultra Slo-Mo",
    "4X Super Slo-Mo",
    "2X Slo-Mo",
    "1X Speed (Low Light)",
    "4X Super Slo-Mo (Ext. Batt.)",
    "2X Slo-Mo (Ext. Batt.)",
    "1X Speed (Ext. Batt.) (Low Light)",
    "8X Ultra Slo-Mo (50Hz)",
    "4X Super Slo-Mo (50Hz)",
    "2X Slo-Mo (50Hz)",
    "1X Speed (50Hz) (Low Light)",
    "4X Super Slo-Mo (50Hz) (Ext. Batt.)",
    "2X Slo-Mo (50Hz) (Ext. Batt.)",
    "1X Speed (50Hz) (Ext. Batt.) (Low Light)",
    "8X Ultra Slo-Mo (Ext. Batt.)",
    "8X Ultra Slo-Mo (50Hz) (Ext. Batt.)",
    "8X Ultra Slo-Mo (Long. Batt.)",
    "4X Super Slo-Mo (Long. Batt.)",
    "2X Slo-Mo (Long. Batt.)",
    "1X Speed (Long. Batt.) (Low Light)",
    "8X Ultra Slo-Mo (50Hz) (Long. Batt.)",
    "4X Super Slo-Mo (50Hz) (Long. Batt.)",
    "2X Slo-Mo (50Hz) (Long. Batt.)",
    "1X Speed (50Hz) (Long. Batt.) (Low Light)",
    "2X Slo-Mo (4K)",
    "4X Super Slo-Mo (2.7K)",
    "2X Slo-Mo (4K) (50Hz)",
    "4X Super Slo-Mo (2.7K) (50Hz)",
    "8X Ultra Slo-Mo (V2)",
    "4X Super Slo-Mo (V2)",
    "2X Slo-Mo (V2)",
    "1X Speed (Low Light) (V2)",
    "8X Ultra Slo-Mo (50Hz) (V2)",
    "4X Super Slo-Mo (50Hz) (V2)",
    "2X Slo-Mo (50Hz) (V2)",
    "1X Speed (50Hz) (Low Light) (V2)",
    "8X Ultra Slo-Mo (Long. Batt.) (V2)",
    "4X Super Slo-Mo (Long. Batt.) (V2)",
    "2X Slo-Mo (Long. Batt.) (V2)",
    "1X Speed (Long. Batt.) (Low Light) (V2)",
    "8X Ultra Slo-Mo (50Hz) (Long. Batt.) (V2)",
    "4X Super Slo-Mo (50Hz) (Long. Batt.) (V2)",
    "2X Slo-Mo (50Hz) (Long. Batt.) (V2)",
    "1X Speed (50Hz) (Long. Batt.) (Low Light) (V2)",
    "2X Slo-Mo (4K) (V2)",
    "2X Slo-Mo (4K) (50Hz) (V2)",
    "1X Speed (Low Light) (V2) (Vertical)",
    "1X Speed (50Hz) (Low Light) (V2) (Vertical)",
    "2X Slo-Mo (V2) (Vertical)",
    "2X Slo-Mo (50Hz) (V2) (Vertical)",
    "1X Speed (Full Frame) (Low Light) (V2)",
    "1X Speed (50Hz) (Full Frame) (Low Light) (V2)",
    "2X Slo-Mo (Full Frame) (V2)",
    "2X Slo-Mo (50Hz) (Full Frame) (V2)",
    "1X Speed (4K) (Low Light) (V2)",
    "1X Speed (4K) (50Hz) (Low Light) (V2)",
    "1X Speed (2.7K) (Low Light) (V2)",
    "1X Speed (2.7K) (50Hz) (Low Light) (V2)",
    "2X Slo-Mo (2.7K) (V2)",
    "2X Slo-Mo (2.7K) (50Hz) (V2)",
    "2X Slo-Mo (Long. Batt.) (V2) (Vertical)",
    "2X Slo-Mo (50Hz) (Long. Batt.) (V2) (Vertical)",
    "1X Speed (Long. Batt.) (Low Light) (V2) (Vertical)",
    "1X Speed (50Hz) (Long. Batt.) (Low Light) (V2) (Vertical)",
    "1X Speed (4K) (Full Frame) (Low Light) (V2)",
    "1X Speed (4K) (50Hz) (Full Frame) (Low Light) (V2)",
    "1X Normal Speed (1:1) (30 Fps) (4K) (V2)",
    "1X Normal Speed (1:1) (25 Fps) (4K) (V2)",
    "2X Slo-Mo Speed (1:1) (4K) (60 Fps) (V2)",
    "2X Slo-Mo Speed (1:1) (4K) (50 Fps) (V2)",
    "1X Normal Speed (21:9) (30 Fps) (5.3K) (V2)",
    "1X Normal Speed (21:9) (25 Fps) (5.3K) (V2)",
    "2X Slo-Mo Speed (21:9) (5.3K) (60 Fps) (V2)",
    "2X Slo-Mo Speed (21:9) (5.3K) (50 Fps) (V2)",
    "1X Normal Speed (21:9) (30 Fps) (4K) (V2)",
    "1X Normal Speed (21:9) (25 Fps) (4K) (V2)",
    "2X Slo-Mo Speed (21:9) (4K) (60 Fps) (V2)",
    "2X Slo-Mo Speed (21:9) (4K) (50 Fps) (V2)",
    "120 4X Super Slo-Mo Speed (21:9) (4K) (V2)",
    "100 4X Super Slo-Mo Speed (21:9) (4K) (V2)",
    "1X Normal Speed (30 Fps) (4:3) (5.3K) (V2)",
    "1X Normal Speed (25 Fps) (4:3) (5.3K) (V2)",
    "1X Normal Speed (30 Fps) (4:3) (4K) (V2)",
    "1X Normal Speed (25 Fps) (4:3) (4K) (V2)",
    "2X Slo-Mo Speed (4:3) (4K) (60 Fps) (V2)",
    "2X Slo-Mo Speed (4:3) (4K) (50 Fps) (V2)",
    "120 4X Super Slo-Mo Speed (2.7K) (4:3) (V2)",
    "100 4X Super Slo-Mo Speed (2.7K) (4:3) (V2)",
};

const static int32_t EASY_MODE_SPEED_VALUE[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 
    146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159
};

#define ENABLE_NIGHT_PHOTO_ID 177
#define ENABLE_NIGHT_PHOTO_SIZE 2
#define ENABLE_NIGHT_PHOTO_NAME "Enable Night Photo"

const static char* ENABLE_NIGHT_PHOTO_STRING[] = {
    "Off",
    "On"
};

const static int32_t ENABLE_NIGHT_PHOTO_VALUE[] = {
    0, 1
};

#define WIRELESS_BAND_ID 178
#define WIRELESS_BAND_SIZE 2
#define WIRELESS_BAND_NAME "Wireless Band"

const static char* WIRELESS_BAND_STRING[] = {
    "2.4 GHz",
    "5 GHz"
};

const static int32_t WIRELESS_BAND_VALUE[] = {
    0, 1
};

#define STAR_TRAILS_LENGTH_ID 179
#define STAR_TRAILS_LENGTH_SIZE 3
#define STAR_TRAILS_LENGTH_NAME "Star Trails Length"

const static char* STAR_TRAILS_LENGTH_STRING[] = {
    "Short",
    "Long",
    "Max"
};

const static int32_t STAR_TRAILS_LENGTH_VALUE[] = {
    1, 2, 3
};

#define SYSTEM_VIDEO_MODE_ID 180
#define SYSTEM_VIDEO_MODE_SIZE 5
#define SYSTEM_VIDEO_MODE_NAME "System Video Mode"

const static char* SYSTEM_VIDEO_MODE_STRING[] = {
    "Highest Quality",
    "Extended Battery",
    "Longest Battery",
    "Standard Quality",
    "Basic Quality",
};

const static int32_t SYSTEM_VIDEO_MODE_VALUE[] = {
    0, 101, 102, 111, 112
};

#define VIDEO_BIT_RATE_ID 182
#define VIDEO_BIT_RATE_SIZE 2
#define VIDEO_BIT_RATE_NAME "Video Bit Rate"

const static char* VIDEO_BIT_RATE_STRING[] = {
    "Standard",
    "High"
};

const static int32_t VIDEO_BIT_RATE_VALUE[] = {
    0, 1
};

#define BIT_DEPTH_ID 183
#define BIT_DEPTH_SIZE 2
#define BIT_DEPTH_NAME "Bit Depth"

const static char* BIT_DEPTH_STRING[] = {
    "8-bit",
    "10-bit"
};

const static int32_t BIT_DEPTH_VALUE[] = {
    0, 2
};

#define PROFILES_ID 184
#define PROFILES_SIZE 3
#define PROFILES_NAME "Profiles"

const static char* PROFILES_STRING[] = {
    "Standard",
    "HDR",
    "Log",
    "HLG HDR"
};

const static int32_t PROFILES_VALUE[] = {
    0, 1, 2, 101
};

#define VIDEO_EASY_MODE_ID 186
#define VIDEO_EASY_MODE_SIZE 5
#define VIDEO_EASY_MODE_NAME "Video Easy Mode"

const static char* VIDEO_EASY_MODE_STRING[] = {
    "Highest Quality",
    "Standard Quality",
    "Basic Quality",
    "Standard Video",
    "HDR Video"
};

const static int32_t VIDEO_EASY_MODE_VALUE[] = {
    0, 1, 2, 3, 4
};

#define LAPSE_MODE_ID 187
#define LAPSE_MODE_SIZE 5
#define LAPSE_MODE_NAME "Lapse Mode"

const static char* LAPSE_MODE_STRING[] = {
    "TimeWarp",
    "Star Trails",
    "Light Painting",
    "Vehicle Lights",
    "Max TimeWarp",
    "Max Star Trails",
    "Max Light Painting",
    "Max Vehicle Lights",
    "Time Lapse Video",
    "Night Lapse Video",
};

const static int32_t LAPSE_MODE_VALUE[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

#define MAX_LENS_MOD_ID 189
#define MAX_LENS_MOD_SIZE 12
#define MAX_LENS_MOD_NAME "Max Lens Mod"

const static char* MAX_LENS_MOD_STRING[] = {
    "None",
    "Max Lens 1.0",
    "Max Lens 2.0",
    "Max Lens 2.5",
    "Macro",
    "Anamorphic",
    "ND 4",
    "ND 8",
    "ND 16",
    "ND 32",
    "Standard Lens",
    "Auto Detect",
};

const static int32_t MAX_LENS_MOD_VALUE[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100
};

#define MAX_LENS_MOD_ENABLE_ID 190
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

#define EASY_NIGHT_PHOTO_ID 191
#define EASY_NIGHT_PHOTO_SIZE 2
#define EASY_NIGHT_PHOTO_NAME "Easy Night Photo"

const static char* EASY_NIGHT_PHOTO_STRING[] = {
    "Off",
    "On"
};

const static int32_t EASY_NIGHT_PHOTO_VALUE[] = {
    0, 1
};

#define MULTI_SHOT_ASPECT_RATIO_ID 192
#define MULTI_SHOT_ASPECT_RATIO_SIZE 4
#define MULTI_SHOT_ASPECT_RATIO_NAME "Multi Shot Aspect Ratio"

const static char* MULTI_SHOT_ASPECT_RATIO_STRING[] = {
    "4:3",
    "16:9",
    "8:7",
    "9:16"
};

const static int32_t MULTI_SHOT_ASPECT_RATIO_VALUE[] = {
    0, 1, 3, 4
};

#define FRAMING_ID 193
#define FRAMING_SIZE 3
#define FRAMING_NAME "Framing"

const static int32_t FRAMING_VALUE[] = {
    0, 1, 2
};

const static char* FRAMING_STRING[] = {
    "Widescreen",
    "Vertical",
    "Full Frame"
};

#define CAMERA_MODE_ID 194
#define CAMERA_MODE_SIZE 2
#define CAMERA_MODE_NAME "Camera Mode"

const static char* CAMERA_MODE_STRING[] = {
    "Single Lens",
    "360"
};

const static int32_t CAMERA_MODE_VALUE[] = {
    0, 1
};

#define _360_PHOTO_FILES_EXTENSION_ID 196
#define _360_PHOTO_FILES_EXTENSION_SIZE 2
#define _360_PHOTO_FILES_EXTENSION_NAME "360 Photo Files Extension"

const static char* _360_PHOTO_FILES_EXTENSION_STRING[] = {
    ".360",
    ".jpg"
};

const static int32_t _360_PHOTO_FILES_EXTENSION_VALUE[] = {
    0, 1
};

#define BEEP_VOLUME_ID 216
#define BEEP_VOLUME_SIZE 4
#define BEEP_VOLUME_NAME "Beep Volume"

const static char* BEEP_VOLUME_STRING[] = {
    "Off",
    "Low",
    "Medium",
    "High"
};

const static int32_t BEEP_VOLUME_VALUE[] = {
    0, 1, 2, 3
};

#define SETUP_SCREEN_SAVER_ID 219
#define SETUP_SCREEN_SAVER_SIZE 5
#define SETUP_SCREEN_SAVER_NAME "Setup Screen Saver"

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

#define VIDEO_FRAMING_ID 232
#define VIDEO_FRAMING_SIZE 3
#define VIDEO_FRAMING_NAME "Video Framing"

const static char* VIDEO_FRAMING_STRING[] = {
    "Widescreen",
    "Vertical",
    "Full Frame"
};

const static int32_t VIDEO_FRAMING_VALUE[] = {
    0, 1, 3
};

#define AUTOMATIC_WI_FI_ACCESS_POINT_ID 236
#define AUTOMATIC_WI_FI_ACCESS_POINT_SIZE 2
#define AUTOMATIC_WI_FI_ACCESS_POINT_NAME "Automatic Wi-Fi Access Point"

const static char* AUTOMATIC_WI_FI_ACCESS_POINT_STRING[] = {
    "Off",
    "On"
};

const static int32_t AUTOMATIC_WI_FI_ACCESS_POINT_VALUE[] = {
    0, 1
};

#define COLOR_ID 116

#define LENS_ID 121
#define BITRATE_ID 124

#endif