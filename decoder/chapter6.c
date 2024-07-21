#include "chapter6.h"
#include "chapter7.h"
#include "mathmatical.h"
#include "common/log.h"

// 6.4.1 Inverse macroblock scanning process
void inverse_macroblock_scanning_process(
    uint32_t MbaffFrameFlag, uint32_t PicWidthInSamplesL, bool is_frame_macroblock,
    uint32_t mbAddr,
    int32_t *x, int32_t *y)
{
    if (MbaffFrameFlag == 0)
    {
        *x = InverseRasterScan(mbAddr, 16, 16, PicWidthInSamplesL, 0); // (6-3)
        *y = InverseRasterScan(mbAddr, 16, 16, PicWidthInSamplesL, 1); // (6-4)
    }
    else
    {
        int xO = InverseRasterScan(mbAddr / 2, 16, 32, PicWidthInSamplesL, 0); // (6-5)
        int yO = InverseRasterScan(mbAddr / 2, 16, 32, PicWidthInSamplesL, 1); // (6-6)
        if (is_frame_macroblock)
        {
            *x = xO;                     // (6-7)
            *y = yO + (mbAddr % 2) * 16; // (6-8)
        }
        else
        {
            *x = xO;                // (6-9)
            *y = yO + (mbAddr % 2); // (6-10)
        }
    }
}

// [TODO] slice_type
// 6.4.2.1 Inverse macroblock partition scanning process
void inverse_macroblock_partition_scanning_process(
    int32_t mb_type,
    uint32_t mbPartIdx,
    int32_t *x, int32_t *y)
{
    *x = InverseRasterScan(mbPartIdx, MbPartWidth(H264_SLICE_P, mb_type), MbPartHeight(H264_SLICE_P, mb_type), 16, 0); // (6-11)
    *y = InverseRasterScan(mbPartIdx, MbPartWidth(H264_SLICE_P, mb_type), MbPartHeight(H264_SLICE_P, mb_type), 16, 1); // (6-12)
}

// 6.4.2.2 Inverse sub-macroblock partition scanning process
void inverse_sub_macroblock_partition_scanning_process(
    int32_t mb_type, int32_t sub_mb_type[],
    uint32_t mbPartIdx, uint32_t subMbPartIdx,
    int32_t *x, int32_t *y)
{
    switch (mb_type)
    {
    case P_8x8:
    case P_8x8ref0:
        *x = InverseRasterScan(subMbPartIdx,
            SubMbPartWidth(H264_SLICE_P, sub_mb_type[mbPartIdx]),
            SubMbPartHeight(H264_SLICE_P, sub_mb_type[mbPartIdx]), 8, 0); // (6-13)
        *y = InverseRasterScan(subMbPartIdx,
            SubMbPartWidth(H264_SLICE_P, sub_mb_type[mbPartIdx]),
            SubMbPartHeight(H264_SLICE_P, sub_mb_type[mbPartIdx]), 8, 1); // (6-14)
        break;
    case B_8x8:
        *x = InverseRasterScan(subMbPartIdx,
            SubMbPartWidth(H264_SLICE_B, sub_mb_type[mbPartIdx]),
            SubMbPartHeight(H264_SLICE_B, sub_mb_type[mbPartIdx]), 8, 0); // (6-13)
        *y = InverseRasterScan(subMbPartIdx,
            SubMbPartWidth(H264_SLICE_B, sub_mb_type[mbPartIdx]),
            SubMbPartHeight(H264_SLICE_B, sub_mb_type[mbPartIdx]), 8, 1); // (6-14)
        break;
    default:
        *x = InverseRasterScan(subMbPartIdx, 4, 4, 8, 0); // (6-15)
        *y = InverseRasterScan(subMbPartIdx, 4, 4, 8, 1); // (6-16)
    }
}

static void inverse_4x4_block_scaning_process(
    int32_t _4x4BlkIdx,
    int32_t *x, int32_t *y)
{
    *x = InverseRasterScan(_4x4BlkIdx / 4, 8, 8, 16, 0) +
         InverseRasterScan(_4x4BlkIdx % 4, 4, 4, 8, 0); // (6-17)
    *y = InverseRasterScan(_4x4BlkIdx / 4, 8, 8, 16, 1) +
         InverseRasterScan(_4x4BlkIdx % 4, 4, 4, 8, 1); // (6-18)
}

// 6.4.3 Inverse 4x4 luma block scanning process
void inverse_4x4_luma_block_scaning_process(
    int32_t luma4x4BlkIdx,
    int32_t *x, int32_t *y)
{
    inverse_4x4_block_scaning_process(luma4x4BlkIdx, x, y);
}

// 6.4.4 Inverse 4x4 Cb or Cr block scanning process for ChromaArrayType equal to 3
void inverse_4x4_Cb_or_Cr_block_scanning_process_for_ChromaArrayType_equal_to_3(
    uint32_t ChromaArrayType,
    int32_t _4x4BlkIdx,
    int32_t *x, int32_t *y)
{
    if (ChromaArrayType == 3)
        inverse_4x4_block_scaning_process(_4x4BlkIdx, x, y);
}

static void inverse_8x8_block_scanning_process(
    int32_t _8x8BlkIdx,
    int32_t *x, int32_t *y)
{
    *x = InverseRasterScan(_8x8BlkIdx, 8, 8, 16, 0); // (6-19)
    *x = InverseRasterScan(_8x8BlkIdx, 8, 8, 16, 1); // (6-20)
}

// 6.4.5 Inverse 8x8 luma block scanning process
void inverse_8x8_luma_block_scanning_process(
    int32_t luma8x8BlkIdx,
    int32_t *x, int32_t *y)
{
    inverse_8x8_block_scanning_process(luma8x8BlkIdx, x, y);
}

// 6.4.6 Inverse 8x8 Cb or Cr block scanning process for ChromaArrayType equal to 3
void inverse_8x8_Cb_or_Cr_block_scanning_process_for_ChromaArrayType_equal_3(
    uint32_t ChromaArrayType,
    int32_t _8x8BlkIdx,
    int32_t *x, int32_t *y)
{
    if (ChromaArrayType == 3)
        inverse_8x8_block_scanning_process(_8x8BlkIdx, x, y);
}

// 6.4.7 Inverse 4x4 chroma block scanning process
void inverse_4x4_chroma_block_scanning_process(
    int32_t chroma4x4BlkIdx,
    int32_t *x, int32_t *y)
{
    *x = InverseRasterScan(chroma4x4BlkIdx, 4, 4, 8, 0); // (6-21)
    *x = InverseRasterScan(chroma4x4BlkIdx, 4, 4, 8, 1); // (6-22)
}

// 6.4.8 Derivation process of the availability for macroblock addresses
bool derivation_process_of_the_availability_for_macroblock_addresses(
    int32_t CurrMbAddr,
    int32_t mbAddr)
{
    // [TODO] check slice number
    return !(mbAddr < 0 || mbAddr > CurrMbAddr);
}

// 6.4.9 Derivation process for neighbouring macroblock addresses and their availability
void derivation_process_for_neighbouring_macroblock_addresses_and_their_availability(
    uint32_t MbaffFrameFlag, int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t *mbAddrA, int32_t *mbAddrB, int32_t *mbAddrC, int32_t *mbAddrD)
{
    if (MbaffFrameFlag != 0)
        return;

    *mbAddrA = CurrMbAddr - 1;
    *mbAddrB = CurrMbAddr - PicWidthInMbs;
    *mbAddrC = CurrMbAddr - PicWidthInMbs + 1;
    *mbAddrD = CurrMbAddr - PicWidthInMbs - 1;

    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrA) || CurrMbAddr % PicWidthInMbs == 0)
        *mbAddrA = -1;
    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrB))
        *mbAddrB = -1;
    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrC) || (CurrMbAddr + 1) % PicWidthInMbs == 0)
        *mbAddrC = -1;
    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrD) || CurrMbAddr % PicWidthInMbs == 0)
        *mbAddrD = -1;
}

// 6.4.10 Derivation process for neighbouring macroblock addresses and their availability in MBAFF frames
void derivation_process_for_neighbouring_macroblock_addresses_and_their_availability_in_MBAFF_frames(
    uint32_t MbaffFrameFlag, int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t *mbAddrA, int32_t *mbAddrB, int32_t *mbAddrC, int32_t *mbAddrD)
{
    if (MbaffFrameFlag != 1)
        return;

    *mbAddrA = 2 * (CurrMbAddr / 2 - 1);
    *mbAddrB = 2 * (CurrMbAddr / 2 - PicWidthInMbs);
    *mbAddrC = 2 * (CurrMbAddr / 2 - PicWidthInMbs + 1);
    *mbAddrD = 2 * (CurrMbAddr / 2 - PicWidthInMbs - 1);

    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrA) || (CurrMbAddr / 2) % PicWidthInMbs == 0)
        *mbAddrA = -1;
    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrB))
        *mbAddrB = -1;
    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrC) || (CurrMbAddr / 2 + 1) % PicWidthInMbs == 0)
        *mbAddrC = -1;
    if (!derivation_process_of_the_availability_for_macroblock_addresses(CurrMbAddr, *mbAddrD) || (CurrMbAddr / 2) % PicWidthInMbs == 0)
        *mbAddrD = -1;
}

// 6.4.11.1 Derivation process for neighbouring macroblocks
void derivation_process_for_neighbouring_macroblocks(
    bool is_for_luma, int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t *mbAddrA, int32_t *mbAddrB)
{
    int xW, yW;

    int xD = -1;
    int yD = 0;

    derivation_process_for_neightboring_locations(is_for_luma, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xD, yD, mbAddrA, &xW, &yW);

    xD = 0;
    yD = -1;

    derivation_process_for_neightboring_locations(is_for_luma, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xD, yD, mbAddrB, &xW, &yW);
}

static void derivation_process_for_neighbouring_8x8_block(
    bool is_for_luma, int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t _8x8BlkIdx,
    int32_t *mbAddrA, int32_t *_8x8BlkIdxA, int32_t *mbAddrB, int32_t *_8x8BlkIdxB)
{
    int xW, yW;

    // Table 6-2
    int xD = -1;
    int yD = 0;
    int xA = (_8x8BlkIdx % 2) * 8 + xD; // (6-23)
    int yA = (_8x8BlkIdx % 2) * 8 + yD; // (6-24)
    derivation_process_for_neightboring_locations(is_for_luma, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xA, yA, mbAddrA, &xW, &yW);
    if (*mbAddrA < 0)
    {
        *_8x8BlkIdxA = -1;
    }
    else
    {
        // 6.4.13.3
    }

    // Table 6-2
    xD = 0;
    yD = -1;
    int xB = (_8x8BlkIdx % 2) * 8 + xD; // (6-23)
    int yB = (_8x8BlkIdx % 2) * 8 + yD; // (6-24)
    derivation_process_for_neightboring_locations(is_for_luma, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xB, yB, mbAddrB, &xW, &yW);
    if (*mbAddrB < 0)
    {
        *_8x8BlkIdxB = -1;
    }
    else
    {
        // 6.4.13.3
    }
}

// 6.4.11.2 Derivation process for neighbouring 8x8 luma block
void derivation_process_for_neighbouring_8x8_luma_block(
    int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t luma8x8BlkIdx,
    int32_t *mbAddrA, int32_t *luma8x8BlkIdxA, int32_t *mbAddrB, int32_t *luma8x8BlkIdxB)
{
    derivation_process_for_neighbouring_8x8_block(true, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, luma8x8BlkIdx, mbAddrA, luma8x8BlkIdxA, mbAddrB, luma8x8BlkIdxB);
}

// 6.4.11.3 Derivation process for neighbouring 8x8 chroma blocks for ChromaArrayType equal to 3
void derivation_process_for_neighbouting_8x8_chroma_blocks_for_ChromaArrayType_equal_to_3(
    uint32_t ChromaArrayType,
    int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t chroma8x8BlkIdx,
    int32_t *mbAddrA, int32_t *chroma8x8BlkIdxA, int32_t *mbAddrB, int32_t *chroma8x8BlkIdxB)
{
    if (ChromaArrayType == 3)
        derivation_process_for_neighbouring_8x8_block(false, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, chroma8x8BlkIdx, mbAddrA, chroma8x8BlkIdxA, mbAddrB, chroma8x8BlkIdxB);
}

static void derivation_process_for_neighbouting_4x4_blocks(
    bool is_for_luma, int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t _4x4BlkIdx,
    int32_t *mbAddrA, int32_t *_4x4BlkIdxA, int32_t *mbAddrB, int32_t *_4x4BlkIdxB)
{
    int xW, yW;
    int x, y;

    // Table 6-2
    int xD = -1;
    int yD = 0;
    inverse_4x4_luma_block_scaning_process(_4x4BlkIdx, &x, &y);
    int xA = x + xD; // (6-25)
    int yA = y + yD; // (6-26)
    derivation_process_for_neightboring_locations(is_for_luma, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xA, yA, mbAddrA, &xW, &yW);
    if (*mbAddrA < 0)
    {
        *_4x4BlkIdxA = -1;
    }
    else
    {
        // 6.4.13.1
    }

    // Table 6-2
    xD = 0;
    yD = -1;
    inverse_4x4_luma_block_scaning_process(_4x4BlkIdx, &x, &y);
    int xB = x + xD; // (6-25)
    int yB = y + yD; // (6-26)
    derivation_process_for_neightboring_locations(is_for_luma, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xB, yB, mbAddrB, &xW, &yW);
    if (*mbAddrB < 0)
    {
        *_4x4BlkIdxB = -1;
    }
    else
    {
        // 6.4.13.1
    }
}

// 6.4.11.4 Derivation process for neighbouring 4x4 luma blocks
void derivation_process_for_neighbouring_4x4_luma_blocks(
    int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t luma4x4BlkIdx,
    int32_t *mbAddrA, int32_t *luma4x4BlkIdxA, int32_t *mbAddrB, int32_t *luma4x4BlkIdxB)
{
    derivation_process_for_neighbouting_4x4_blocks(true, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, luma4x4BlkIdx, mbAddrA, luma4x4BlkIdxA, mbAddrB, luma4x4BlkIdxB);
}

// 6.4.11.5 Derivation process for neighbouring 4x4 chroma blocks
void derivation_process_for_neighbouring_4x4_chroma_blocks(
    uint32_t ChromaArrayType,
    int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t chroma4x4BlkIdx,
    int32_t *mbAddrA, int32_t *chroma4x4BlkIdxA, int32_t *mbAddrB, int32_t *chroma4x4BlkIdxB)
{
    if (ChromaArrayType != 1 && ChromaArrayType != 2)
        return;

    int xW, yW;
    int x, y;

    // Table 6-2
    int xD = -1;
    int yD = 0;
    inverse_4x4_chroma_block_scanning_process(chroma4x4BlkIdx, &x, &y);
    int xA = x + xD; // (6-27)
    int yA = y + yD; // (6-28)
    derivation_process_for_neightboring_locations(false, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xA, yA, mbAddrA, &xW, &yW);
    if (*mbAddrA < 0)
    {
        *chroma4x4BlkIdxA = -1;
    }
    else
    {
        // 6.4.13.2
    }

    // Table 6-2
    xD = 0;
    yD = -1;
    inverse_4x4_chroma_block_scanning_process(chroma4x4BlkIdx, &x, &y);
    int xB = x + xD; // (6-27)
    int yB = y + yD; // (6-28)
    derivation_process_for_neightboring_locations(false, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xB, yB, mbAddrB, &xW, &yW);
    if (*mbAddrB < 0)
    {
        *chroma4x4BlkIdxB = -1;
    }
    else
    {
        // 6.4.13.2
    }
}

// 6.4.11.6 Derivation process for neighbouring 4x4 chroma blocks for ChromaArrayType equal to 3
void derivation_process_for_neighbouring_4x4_chroma_blocks_for_ChromaArrayType_equal_to_3(
    uint32_t ChromaArrayType,
    int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t chroma4x4BlkIdx,
    int32_t *mbAddrA, int32_t *chroma4x4BlkIdxA, int32_t *mbAddrB, int32_t *chroma4x4BlkIdxB)
{
    if (ChromaArrayType == 3)
        derivation_process_for_neighbouting_4x4_blocks(false, MbWidthC, MbHeightC, MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, chroma4x4BlkIdx, mbAddrA, chroma4x4BlkIdxA, mbAddrB, chroma4x4BlkIdxB);
}
// [TODO] others

// 6.4.12 Derivation process for neighbouring locations
void derivation_process_for_neightboring_locations(
    bool is_for_luma, int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
    int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t xN, int32_t yN,
    int32_t *mbAddrN, int32_t *xW, int32_t *yW)
{
    int32_t maxW = 16, maxH = 16; // (6-31)
    if (!is_for_luma)
    {
        maxW = MbWidthC;  // (6-32)
        maxH = MbHeightC; // (6-33)
    }

    if (MbaffFrameFlag == 0)
    {
        specification_for_neighbouring_locations_in_fields_and_non_MBAFF_frames(MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, xN, yN, maxW, maxH, mbAddrN, xW, yW);
    }
    else
    {
        // 6.4.12.2
        logfatal("MBAFF is not supported");
    }
}

// 6.4.12.1 Specification for neighbouring locations in fields and non-MBAFF frames
void specification_for_neighbouring_locations_in_fields_and_non_MBAFF_frames(
    uint32_t MbaffFrameFlag, int32_t CurrMbAddr, uint32_t PicWidthInMbs,
    int32_t xN, int32_t yN, int32_t maxW, int32_t maxH,
    int32_t *mbAddrN, int32_t *xW, int32_t *yW)
{
    if (MbaffFrameFlag != 0)
        return;

    int32_t mbAddrA = -1, mbAddrB = -1, mbAddrC = -1, mbAddrD = -1;
    derivation_process_for_neighbouring_macroblock_addresses_and_their_availability(MbaffFrameFlag, CurrMbAddr, PicWidthInMbs, &mbAddrA, &mbAddrB, &mbAddrC, &mbAddrD);

    // Table 6-3 – Specification of mbAddrN
    if (xN < 0 && yN < 0)
        *mbAddrN = mbAddrD;
    else if (xN < 0 && (yN >= 0 && yN <= maxH - 1))
        *mbAddrN = mbAddrA;
    else if ((xN >= 0 && xN <= maxW - 1) && yN < 0)
        *mbAddrN = mbAddrB;
    else if ((xN >= 0 && xN <= maxW - 1) && (yN >= 0 && yN <= maxH - 1))
        *mbAddrN = CurrMbAddr;
    else if (xN > maxW - 1 && yN < 0)
        *mbAddrN = mbAddrC;
    else if (xN > maxW - 1 && (yN >= 0 && yN <= maxH - 1))
        *mbAddrN = -1;
    else if (yN > maxH - 1)
        *mbAddrN = -1;

    *xW = (xN + maxW) % maxW; // (6-34)
    *yW = (yN + maxH) % maxH; // (6-35)
}

// 6.4.12.2 Specification for neighbouring locations in MBAFF frames
void specifiction_for_neighbouring_locations_in_MBAFF_frames()
{
    // [TODO]
}