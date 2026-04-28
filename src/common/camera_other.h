#pragma once
#ifndef CAMERA_OTHER
#define CAMERA_OTHER
#include <cinttypes>

#define GOPRO_MODE_SIZE 9
#define GOPRO_MODE_NAME "Webcam Mode"

const static char* GOPRO_MODE_STRING[] = {
    "Video",
    "Photo Burst",
    "Photo Single",
    "Timelapse",
    "Timelapse Trail",
    "Timelapse Light Painting",
    "Timelapse Light Trail",
    "Timelapse Video",
    "Timelapse Night Video",
};

const static int32_t GOPRO_MODE_VALUE[] = {
    0, // Video
    65538, // Photo Burst
    65536, // Photo Single
    131072, // Timelapse
    131075, // Timelapse Trail
    131076, // Timelapse Light Painting
    131077, // Timelapse Light Trail
    131073, // Timelapse Video
    131074, // Timelapse Night Video
};

#define WEBCAM_START_RES_SIZE 3
#define WEBCAM_START_RES_NAME "Webcam Resolution"

const static char* WEBCAM_START_RES_STRING[] = {
    "480p",
    "720p",
    "1080p"
};

const static int32_t WEBCAM_START_RES_VALUE[] = {
    4, 7, 12
};

#define WEBCAM_START_FOV_SIZE 4
#define WEBCAM_START_FOV_NAME "Webcam Field-of-View"

const static char* WEBCAM_START_FOV_STRING[] = {
    "Wide",
    "Narrow",
    "Superview",
    "Linear"
};

const static int32_t WEBCAM_START_FOV_VALUE[] = {
    0, 2, 3, 4
};

enum class ExecutionType {
    Set, SetAll
};

#endif