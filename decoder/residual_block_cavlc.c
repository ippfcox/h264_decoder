#include "residual_block_cavlc.h"
#include "slice_header.h"
#include "mathmatical.h"
#include "chapter6.h"

// 9.2.1 Parsing process for total number of non-zero transform coefficient levels and number of trailing ones
static void TotalCoeff_TrailingOnes_nC(
    struct bit_stream *bs,
    struct slice_header_t *sh,
    struct slice_data_t *sd,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs,
    enum LEVEL_TYPE level_type,
    int blkIdx, int32_t CurrMbAddr,
    int maxNumCoeff, int32_t *totalCoeff, int32_t *trailingOnes, int32_t *nC)
{
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;

    int luma4x4BlkIdx = blkIdx;
    int chroma4x4BlkIdx = blkIdx;
    int cb4x4BlkIdx = blkIdx;
    int cr4x4BlkIdx = blkIdx;

    int32_t mbAddrA = -1, mbAddrB = -1;
    int32_t luma4x4BlkIdxA = -1, luma4x4BlkIdxB = -1;
    int32_t cb4x4BlkIdxA = -1, cb4x4BlkIdxB = -1;
    int32_t cr4x4BlkIdxA = -1, cr4x4BlkIdxB = -1;
    int32_t chroma4x4BlkIdxA = -1, chroma4x4BlkIdxB = -1;
    int32_t availableFlagA = 1, availableFlagB = 1;

    if (level_type == LEVEL_TYPE_ChromaDCLevel)
    {
        if (spsd->ChromaArrayType == 1)
            *nC = -1;
        else if (spsd->ChromaArrayType == 2)
            *nC = -2;
    }
    else
    {
        // 1.
        if (level_type == LEVEL_TYPE_Intra16x16DCLevel)
        {
            luma4x4BlkIdx = 0;
        }
        // 2.
        else if (level_type == LEVEL_TYPE_CbIntra16x16DCLevel)
        {
            cb4x4BlkIdx = 0;
        }
        // 3.
        else if (level_type == LEVEL_TYPE_CrIntra16x16DCLevel)
        {
            cr4x4BlkIdx = 0;
        }
        // 4.
        if (level_type == LEVEL_TYPE_Intra16x16DCLevel || level_type == LEVEL_TYPE_Intra16x16ACLevel || level_type == LEVEL_TYPE_LumaLevel4x4)
        {
            derivation_process_for_neighbouring_4x4_luma_blocks(spsd->MbWidthC, spsd->MbHeightC, sh->MbaffFrameFlag, CurrMbAddr, spsd->PicWidthInMbs, luma4x4BlkIdx, &mbAddrA, &luma4x4BlkIdxA, &mbAddrB, &luma4x4BlkIdxB);
        }
        else if (level_type == LEVEL_TYPE_CbIntra16x16DCLevel || level_type == LEVEL_TYPE_CbIntra16x16ACLevel || level_type == LEVEL_TYPE_CbLevel4x4)
        {
            derivation_process_for_neighbouring_4x4_chroma_blocks_for_ChromaArrayType_equal_to_3(spsd->ChromaArrayType, spsd->MbWidthC, spsd->MbHeightC, sh->MbaffFrameFlag, CurrMbAddr, spsd->PicWidthInMbs, cb4x4BlkIdx, &mbAddrA, &cb4x4BlkIdxA, &mbAddrB, &cb4x4BlkIdxB);
        }
        else if (level_type == LEVEL_TYPE_CrIntra16x16DCLevel || level_type == LEVEL_TYPE_CrIntra16x16ACLevel || level_type == LEVEL_TYPE_CrLevel4x4)
        {
            derivation_process_for_neighbouring_4x4_chroma_blocks_for_ChromaArrayType_equal_to_3(spsd->ChromaArrayType, spsd->MbWidthC, spsd->MbHeightC, sh->MbaffFrameFlag, CurrMbAddr, spsd->PicWidthInMbs, cr4x4BlkIdx, &mbAddrA, &cr4x4BlkIdxA, &mbAddrB, &cr4x4BlkIdxB);
        }
        else if (level_type == LEVEL_TYPE_ChromaACLevel)
        {
            derivation_process_for_neighbouring_4x4_chroma_blocks(spsd->ChromaArrayType, spsd->MbWidthC, spsd->MbHeightC, sh->MbaffFrameFlag, CurrMbAddr, spsd->PicWidthInMbs, chroma4x4BlkIdx, &mbAddrA, &chroma4x4BlkIdxA, &mbAddrB, &chroma4x4BlkIdxB);
        }
        // 5.
    }
}

// static int32_t ce(struct bit_stream *bs, int maxNumCoeff, int blkIdx, int32_t coeffLevel[16])
// {
//     for (int i = 0; i < maxNumCoeff; ++i)
//         coeffLevel[i] = 0;
// }

// 7.3.5.3.2 Residual block CAVLC syntax
static void residual_block_cavlc(
    struct residual_block_cavlc_t *rbc,
    struct bit_stream *bs,
    enum LEVEL_TYPE level_type,
    int32_t coeffLevel[0], int32_t startIdx, int32_t endIdx, int32_t maxNumCoeff)
{
    for (int i = 0; i < maxNumCoeff; ++i)
        coeffLevel[i] = 0;
    // rbc->coeff_token = bs_ce(bs);
    // if ()
}
