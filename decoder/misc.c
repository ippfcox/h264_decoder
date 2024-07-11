#include <math.h>
#include "misc.h"

char spaces64[] = "                                                                ";

char *nal_type_names[] = {
    "H264_NAL_UNSPECIFIED",
    "H264_NAL_SLICE",
    "H264_NAL_DPA",
    "H264_NAL_DPB",
    "H264_NAL_DPC",
    "H264_NAL_IDR_SLICE",
    "H264_NAL_SEI",
    "H264_NAL_SPS",
    "H264_NAL_PPS",
    "H264_NAL_AUD",
    "H264_NAL_END_SEQUENCE",
    "H264_NAL_END_STREAM",
    "H264_NAL_FILLER_DATA",
    "H264_NAL_SPS_EXT",
    "H264_NAL_PREFIX",
    "H264_NAL_SUB_SPS",
    "H264_NAL_DPS",
    "H264_NAL_RESERVED17",
    "H264_NAL_RESERVED18",
    "H264_NAL_AUXILIARY_SLICE",
    "H264_NAL_EXTEN_SLICE",
    "H264_NAL_DEPTH_EXTEN_SLICE",
    "H264_NAL_RESERVED22",
    "H264_NAL_RESERVED23",
    "H264_NAL_UNSPECIFIED24",
    "H264_NAL_UNSPECIFIED25",
    "H264_NAL_UNSPECIFIED26",
    "H264_NAL_UNSPECIFIED27",
    "H264_NAL_UNSPECIFIED28",
    "H264_NAL_UNSPECIFIED29",
    "H264_NAL_UNSPECIFIED30",
    "H264_NAL_UNSPECIFIED31",
};

const char *nal_type_name(enum H264_NAL_TYPE type)
{
    return nal_type_names[type];
}

int get_log2(uint64_t value)
{
    int result = 0;
    while (value)
        value >>= 2;
    return result;
}