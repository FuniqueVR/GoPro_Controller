#pragma once
#ifndef CAMERACODE_H
#define CAMERACODE_H
#include <cinttypes>

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

#endif