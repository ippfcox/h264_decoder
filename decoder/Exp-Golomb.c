#include "Exp-Golomb.h"

// 9.1 Parsing process for Exp-Golomb codes
static uint64_t exp_golomb_codes(struct bit_stream *bs)
{
    // (9-1)
    int leadingZeroBits = -1;
    for (uint8_t b = 0; !b; leadingZeroBits++)
        b = bs_read_bits(bs, 1);
    // (9-2)
    return (1 << leadingZeroBits) - 1 + bs_read_bits(bs, leadingZeroBits);
}

// 9.1 Parsing process for Exp-Golomb codes
uint32_t exp_golomb_ue(struct bit_stream *bs)
{
    return exp_golomb_codes(bs);
}

// 9.1.1 Mapping process for signed Exp-Golomb codes
int32_t exp_golomb_se(struct bit_stream *bs)
{
    uint64_t codeNum = exp_golomb_codes(bs);

    return codeNum % 2 == 0 ? -1 * (codeNum >> 1) : (codeNum + 1) >> 1; // ceil when odd
}

struct codeNum_to_coded_block_pattern
{
    uint64_t codeNum;
    int32_t coded_block_pattern_of_Intra_4x4_or_Intra_8x8;
    int32_t coded_block_pattern_of_Inter;
};

// Table 9-4 – Assignment of codeNum to values of coded_block_pattern for macroblock prediction modes
// (a) ChromaArrayType is equal to 1 or 2
struct codeNum_to_coded_block_pattern patterns_ChromaArrayType_1_or_2[] = {
    {0, 47, 0},
    {1, 31, 16},
    {2, 15, 1},
    {3, 0, 2},
    {4, 23, 4},
    {5, 27, 8},
    {6, 29, 32},
    {7, 30, 3},
    {8, 7, 5},
    {9, 11, 10},
    {10, 13, 12},
    {11, 14, 15},
    {12, 39, 47},
    {13, 43, 7},
    {14, 45, 11},
    {15, 46, 13},
    {16, 16, 14},
    {17, 3, 6},
    {18, 5, 9},
    {19, 10, 31},
    {20, 12, 35},
    {21, 19, 37},
    {22, 21, 42},
    {23, 26, 44},
    {24, 28, 33},
    {25, 35, 34},
    {26, 37, 36},
    {27, 42, 40},
    {28, 44, 39},
    {29, 1, 43},
    {30, 2, 45},
    {31, 4, 46},
    {32, 8, 17},
    {33, 17, 18},
    {34, 18, 20},
    {35, 20, 24},
    {36, 24, 19},
    {37, 6, 21},
    {38, 9, 26},
    {39, 22, 28},
    {40, 25, 23},
    {41, 32, 27},
    {42, 33, 29},
    {43, 34, 30},
    {44, 36, 22},
    {45, 40, 25},
    {46, 38, 38},
    {47, 41, 41},
};

// (b) ChromaArrayType is equal to 0 or 3
struct codeNum_to_coded_block_pattern patterns_ChromaArrayType_0_or_3[] = {
    {0, 15, 0},
    {1, 0, 1},
    {2, 7, 2},
    {3, 11, 4},
    {4, 13, 8},
    {5, 14, 3},
    {6, 3, 5},
    {7, 5, 10},
    {8, 10, 12},
    {9, 12, 15},
    {10, 1, 7},
    {11, 2, 11},
    {12, 4, 13},
    {13, 8, 14},
    {14, 6, 6},
    {15, 9, 9},
};

// 9.1.2 Mapping process for coded block pattern
int32_t exp_golomb_me(struct bit_stream *bs, uint8_t ChromaArrayType, enum H264_MB_PART_PRED_MODE MbPartPredMode)
{
    uint64_t codeNum = exp_golomb_codes(bs);

    switch (ChromaArrayType)
    {
    case 1:
    case 2:
        switch (MbPartPredMode)
        {
        case Intra_4x4:
        case Intra_8x8:
            return patterns_ChromaArrayType_1_or_2[codeNum].coded_block_pattern_of_Intra_4x4_or_Intra_8x8;
        case Inter:
            return patterns_ChromaArrayType_1_or_2[codeNum].coded_block_pattern_of_Inter;
        default:
            return -1;
        }
    case 0:
    case 3:
        switch (MbPartPredMode)
        {
        case Intra_4x4:
        case Intra_8x8:
            return patterns_ChromaArrayType_0_or_3[codeNum].coded_block_pattern_of_Intra_4x4_or_Intra_8x8;
        case Inter:
            return patterns_ChromaArrayType_0_or_3[codeNum].coded_block_pattern_of_Inter;
        default:
            return -1;
        }
    default:
        return -1;
    }
}

// 9.1 Parsing process for Exp-Golomb codes
int32_t exp_golomb_te(struct bit_stream *bs, int x)
{
    if (x > 1)
    {
        return exp_golomb_codes(bs);
    }
    else if (x == 1)
    {
        return !bs_read_bits(bs, 1); // (9-3)
    }
    else
    {
        return 0;
    }
}