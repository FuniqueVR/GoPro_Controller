/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#ifndef CAMERACODE_H
#define CAMERACODE_H
#include <cinttypes>

#define GOPRO_SETTING_SIZE 54
#define GOPRO_STATUS_SIZE 82

const static int32_t GOPRO_SETTING_IDS[] = {
    2,3,5,30,32,43,59,83,88,91,108,121,122,123,125,128,134,135,150,151,156,157,162,167,168,171,172,173,175,176,
    177,178,179,180,182,183,184,186,187,189,190,191,192,193,194,196,216,219,223,227,232,233,234,236
};
const static int32_t GOPRO_STATE_IDS[] = {
    1,2,6,8,9,10,11,13,17,19,20,21,22,23,24,26,27,28,29,30,31,32,33,34,35,38,39,41,42,45,49,54,55,56,58,59,60,
    65,66,67,68,69,70,74,75,76,77,78,79,81,82,83,85,86,88,89,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,
    110,111,112,113,114,115,116,117,118,122
};

#define VIDEO_RESOLUTION_SIZE 27
#define VIDEO_RESOLUTION_NAME "Video Resolution"

enum VIDEO_RESOLUTION_ {
    V4K,
    V27K,
    V27K34,
    V1440,
    V1080,
    V720,
    V4K43,
    V56K,
    V5K,
    V5K43,
    V53K87,
    V53K43,
    V4K87,
    V8K,
    V53K219,
    V4K219,
    V4K11,
    V900,
    V4KSPH,
    V53K,
    V53K87V2,
    V4K87V2,
    V4K916V2,
    V1080916V2,
    V27K43V2,
    V4K43V2,
    V53K43V2
};

const static char* VIDEO_RESOLUTION_STRING[] = {
    "4K",
    "2.7K",
    "2.7K 3:4",
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
    "4K43 V2",
    "5.3K 4:3 V2"
};

const static int32_t VIDEO_RESOLUTION_VALUE[] = {
    1,
    4,
    6,
    7,
    9,
    12,
    18,
    21,
    24,
    25,
    26,
    27,
    28,
    31,
    35,
    36,
    37,
    38,
    39,
    100,
    107,
    108,
    109,
    110,
    111,
    112,
    113
};

#define FRAME_PRE_SECOND_SIZE 13
#define FRAME_PRE_SECOND_NAME "Frame Per Second"

enum FRAME_PRE_SECOND_ {
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
    V300,
};

const static char* FRAME_PRE_SECOND_STRING[] = {
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

const static int32_t FRAME_PRE_SECOND_VALUE[] = {
    0,
    1,
    2,
    3,
    5,
    6,
    8,
    9,
    10,
    13,
    15,
    16,
    17
};

enum VIDEO_TIMELAPSE_RATE_ {
    V0_5Sec,
    V1Sec,
    V2Sec,
    V5Sec,
    V10Sec,
    V30Sec,
    V60Sec,
    V2M,
    V5M,
    V30M,
    V60M,
    V3Sec,
};

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
    "3 Sec",
};

const static int32_t VIDEO_TIMELAPSE_RATE_VALUE[] = {
    0,1,2,3,4,5,6,7,8,9,10,11
};

int32_t GET_SETTING_SIZE_BY_ID(int32_t x){
    if(x == 2) return VIDEO_RESOLUTION_SIZE;
    else if(x == 3) return FRAME_PRE_SECOND_SIZE;
    else return 0;
};

const char* GET_SETTING_NAME_BY_ID(int32_t x){
    if(x == 2) return VIDEO_RESOLUTION_NAME;
    else if(x == 3) return FRAME_PRE_SECOND_NAME;
    else return "";
};
    
const char** GET_SETTING_STRING_BY_ID(int32_t x){
    if(x == 2) return FRAME_PRE_SECOND_STRING;
    else if(x == 3) return VIDEO_TIMELAPSE_RATE_STRING;
    return nullptr;
};
    
const int32_t* GET_SETTING_VALUE_BY_ID(int32_t x){
    if(x == 2) return FRAME_PRE_SECOND_VALUE;
    else if(x == 3) return VIDEO_TIMELAPSE_RATE_VALUE;
    else return nullptr;
};
#endif