#pragma once
#ifndef CAMERA_OTHER
#define CAMERA_OTHER
#include <cinttypes>

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