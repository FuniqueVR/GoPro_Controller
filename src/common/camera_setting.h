#pragma once
#ifndef CAMERA_SETTING
#define CAMERA_SETTING
#include <cinttypes>

#define VIDEO_RESOLUTION_SIZE 27
#define VIDEO_RESOLUTION_NAME "Video Resolution"

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
    "300.0"
};

const static int32_t FRAMES_PER_SECOND_VALUE[] = {
    0, 1, 2, 3, 5, 6, 8, 9, 10, 13,
    15, 16, 17
};

#define VIDEO_TIMELAPSE_RATE_SIZE 12
#define VIDEO_TIMELAPSE_RATE_NAME "Video Timelapse Rate"

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
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

#define PHOTO_TIMELAPSE_RATE_SIZE 12
#define PHOTO_TIMELAPSE_RATE_NAME "Photo Timelapse Rate"

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
    11, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110
};

#define NIGHTLAPSE_RATE_SIZE 12
#define NIGHTLAPSE_RATE_NAME "Nightlapse Rate"

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
    4, 5, 10, 15, 20, 30, 100, 120, 300, 1800, 3600, 3601
};

#define WEBCAM_DIGITAL_LENSES_SIZE 4
#define WEBCAM_DIGITAL_LENSES_NAME "Webcam Digital Lenses"

const static char* WEBCAM_DIGITAL_LENSES_STRING[] = {
    "Wide",
    "Narrow",
    "Superview",
    "Linear",
};

const static int32_t WEBCAM_DIGITAL_LENSES_VALUE[] = {
    0, 2, 3, 4
};

#define AUTO_POWER_DOWN_SIZE 7
#define AUTO_POWER_DOWN_NAME "Auto Power Down"

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
    0, 1, 4, 6, 7, 11, 12
};

#define GPS_SIZE 2
#define GPS_NAME "GPS"

const static char* GPS_STRING[] = {
    "Off",
    "On"
};

const static int32_t GPS_VALUE[] = {
    0, 1
};

#define LCD_BRIGHTNESS_SIZE 10
#define LCD_BRIGHTNESS_NAME "LCD Brightness"

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
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100
};

#define LED_SIZE 6
#define LED_NAME "LED"

const static char* LED_STRING[] = {
    "Off",
    "On",
    "All On",
    "All Off",
    "Front Off Only",
    "Back Only",
};

const static int32_t LED_VALUE[] = {
    0, 2, 3, 4, 5, 100
};

#define VIDEO_ASPECT_RATIO_SIZE 6
#define VIDEO_ASPECT_RATIO_NAME "Video Aspect Ratio"

const static char* VIDEO_ASPECT_RATIO_STRING[] = {
    "4:3",
    "16:9",
    "8:7",
    "9:16",
    "21:9",
    "1:1",
};

const static int32_t VIDEO_ASPECT_RATIO_VALUE[] = {
    0, 1, 3, 4, 5, 6
};

#define VIDEO_LENS_SIZE 13
#define VIDEO_LENS_NAME "Video Lens"

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
    0, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14, 104
};

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

#define MAX_LENS_SIZE 2
#define MAX_LENS_NAME "Max Lens"

const static char* MAX_LENS_STRING[] = {
    "Off",
    "On"
};

const static int32_t MAX_LENS_VALUE[] = {
    0, 1
};

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

#define SCHEDULED_CAPTURE_SIZE 2
#define SCHEDULED_CAPTURE_NAME "Scheduled Capture"

const static char* SCHEDULED_CAPTURE_STRING[] = {
    "Off",
    "On"
};

const static int32_t SCHEDULED_CAPTURE_VALUE[] = {
    0, 1
};

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

#define CONTROL_MODE_SIZE 2
#define CONTROL_MODE_NAME "Control Mode"

const static char* CONTROL_MODE_STRING[] = {
    "Easy",
    "Pro"
};

const static int32_t CONTROL_MODE_VALUE[] = {
    0, 1
};

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

#define ENABLE_NIGHT_PHOTO_SIZE 2
#define ENABLE_NIGHT_PHOTO_NAME "Enable Night Photo"

const static char* ENABLE_NIGHT_PHOTO_STRING[] = {
    "Off",
    "On"
};

const static int32_t ENABLE_NIGHT_PHOTO_VALUE[] = {
    0, 1
};

#define WIRELESS_BAND_SIZE 2
#define WIRELESS_BAND_NAME "Wireless Band"

const static char* WIRELESS_BAND_STRING[] = {
    "2.4 GHz",
    "5 GHz"
};

const static int32_t WIRELESS_BAND_VALUE[] = {
    0, 1
};

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

#define VIDEO_BIT_RATE_SIZE 2
#define VIDEO_BIT_RATE_NAME "Video Bit Rate"

const static char* VIDEO_BIT_RATE_STRING[] = {
    "Standard",
    "High"
};

const static int32_t VIDEO_BIT_RATE_VALUE[] = {
    0, 1
};

#define BIT_DEPTH_SIZE 2
#define BIT_DEPTH_NAME "Bit Depth"

const static char* BIT_DEPTH_STRING[] = {
    "8-bit",
    "10-bit"
};

const static int32_t BIT_DEPTH_VALUE[] = {
    0, 2
};

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

const static char* EASY_NIGHT_PHOTO_STRING[] = {
    "Off",
    "On"
};

const static int32_t EASY_NIGHT_PHOTO_VALUE[] = {
    0, 1
};

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


#endif