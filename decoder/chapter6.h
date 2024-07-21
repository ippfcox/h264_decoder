#ifndef CHAPTER6_H__
#define CHAPTER6_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"

    // 6.4.1 Inverse macroblock scanning process
    void inverse_macroblock_scanning_process(
        uint32_t MbaffFrameFlag, uint32_t PicWidthInSamplesL, bool is_frame_macroblock,
        uint32_t mbAddr,
        int32_t *x, int32_t *y);
    // 6.4.2.1 Inverse macroblock partition scanning process
    void inverse_macroblock_partition_scanning_process(
        int32_t mb_type,
        uint32_t mbPartIdx,
        int32_t *x, int32_t *y);
    // 6.4.2.2 Inverse sub-macroblock partition scanning process
    void inverse_sub_macroblock_partition_scanning_process(
        int32_t mb_type, int32_t sub_mb_type[],
        uint32_t mbPartIdx, uint32_t subMbPartIdx,
        int32_t *x, int32_t *y);
    // 6.4.3 Inverse 4x4 luma block scanning process
    void inverse_4x4_luma_block_scaning_process(
        int luma4x4BlkIdx,
        int *x, int *y);
    // 6.4.4 Inverse 4x4 Cb or Cr block scanning process for ChromaArrayType equal to 3
    void inverse_4x4_Cb_or_Cr_block_scanning_process_for_ChromaArrayType_equal_to_3(
        uint32_t ChromaArrayType,
        int32_t _4x4BlkIdx,
        int32_t *x, int32_t *y);
    // 6.4.5 Inverse 8x8 luma block scanning process
    void inverse_8x8_luma_block_scanning_process(
        int32_t luma8x8BlkIdx,
        int32_t *x, int32_t *y);
    // 6.4.6 Inverse 8x8 Cb or Cr block scanning process for ChromaArrayType equal to 3
    void inverse_8x8_Cb_or_Cr_block_scanning_process_for_ChromaArrayType_equal_3(
        uint32_t ChromaArrayType,
        int32_t _8x8BlkIdx,
        int32_t *x, int32_t *y);
    // 6.4.7 Inverse 4x4 chroma block scanning process
    void inverse_4x4_chroma_block_scanning_process(
        int32_t chroma4x4BlkIdx,
        int32_t *x, int32_t *y);
    // 6.4.8 Derivation process of the availability for macroblock addresses
    bool derivation_process_of_the_availability_for_macroblock_addresses(
        int32_t CurrMbAddr,
        int32_t mbAddr);
    // 6.4.9 Derivation process for neighbouring macroblock addresses and their availability
    void derivation_process_for_neighbouring_macroblock_addresses_and_their_availability(
        uint32_t MbaffFrameFlag, int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t *mbAddrA, int32_t *mbAddrB, int32_t *mbAddrC, int32_t *mbAddrD);
    // 6.4.10 Derivation process for neighbouring macroblock addresses and their availability in MBAFF frames
    void derivation_process_for_neighbouring_macroblock_addresses_and_their_availability_in_MBAFF_frames(
        uint32_t MbaffFrameFlag, int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t *mbAddrA, int32_t *mbAddrB, int32_t *mbAddrC, int32_t *mbAddrD);
    // 6.4.11.1 Derivation process for neighbouring macroblocks
    void derivation_process_for_neighbouring_macroblocks(
        bool is_for_luma, int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t *mbAddrA, int32_t *mbAddrB);
    // 6.4.11.2 Derivation process for neighbouring 8x8 luma block
    void derivation_process_for_neighbouring_8x8_luma_block(
        int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t luma8x8BlkIdx,
        int32_t *mbAddrA, int32_t *luma8x8BlkIdxA, int32_t *mbAddrB, int32_t *luma8x8BlkIdxB);
    // 6.4.11.3 Derivation process for neighbouring 8x8 chroma blocks for ChromaArrayType equal to 3
    void derivation_process_for_neighbouting_8x8_chroma_blocks_for_ChromaArrayType_equal_to_3(
        uint32_t ChromaArrayType,
        int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t _8x8BlkIdx,
        int32_t *mbAddrA, int32_t *_8x8BlkIdxA, int32_t *mbAddrB, int32_t *_8x8BlkIdxB);
    // 6.4.11.4 Derivation process for neighbouring 4x4 luma blocks
    void derivation_process_for_neighbouring_4x4_luma_blocks(
        int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t luma4x4BlkIdx,
        int32_t *mbAddrA, int32_t *luma4x4BlkIdxA, int32_t *mbAddrB, int32_t *luma4x4BlkIdxB);
    // 6.4.11.5 Derivation process for neighbouring 4x4 chroma blocks
    void derivation_process_for_neighbouring_4x4_chroma_blocks(
        uint32_t ChromaArrayType,
        int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t chroma4x4BlkIdx,
        int32_t *mbAddrA, int32_t *chroma4x4BlkIdxA, int32_t *mbAddrB, int32_t *chroma4x4BlkIdxB);
    // 6.4.11.6 Derivation process for neighbouring 4x4 chroma blocks for ChromaArrayType equal to 3
    void derivation_process_for_neighbouring_4x4_chroma_blocks_for_ChromaArrayType_equal_to_3(
        uint32_t ChromaArrayType,
        int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t chroma4x4BlkIdx,
        int32_t *mbAddrA, int32_t *chroma4x4BlkIdxA, int32_t *mbAddrB, int32_t *chroma4x4BlkIdxB);
    // 6.4.12 Derivation process for neighbouring locations
    void derivation_process_for_neightboring_locations(
        bool is_for_luma, int32_t MbWidthC, int32_t MbHeightC, uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t xN, int32_t yN,
        int32_t *mbAddrN, int32_t *xW, int32_t *yW);
    // 6.4.12.1 Specification for neighbouring locations in fields and non-MBAFF frames
    void specification_for_neighbouring_locations_in_fields_and_non_MBAFF_frames(
        uint32_t MbaffFrameFlag,
        int32_t CurrMbAddr, uint32_t PicWidthInMbs,
        int32_t xN, int32_t yN, int32_t maxW, int32_t maxH,
        int32_t *mbAddrN, int32_t *xW, int32_t *yW);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CHAPTER6_H__