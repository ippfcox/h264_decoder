#ifndef H264_H__
#define H264_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

    // Table 7-1 - NAL unit type codes
    enum H264_NAL_TYPE
    {
        H264_NAL_UNSPECIFIED = 0,
        H264_NAL_SLICE = 1,
        H264_NAL_DPA = 2,
        H264_NAL_DPB = 3,
        H264_NAL_DPC = 4,
        H264_NAL_IDR_SLICE = 5,
        H264_NAL_SEI = 6,
        H264_NAL_SPS = 7,
        H264_NAL_PPS = 8,
        H264_NAL_AUD = 9,
        H264_NAL_END_SEQUENCE = 10,
        H264_NAL_END_STREAM = 11,
        H264_NAL_FILLER_DATA = 12,
        H264_NAL_SPS_EXT = 13,
        H264_NAL_PREFIX = 14,
        H264_NAL_SUB_SPS = 15,
        H264_NAL_DPS = 16,
        H264_NAL_RESERVED17 = 17,
        H264_NAL_RESERVED18 = 18,
        H264_NAL_AUXILIARY_SLICE = 19,
        H264_NAL_EXTEN_SLICE = 20,
        H264_NAL_DEPTH_EXTEN_SLICE = 21,
        H264_NAL_RESERVED22 = 22,
        H264_NAL_RESERVED23 = 23,
        H264_NAL_UNSPECIFIED24 = 24,
        H264_NAL_UNSPECIFIED25 = 25,
        H264_NAL_UNSPECIFIED26 = 26,
        H264_NAL_UNSPECIFIED27 = 27,
        H264_NAL_UNSPECIFIED28 = 28,
        H264_NAL_UNSPECIFIED29 = 29,
        H264_NAL_UNSPECIFIED30 = 30,
        H264_NAL_UNSPECIFIED31 = 31,
    };

    enum H264_SLICE_TYPE
    {
        H264_SLICE_P = 0,
        H264_SLICE_B = 1,
        H264_SLICE_I = 2,
        H264_SLICE_SP = 3,
        H264_SLICE_SI = 4,
        H264_SLICE_P2 = 5,
        H264_SLICE_B2 = 6,
        H264_SLICE_I2 = 7,
        H264_SLICE_SP2 = 8,
        H264_SLICE_SI2 = 9,
    };

    enum CHROMA_FORMAT
    {
        CHROMA_MONOCHROME,
        CHROMA_420,
        CHROMA_422,
        CHROMA_444,
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // H264_H__