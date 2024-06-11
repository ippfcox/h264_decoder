#ifndef H264_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <stdint.h>

    // Table 7-1 - NAL unit type codes
    enum
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

    struct NAL_unit
    {
        // 原始数据
        uint8_t *buffer;
        size_t size;

        size_t NumBytesInNALunit;  // == size
        size_t nalUnitHeaderBytes; // header size;
        uint8_t *rbsp_byte;        // rbsp data
        size_t NumBytesInRBSP;     // rbsp size

        uint8_t forbidden_zero_bit : 1; // f(1)
        uint8_t nal_ref_idc : 2;        // u(2)
        uint8_t nal_unit_type : 5;      // u(5)

        uint8_t svc_extension_flag : 1;    // u(1)
        // [TODO]
        uint8_t avc_3d_extension_flag : 1; // u(1)
        // [TODO]

        uint8_t emulation_prevention_three_byte; // f(8)
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // H264_H__