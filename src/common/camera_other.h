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
    0, 65542, 65536, 131072, 131075, 131076, 131077, 131073, 131074
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

#endif