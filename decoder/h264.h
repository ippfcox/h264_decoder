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

    // Table 7-11 – Macroblock types for I slices
    enum H264_MB_TYPE_I
    {
        I_NxN,
        I_16x16_0_0_0,
        I_16x16_1_0_0,
        I_16x16_2_0_0,
        I_16x16_3_0_0,
        I_16x16_0_1_0,
        I_16x16_1_1_0,
        I_16x16_2_1_0,
        I_16x16_3_1_0,
        I_16x16_0_2_0,
        I_16x16_1_2_0,
        I_16x16_2_2_0,
        I_16x16_3_2_0,
        I_16x16_0_0_1,
        I_16x16_1_0_1,
        I_16x16_2_0_1,
        I_16x16_3_0_1,
        I_16x16_0_1_1,
        I_16x16_1_1_1,
        I_16x16_2_1_1,
        I_16x16_3_1_1,
        I_16x16_0_2_1,
        I_16x16_1_2_1,
        I_16x16_2_2_1,
        I_16x16_3_2_1,
        I_PCM,

        H264_MB_TYPE_I_TAIL,
    };

    // Table 7-11 – Macroblock types for I slices
    enum H264_MB_PART_PRED_MODE
    {
        Intra_4x4,
        Intra_8x8,
        Intra_16x16,
        Inter,

        Pred_L0,
        Pred_L1,

        BiPred,

        Direct,

        H264_MB_PART_PRED_MODE_TAIL,
    };

    enum H264_MB_TYPE_SI
    {
        SI,
    };

    // Table 7-13 – Macroblock type values 0 to 4 for P and SP slices
    enum H264_MB_TYPE_P
    {
        P_L0_16x16,
        P_L0_L0_16x8,
        P_L0_L0_8x16,
        P_8x8,
        P_8x8ref0,
        P_Skip,

        H264_MB_TYPE_P_TAIL,
    };

    // Table 7-14 – Macroblock type values 0 to 22 for B slices
    enum H264_MB_TYPE_B
    {
        B_Direct_16x16,
        B_L0_16x16,
        B_L1_16x16,
        B_Bi_16x16,
        B_L0_L0_16x8,
        B_L0_L0_8x16,
        B_L1_L1_16x8,
        B_L1_L1_8x16,
        B_L0_L1_16x8,
        B_L0_L1_8x16,
        B_L1_L0_16x8,
        B_L1_L0_8x16,
        B_L0_Bi_16x8,
        B_L0_Bi_8x16,
        B_L1_Bi_16x8,
        B_L1_Bi_8x16,
        B_Bi_L0_16x8,
        B_Bi_L0_8x16,
        B_Bi_L1_16x8,
        B_Bi_L1_8x16,
        B_Bi_Bi_16x8,
        B_Bi_Bi_8x16,
        B_8x8,
        B_Skip,
    };

    // Table 7-17 – Sub-macroblock types in P macroblocks
    enum H264_SUB_MB_TYPE_P
    {
        P_L0_8x8,
        P_L0_8x4,
        P_L0_4x8,
        P_L0_4x4,
    };

    enum H264_SUB_MB_TYPE_B
    {
        B_Direct_8x8,
        B_L0_8x8,
        B_L1_8x8,
        B_Bi_8x8,
        B_L0_8x4,
        B_L0_4x8,
        B_L1_8x4,
        B_L1_4x8,
        B_Bi_8x4,
        B_Bi_4x8,
        B_L0_4x4,
        B_L1_4x4,
        B_Bi_4x4,
    };

    // -------------------------------------------------------------------------
    enum LEVEL_TYPE
    {
        LEVEL_TYPE_ChromaDCLevel,
        LEVEL_TYPE_ChromaACLevel,
        LEVEL_TYPE_Intra16x16DCLevel,
        LEVEL_TYPE_Intra16x16ACLevel,
        LEVEL_TYPE_LumaLevel4x4,
        LEVEL_TYPE_CbIntra16x16DCLevel,
        LEVEL_TYPE_CbIntra16x16ACLevel,
        LEVEL_TYPE_CbLevel4x4,
        LEVEL_TYPE_CrIntra16x16DCLevel,
        LEVEL_TYPE_CrIntra16x16ACLevel,
        LEVEL_TYPE_CrLevel4x4,
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // H264_H__