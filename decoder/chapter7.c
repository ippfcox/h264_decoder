#include "chapter7.h"

struct macroblock_types_for_I_slice
{
    uint32_t mb_type;
    enum H264_MB_TYPE_I Name_of_mb_type;
    int32_t transform_size_8x8_flag;
    enum H264_MB_PART_PRED_MODE mb_part_pred_modes[1];
    int32_t Intra16x16PredMode;
    int32_t CodedBlockPatternChroma;
    int32_t CodedBlockPatternLuma;
};

// Table 7-11 – Macroblock types for I slices
struct macroblock_types_for_I_slice macroblock_types_for_I_slices[] = {
    {0, I_NxN, 0, {Intra_4x4}, -1, -1, -1},
    {0, I_NxN, 1, {Intra_8x8}, -1, -1, -1},
    {1, I_16x16_0_0_0, -1, {Intra_16x16}, 0, 0, 0},
    {2, I_16x16_1_0_0, -1, {Intra_16x16}, 1, 0, 0},
    {3, I_16x16_2_0_0, -1, {Intra_16x16}, 2, 0, 0},
    {4, I_16x16_3_0_0, -1, {Intra_16x16}, 3, 0, 0},
    {5, I_16x16_0_1_0, -1, {Intra_16x16}, 0, 1, 0},
    {6, I_16x16_1_1_0, -1, {Intra_16x16}, 1, 1, 0},
    {7, I_16x16_2_1_0, -1, {Intra_16x16}, 2, 1, 0},
    {8, I_16x16_3_1_0, -1, {Intra_16x16}, 3, 1, 0},
    {9, I_16x16_0_2_0, -1, {Intra_16x16}, 0, 2, 0},
    {10, I_16x16_1_2_0, -1, {Intra_16x16}, 1, 2, 0},
    {11, I_16x16_2_2_0, -1, {Intra_16x16}, 2, 2, 0},
    {12, I_16x16_3_2_0, -1, {Intra_16x16}, 3, 2, 0},
    {13, I_16x16_0_0_1, -1, {Intra_16x16}, 0, 0, 15},
    {14, I_16x16_1_0_1, -1, {Intra_16x16}, 1, 0, 15},
    {15, I_16x16_2_0_1, -1, {Intra_16x16}, 2, 0, 15},
    {16, I_16x16_3_0_1, -1, {Intra_16x16}, 3, 0, 15},
    {17, I_16x16_0_1_1, -1, {Intra_16x16}, 0, 1, 15},
    {18, I_16x16_1_1_1, -1, {Intra_16x16}, 1, 1, 15},
    {19, I_16x16_2_1_1, -1, {Intra_16x16}, 2, 1, 15},
    {20, I_16x16_3_1_1, -1, {Intra_16x16}, 3, 1, 15},
    {21, I_16x16_0_2_1, -1, {Intra_16x16}, 0, 2, 15},
    {22, I_16x16_1_2_1, -1, {Intra_16x16}, 1, 2, 15},
    {23, I_16x16_2_2_1, -1, {Intra_16x16}, 2, 2, 15},
    {24, I_16x16_3_2_1, -1, {Intra_16x16}, 3, 2, 15},
    {25, I_PCM, -1, -1, -1, -1, -1},
};

struct macroblock_type_with_value_0_for_SI_slice
{
    int32_t mb_type;
    enum H264_MB_TYPE_SI Name_of_mb_type;
    enum H264_MB_PART_PRED_MODE mb_part_pred_mode;
    int32_t Intra16x16PredMode;
    int32_t CodedBlockPatternChroma;
    int32_t CodedBlockPatternLuma;
};

// Table 7-12 – Macroblock type with value 0 for SI slices
struct macroblock_type_with_value_0_for_SI_slice macroblock_type_with_value_0_for_SI_slices[] = {
    {0, SI, Intra_4x4, -1, -1, -1},
};

struct macroblock_type_values_0_to_4_for_P_and_SP_slice
{
    int32_t mb_type;
    enum H264_MB_TYPE_P Name_of_mb_type;
    int32_t num_mb_part;
    enum H264_MB_PART_PRED_MODE mb_part_pred_modes[2];
    int32_t mb_part_width;
    int32_t mb_part_height;
};

// Table 7-13 – Macroblock type values 0 to 4 for P and SP slices
struct macroblock_type_values_0_to_4_for_P_and_SP_slice macroblock_type_values_0_to_4_for_P_and_SP_slices[] = {
    {0, P_L0_16x16, 1, {Pred_L0, -1}, 16, 16},
    {1, P_L0_L0_16x8, 2, {Pred_L0, Pred_L0}, 16, 8},
    {2, P_L0_L0_8x16, 2, {Pred_L0, Pred_L0}, 8, 16},
    {3, P_8x8, 4, {-1, -1}, 8, 8},
    {4, P_8x8ref0, 4, {-1, -1}, 8, 8},
    {5, P_Skip, 1, {Pred_L0, -1}, 16, 16},
};

struct macroblock_type_values_0_to_22_for_B_slice
{
    int32_t mb_type;
    enum H264_MB_TYPE_B Name_of_mb_type;
    int32_t num_mb_part;
    enum H264_MB_PART_PRED_MODE mb_part_pred_modes[2];
    int32_t mb_part_width;
    int32_t mb_part_height;
};

// Table 7-14 – Macroblock type values 0 to 22 for B slices
struct macroblock_type_values_0_to_22_for_B_slice macroblock_type_values_0_to_22_for_B_slices[] = {
    {0, B_Direct_16x16, -1, {Direct, -1}, 8, 8},
    {1, B_L0_16x16, 1, {Pred_L0, -1}, 16, 16},
    {2, B_L1_16x16, 1, {Pred_L1, -1}, 16, 16},
    {3, B_Bi_16x16, 1, {BiPred, -1}, 16, 16},
    {4, B_L0_L0_16x8, 2, {Pred_L0, Pred_L0}, 16, 8},
    {5, B_L0_L0_8x16, 2, {Pred_L0, Pred_L0}, 8, 16},
    {6, B_L1_L1_16x8, 2, {Pred_L1, Pred_L1}, 16, 8},
    {7, B_L1_L1_8x16, 2, {Pred_L1, Pred_L1}, 8, 16},
    {8, B_L0_L1_16x8, 2, {Pred_L0, Pred_L1}, 16, 8},
    {9, B_L0_L1_8x16, 2, {Pred_L0, Pred_L1}, 8, 16},
    {10, B_L1_L0_16x8, 2, {Pred_L1, Pred_L0}, 16, 8},
    {11, B_L1_L0_8x16, 2, {Pred_L1, Pred_L0}, 8, 16},
    {12, B_L0_Bi_16x8, 2, {Pred_L0, BiPred}, 16, 8},
    {13, B_L0_Bi_8x16, 2, {Pred_L0, BiPred}, 8, 16},
    {14, B_L1_Bi_16x8, 2, {Pred_L1, BiPred}, 16, 8},
    {15, B_L1_Bi_8x16, 2, {Pred_L1, BiPred}, 8, 16},
    {16, B_Bi_L0_16x8, 2, {BiPred, Pred_L0}, 16, 8},
    {17, B_Bi_L0_8x16, 2, {BiPred, Pred_L0}, 8, 16},
    {18, B_Bi_L1_16x8, 2, {BiPred, Pred_L1}, 16, 8},
    {19, B_Bi_L1_8x16, 2, {BiPred, Pred_L1}, 8, 16},
    {20, B_Bi_Bi_16x8, 2, {BiPred, BiPred}, 16, 8},
    {21, B_Bi_Bi_8x16, 2, {BiPred, BiPred}, 8, 16},
    {22, B_8x8, 4, {-1, -1}, 8, 8},
    {23, B_Skip, -1, {Direct, -1}, 8, 8},
};

struct sub_macroblock_types_in_P_macroblock
{
    int32_t sub_mb_type;
    enum H264_SUB_MB_TYPE_P Name_of_sub_mb_type;
    int32_t num_sub_mb_part;
    enum H264_MB_PART_PRED_MODE sub_mb_pred_mode;
    int32_t sub_mb_part_width;
    int32_t sub_mb_part_height;
};

// Table 7-17 – Sub-macroblock types in P macroblocks
struct sub_macroblock_types_in_P_macroblock sub_macroblock_types_in_P_macroblocks[] = {
    {-1, -1, -1, -1, -1, -1},
    {0, P_L0_8x8, 1, Pred_L0, 8, 8},
    {1, P_L0_8x4, 2, Pred_L0, 8, 4},
    {2, P_L0_4x8, 2, Pred_L0, 4, 8},
    {3, P_L0_4x4, 4, Pred_L0, 4, 4},
};

struct sub_macroblock_types_in_B_macroblock
{
    int32_t sub_mb_type;
    enum H264_SUB_MB_TYPE_B Name_of_sub_mb_type;
    int32_t num_sub_mb_part;
    enum H264_MB_PART_PRED_MODE sub_mb_pred_mode;
    int32_t sub_mb_part_width;
    int32_t sub_mb_part_height;
};

// Table 7-18 – Sub-macroblock types in B macroblocks
struct sub_macroblock_types_in_B_macroblock sub_macroblock_types_in_B_macroblocks[] = {
    {-1, -1, 4, Direct, 4, 4},
    {0, B_Direct_8x8, 4, Direct, 4, 4},
    {1, B_L0_8x8, 1, Pred_L0, 8, 8},
    {2, B_L1_8x8, 1, Pred_L1, 8, 8},
    {3, B_Bi_8x8, 1, BiPred, 8, 8},
    {4, B_L0_8x4, 2, Pred_L0, 8, 4},
    {5, B_L0_4x8, 2, Pred_L0, 4, 8},
    {6, B_L1_8x4, 2, Pred_L1, 8, 4},
    {7, B_L1_4x8, 2, Pred_L1, 4, 8},
    {8, B_Bi_8x4, 2, BiPred, 8, 4},
    {9, B_Bi_4x8, 2, BiPred, 4, 8},
    {10, B_L0_4x4, 4, Pred_L0, 4, 4},
    {11, B_L1_4x4, 4, Pred_L1, 4, 4},
    {12, B_Bi_4x4, 4, BiPred, 4, 4},
};

enum H264_MB_PART_PRED_MODE MbPartPredMode(
    int32_t transform_size_8x8_flag, enum H264_SLICE_TYPE slice_type,
    int32_t mb_type, int32_t mbPartIdx)
{
    switch (slice_type)
    {
    case H264_SLICE_I:
        for (int i = 0; i < sizeof(macroblock_types_for_I_slices) / sizeof(struct macroblock_types_for_I_slice); ++i)
        {
            if (mb_type == macroblock_types_for_I_slices[i].mb_type &&
                transform_size_8x8_flag == macroblock_types_for_I_slices[i].transform_size_8x8_flag)
                return macroblock_types_for_I_slices[i].mb_part_pred_modes[0];
        }
        break;
    case H264_SLICE_SI:
        for (int i = 0; i < sizeof(macroblock_type_with_value_0_for_SI_slices) / sizeof(struct macroblock_type_with_value_0_for_SI_slice); ++i)
        {
            if (mb_type == macroblock_type_with_value_0_for_SI_slices[i].mb_type)
                return macroblock_type_with_value_0_for_SI_slices[i].mb_part_pred_mode;
        }
        break;

    case H264_SLICE_P:
    case H264_SLICE_SP:
        for (int i = 0; i < sizeof(macroblock_type_values_0_to_4_for_P_and_SP_slices) / sizeof(struct macroblock_type_values_0_to_4_for_P_and_SP_slice); ++i)
        {
            if (mb_type == macroblock_type_values_0_to_4_for_P_and_SP_slices[i].mb_type)
                return macroblock_type_values_0_to_4_for_P_and_SP_slices[i].mb_part_pred_modes[mbPartIdx];
        }
        break;

    case H264_SLICE_B:
        for (int i = 0; i < sizeof(macroblock_type_values_0_to_22_for_B_slices) / sizeof(struct macroblock_type_values_0_to_22_for_B_slice); ++i)
        {
            if (mb_type == macroblock_type_values_0_to_22_for_B_slices[i].mb_type)
                return macroblock_type_values_0_to_22_for_B_slices[i].mb_part_pred_modes[mbPartIdx];
        }
        break;
    default:
        break;
    }

    return H264_MB_PART_PRED_MODE_TAIL;
}

enum H264_MB_PART_PRED_MODE SubMbPredMode(int32_t sub_my_type)
{
    for (int i = 0; i < sizeof(sub_macroblock_types_in_P_macroblocks) / sizeof(struct sub_macroblock_types_in_P_macroblock); ++i)
    {
        if (sub_my_type == sub_macroblock_types_in_P_macroblocks[i].sub_mb_type)
            return sub_macroblock_types_in_P_macroblocks[i].sub_mb_pred_mode;
    }

    return H264_MB_PART_PRED_MODE_TAIL;
}

int32_t NumMbPart(int32_t mb_type)
{
    for (int i = 0; i < sizeof(macroblock_types_for_I_slices) / sizeof(struct macroblock_types_for_I_slice); ++i)
    {
        if (macroblock_types_for_I_slices[i].mb_type == mb_type)
            return macroblock_type_values_0_to_4_for_P_and_SP_slices[i].num_mb_part;
    }

    return 0;
}

int32_t NumSubMbPart(int32_t sub_my_type)
{
    for (int i = 0; i < sizeof(sub_macroblock_types_in_P_macroblocks) / sizeof(struct sub_macroblock_types_in_P_macroblock); ++i)
    {
        if (sub_macroblock_types_in_P_macroblocks[i].sub_mb_type == sub_my_type)
            return sub_macroblock_types_in_P_macroblocks[i].num_sub_mb_part;
    }

    return 0;
}

int32_t MbPartWidth(
    enum H264_SLICE_TYPE slice_type,
    int32_t mb_type)
{
    switch (slice_type)
    {
    case H264_SLICE_P:
    case H264_SLICE_SP:
        for (int i = 0; i < sizeof(macroblock_type_values_0_to_4_for_P_and_SP_slices) / sizeof(struct macroblock_type_values_0_to_4_for_P_and_SP_slice); ++i)
        {
            if (mb_type == macroblock_type_values_0_to_4_for_P_and_SP_slices[i].mb_type)
                return macroblock_type_values_0_to_4_for_P_and_SP_slices[i].mb_part_width;
        }
    case H264_SLICE_B:
        for (int i = 0; i < sizeof(macroblock_type_values_0_to_22_for_B_slices) / sizeof(struct macroblock_type_values_0_to_22_for_B_slice); ++i)
        {
            if (mb_type == macroblock_type_values_0_to_22_for_B_slices[i].mb_type)
                return macroblock_type_values_0_to_22_for_B_slices[i].mb_part_width;
        }
        break;
    default:
        break;
    }

    return -1;
}

int32_t MbPartHeight(
    enum H264_SLICE_TYPE slice_type,
    int32_t mb_type)
{
    switch (slice_type)
    {
    case H264_SLICE_P:
    case H264_SLICE_SP:
        for (int i = 0; i < sizeof(macroblock_type_values_0_to_4_for_P_and_SP_slices) / sizeof(struct macroblock_type_values_0_to_4_for_P_and_SP_slice); ++i)
        {
            if (mb_type == macroblock_type_values_0_to_4_for_P_and_SP_slices[i].mb_type)
                return macroblock_type_values_0_to_4_for_P_and_SP_slices[i].mb_part_height;
        }
    case H264_SLICE_B:
        for (int i = 0; i < sizeof(macroblock_type_values_0_to_22_for_B_slices) / sizeof(struct macroblock_type_values_0_to_22_for_B_slice); ++i)
        {
            if (mb_type == macroblock_type_values_0_to_22_for_B_slices[i].mb_type)
                return macroblock_type_values_0_to_22_for_B_slices[i].mb_part_height;
        }
        break;
    default:
        break;
    }

    return -1;
}

int32_t SubMbPartWidth(
    enum H264_SLICE_TYPE slice_type,
    int32_t sub_mb_type)
{
    switch (slice_type)
    {
    case H264_SLICE_P:
    case H264_SLICE_SP:
        for (int i = 0; i < sizeof(sub_macroblock_types_in_P_macroblocks) / sizeof(struct sub_macroblock_types_in_P_macroblock); ++i)
        {
            if (sub_mb_type == sub_macroblock_types_in_P_macroblocks[i].sub_mb_type)
                return sub_macroblock_types_in_P_macroblocks[i].sub_mb_part_width;
        }
    case H264_SLICE_B:
        for (int i = 0; i < sizeof(sub_macroblock_types_in_B_macroblocks) / sizeof(struct sub_macroblock_types_in_B_macroblock); ++i)
        {
            if (sub_mb_type == sub_macroblock_types_in_B_macroblocks[i].sub_mb_type)
                return sub_macroblock_types_in_B_macroblocks[i].sub_mb_part_width;
        }
        break;
    default:
        break;
    }

    return -1;
}

int32_t SubMbPartHeight(
    enum H264_SLICE_TYPE slice_type,
    int32_t sub_mb_type)
{
    switch (slice_type)
    {
    case H264_SLICE_P:
    case H264_SLICE_SP:
        for (int i = 0; i < sizeof(sub_macroblock_types_in_P_macroblocks) / sizeof(struct sub_macroblock_types_in_P_macroblock); ++i)
        {
            if (sub_mb_type == sub_macroblock_types_in_P_macroblocks[i].sub_mb_type)
                return sub_macroblock_types_in_P_macroblocks[i].sub_mb_part_width;
        }
    case H264_SLICE_B:
        for (int i = 0; i < sizeof(sub_macroblock_types_in_B_macroblocks) / sizeof(struct sub_macroblock_types_in_B_macroblock); ++i)
        {
            if (sub_mb_type == sub_macroblock_types_in_B_macroblocks[i].sub_mb_type)
                return sub_macroblock_types_in_B_macroblocks[i].sub_mb_part_height;
        }
        break;
    default:
        break;
    }

    return -1;
}