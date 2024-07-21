#include "macroblock.h"
#include "Exp-Golomb.h"
#include "chapter7.h"

// 7.3.5.1 Macroblock prediction syntax
void mb_pred(
    struct mb_pred_t *mp,
    struct bit_stream *bs,
    struct macroblock_layer_t *ml,
    struct slice_header_t *sh,
    struct slice_data_t *sd,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs)
{
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;

    enum H264_MB_PART_PRED_MODE MbPartPredMode_0 = MbPartPredMode(ml->transform_size_8x8_flag, H264_SLICE_I, ml->mb_type, 0);

    if (MbPartPredMode_0 == Intra_4x4 ||
        MbPartPredMode_0 == Intra_8x8 ||
        MbPartPredMode_0 == Intra_16x16)
    {
        if (MbPartPredMode_0 == Intra_4x4)
        {
            for (int luma4x4BlkIdx = 0; luma4x4BlkIdx < 16; ++luma4x4BlkIdx)
            {
                if (ppsr->entropy_coding_mode_flag)
                    mp->prev_intra4x4_pred_mode_flag[luma4x4BlkIdx] = bs_ae(bs);
                else
                    mp->prev_intra4x4_pred_mode_flag[luma4x4BlkIdx] = bs_u(bs, 1);
                if (!mp->prev_intra4x4_pred_mode_flag[luma4x4BlkIdx])
                {
                    if (ppsr->entropy_coding_mode_flag)
                        mp->rem_intra4x4_pred_mode[luma4x4BlkIdx] = bs_ae(bs);
                    else
                        mp->rem_intra4x4_pred_mode[luma4x4BlkIdx] = bs_u(bs, 3);
                }
            }
        }
        if (MbPartPredMode_0 == Intra_8x8)
        {
            for (int luma8x8BlkIdx = 0; luma8x8BlkIdx < 4; ++luma8x8BlkIdx)
            {
                if (ppsr->entropy_coding_mode_flag)
                    mp->prev_intra8x8_pred_mode_flag[luma8x8BlkIdx] = bs_ae(bs);
                else
                    mp->prev_intra8x8_pred_mode_flag[luma8x8BlkIdx] = bs_u(bs, 1);
                if (!mp->prev_intra8x8_pred_mode_flag[luma8x8BlkIdx])
                {
                    if (ppsr->entropy_coding_mode_flag)
                        mp->rem_intra8x8_pred_mode[luma8x8BlkIdx] = bs_ae(bs);
                    else
                        mp->rem_intra8x8_pred_mode[luma8x8BlkIdx] = bs_u(bs, 3);
                }
            }
        }
        if (spsd->ChromaArrayType == 1 || spsd->ChromaArrayType == 2)
        {
            if (ppsr->entropy_coding_mode_flag)
                mp->intra_chroma_pred_mode = bs_ae(bs);
            else
                mp->intra_chroma_pred_mode = exp_golomb_ue(bs);
        }
    }
    else if (MbPartPredMode_0 != Direct)
    {
        for (int mbPartIdx = 0; mbPartIdx < NumMbPart(ml->mb_type); ++mbPartIdx)
        {
            if ((sh->num_ref_idx_l0_active_minus1 > 0 ||
                    sd->mb_field_decoding_flag != sh->field_pic_flag) &&
                MbPartPredMode(ml->transform_size_8x8_flag, H264_SLICE_I, ml->mb_type, mbPartIdx) != Pred_L1)
            {
                if (ppsr->entropy_coding_mode_flag)
                {
                    mp->ref_idx_l0[mbPartIdx] = bs_ae(bs);
                }
                else
                {
                    int range = 0;
                    if (sh->MbaffFrameFlag == 0 || sd->mb_field_decoding_flag == 0)
                        range = sh->num_ref_idx_l0_active_minus1;
                    if (sh->MbaffFrameFlag == 1 && sd->mb_field_decoding_flag == 1)
                        range = 2 * sh->num_ref_idx_l0_active_minus1 + 1;
                    mp->ref_idx_l0[mbPartIdx] = exp_golomb_te(bs, range);
                }
            }
        }
        for (int mbPartIdx = 0; mbPartIdx < NumMbPart(ml->mb_type); ++mbPartIdx)
        {
            if ((sh->num_ref_idx_l1_active_minus1 > 0 ||
                    sd->mb_field_decoding_flag != sh->field_pic_flag) &&
                MbPartPredMode(ml->transform_size_8x8_flag, H264_SLICE_I, ml->mb_type, mbPartIdx) != Pred_L0)
            {
                if (ppsr->entropy_coding_mode_flag)
                {
                    mp->ref_idx_l1[mbPartIdx] = bs_ae(bs);
                }
                else
                {
                    int range = 0;
                    if (sh->MbaffFrameFlag == 0 || sd->mb_field_decoding_flag == 0)
                        range = sh->num_ref_idx_l1_active_minus1;
                    if (sh->MbaffFrameFlag == 1 && sd->mb_field_decoding_flag == 1)
                        range = 2 * sh->num_ref_idx_l1_active_minus1 + 1;
                    mp->ref_idx_l1[mbPartIdx] = exp_golomb_te(bs, range);
                }
            }
        }
        for (int mbPartIdx = 0; mbPartIdx < NumMbPart(ml->mb_type); ++mbPartIdx)
        {
            if (MbPartPredMode(ml->transform_size_8x8_flag, H264_SLICE_I, ml->mb_type, mbPartIdx) != Pred_L1)
            {
                for (int compIdx = 0; compIdx < 2; ++compIdx)
                {
                    if (ppsr->entropy_coding_mode_flag)
                        mp->mvd_l0[mbPartIdx][0][compIdx] = bs_ae(bs);
                    else
                        mp->mvd_l0[mbPartIdx][0][compIdx] = exp_golomb_se(bs);
                }
            }
        }
        for (int mbPartIdx = 0; mbPartIdx < NumMbPart(ml->mb_type); ++mbPartIdx)
        {
            if (MbPartPredMode(ml->transform_size_8x8_flag, H264_SLICE_I, ml->mb_type, mbPartIdx) != Pred_L0)
            {
                for (int compIdx = 0; compIdx < 2; ++compIdx)
                {
                    if (ppsr->entropy_coding_mode_flag)
                        mp->mvd_l1[mbPartIdx][0][compIdx] = bs_ae(bs);
                    else
                        mp->mvd_l1[mbPartIdx][0][compIdx] = exp_golomb_se(bs);
                }
            }
        }
    }
}

// 7.3.5.2 Sub-macroblock prediction syntax
void sub_mb_pred(
    struct sub_mb_pred_t *smp,
    struct bit_stream *bs,
    struct macroblock_layer_t *ml,
    struct slice_header_t *sh,
    struct slice_data_t *sd,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs)
{
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;

    for (int mbPartIdx = 0; mbPartIdx < 4; ++mbPartIdx)
    {
        if (ppsr->entropy_coding_mode_flag)
            smp->sub_mb_type[mbPartIdx] = bs_ae(bs);
        else
            smp->sub_mb_type[mbPartIdx] = exp_golomb_ue(bs);
    }
    for (int mbPartIdx = 0; mbPartIdx < 4; ++mbPartIdx)
    {
        if ((sh->num_ref_idx_l0_active_minus1 > 0 ||
                sd->mb_field_decoding_flag != sh->field_pic_flag) &&
            ml->mb_type != P_8x8ref0 &&
            smp->sub_mb_type[mbPartIdx] != B_Direct_8x8 &&
            SubMbPredMode(smp->sub_mb_type[mbPartIdx]) != Pred_L1)
        {
            if (ppsr->entropy_coding_mode_flag)
            {
                smp->ref_idx_l0[mbPartIdx] = bs_ae(bs);
            }
            else
            {
                int range = 0;
                if (sh->MbaffFrameFlag == 0 || sd->mb_field_decoding_flag == 0)
                    range = sh->num_ref_idx_l0_active_minus1;
                if (sh->MbaffFrameFlag == 1 && sd->mb_field_decoding_flag == 1)
                    range = 2 * sh->num_ref_idx_l0_active_minus1 + 1;
                smp->ref_idx_l0[mbPartIdx] = exp_golomb_te(bs, range);
            }
        }
    }
    for (int mbPartIdx = 0; mbPartIdx < 4; ++mbPartIdx)
    {
        if ((sh->num_ref_idx_l1_active_minus1 > 0 ||
                sd->mb_field_decoding_flag != sh->field_pic_flag) &&
            smp->sub_mb_type[mbPartIdx] != B_Direct_8x8 &&
            SubMbPredMode(smp->sub_mb_type[mbPartIdx]) != Pred_L0)
        {
            if (ppsr->entropy_coding_mode_flag)
            {
                smp->ref_idx_l1[mbPartIdx] = bs_ae(bs);
            }
            else
            {
                int range = 0;
                if (sh->MbaffFrameFlag == 0 || sd->mb_field_decoding_flag == 0)
                    range = sh->num_ref_idx_l1_active_minus1;
                if (sh->MbaffFrameFlag == 1 && sd->mb_field_decoding_flag == 1)
                    range = 2 * sh->num_ref_idx_l1_active_minus1 + 1;
                smp->ref_idx_l1[mbPartIdx] = exp_golomb_te(bs, range);
            }
        }
    }
    for (int mbPartIdx = 0; mbPartIdx < 4; ++mbPartIdx)
    {
        if (smp->sub_mb_type[mbPartIdx] != B_Direct_8x8 &&
            SubMbPredMode(smp->sub_mb_type[mbPartIdx]) != Pred_L1)
        {
            for (int subMbPartIdx = 0; subMbPartIdx < NumSubMbPart(smp->sub_mb_type[mbPartIdx]); ++subMbPartIdx)
            {
                for (int compIdx = 0; compIdx < 2; ++compIdx)
                {
                    if (ppsr->entropy_coding_mode_flag)
                        smp->mvd_l0[mbPartIdx][subMbPartIdx][compIdx] = bs_ae(bs);
                    else
                        smp->mvd_l0[mbPartIdx][subMbPartIdx][compIdx] = exp_golomb_se(bs);
                }
            }
        }
    }
    for (int mbPartIdx = 0; mbPartIdx < 4; ++mbPartIdx)
    {
        if (smp->sub_mb_type[mbPartIdx] != B_Direct_8x8 &&
            SubMbPredMode(smp->sub_mb_type[mbPartIdx]) != Pred_L0)
        {
            for (int subMbPartIdx = 0; subMbPartIdx < NumSubMbPart(smp->sub_mb_type[mbPartIdx]); ++subMbPartIdx)
            {
                for (int compIdx = 0; compIdx < 2; ++compIdx)
                {
                    if (ppsr->entropy_coding_mode_flag)
                        smp->mvd_l1[mbPartIdx][subMbPartIdx][compIdx] = bs_ae(bs);
                    else
                        smp->mvd_l1[mbPartIdx][subMbPartIdx][compIdx] = exp_golomb_se(bs);
                }
            }
        }
    }
}

// 7.3.5.3.1 Residual luma syntax
static void residual_luma(
    struct residual_luma_t *rl,
    struct bit_stream *bs,
    struct slice_header_t *sh,
    struct macroblock_layer_t *ml,
    struct pic_parameter_set_rbsp_t *ppsrs,
    int32_t i16x16DClevel[16][16], int32_t i16x16AClevel[16][16], int32_t level4x4[16][16], int32_t level8x8[4][64],
    int32_t startIdx, int32_t endIdx)
{
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];

    enum H264_MB_PART_PRED_MODE MbPartPredMode_0 = MbPartPredMode(ml->transform_size_8x8_flag, H264_SLICE_I, ml->mb_type, 0);

    if (startIdx == 0 && MbPartPredMode_0 == Intra_16x16)
    {
        // residual_block()
    }
    for (int i8x8 = 0; i8x8 < 4; ++i8x8)
    {
        if (ml->transform_size_8x8_flag || !ppsr->entropy_coding_mode_flag)
        {
            for (int i4x4 = 0; i4x4 < 4; ++i4x4)
            {
                if (ml->CodedBlockPatternLuma & (1 << i8x8))
                {
                    if (MbPartPredMode_0 == Intra_16x16)
                    {
                        // residual_block()
                    }
                    else
                    {
                        // residual_block()
                    }
                }
                else if (MbPartPredMode_0 == Intra_16x16)
                {
                    for (int i = 0; i < 15; ++i)
                    {
                        i16x16AClevel[i8x8 * 4 + i4x4][i] = 0;
                    }
                }
                else
                {
                    for (int i = 0; i < 16; ++i)
                    {
                        level4x4[i8x8 * 4 + i4x4][i] = 0;
                    }
                }
                if (!ppsr->entropy_coding_mode_flag && ml->transform_size_8x8_flag)
                {
                    for (int i = 0; i < 16; ++i)
                    {
                        level8x8[i8x8][4 * i + i4x4] = level4x4[i8x8 * 4 + i4x4][i];
                    }
                }
            }
        }
        else if (ml->CodedBlockPatternLuma & (1 << i8x8))
        {
            // residual_block()
        }
        else
        {
            for (int i = 0; i < 64; ++i)
            {
                level8x8[i8x8][i] = 0;
            }
        }
    }
}

// 7.3.5.3 Residual data syntax
static void residual(
    struct residual_t *r,
    struct bit_stream *bs,
    struct slice_header_t *sh,
    struct macroblock_layer_t *ml,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs,
    int32_t CurrMbAddr,
    int32_t startIdx, int32_t endIdx)
{
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;

    // if (!ppsr->entropy_coding_mode_flag)
    //     residual_block = residual_block_cavlc;
    // else
    //     residual_block = residual_block_acbac;

    residual_luma(&r->rl, bs, sh, ml, ppsrs,
        r->Intra16x16ACLevel, r->Intra16x16DCLevel,
        r->LumaLevel4x4, r->LumaLevel8x8,
        startIdx, endIdx);
    if (spsd->ChromaArrayType == 1 || spsd->ChromaArrayType == 2)
    {
        int32_t NumC8x8 = 4 / (spsd->SubWidthC * spsd->SubHeightC);
        for (int iCbCr = 0; iCbCr < 2; ++iCbCr)
        {
            if ((ml->CodedBlockPatternChroma & 3) && startIdx == 0) /* chroma DC residual present */
            {
                // residual_block()
            }
            else
            {
                for (int i = 0; i < 4 * NumC8x8; ++i)
                {
                    r->ChromaDCLevel[iCbCr][i] = 0;
                }
            }
        }
        for (int iCbCr = 0; iCbCr < 2; ++iCbCr)
        {
            for (int i8x8 = 0; i8x8 < NumC8x8; ++i8x8)
            {
                for (int i4x4 = 0; i4x4 < 4; ++i4x4)
                {
                    if (ml->CodedBlockPatternLuma & 2) /* chroma AC residual present */
                    {
                        // residual_block()
                    }
                    else
                    {
                        for (int i = 0; i < 15; ++i)
                        {
                            r->ChromaACLevel[iCbCr][i8x8 * 4 + i4x4][i] = 0;
                        }
                    }
                }
            }
        }
    }
    else if (spsd->ChromaArrayType == 3)
    {
        residual_luma(&r->rl, bs, sh, ml, ppsrs,
            r->CbIntra16x16ACLevel, r->CbIntra16x16DCLevel,
            r->CbLevel4x4, r->CbLevel8x8,
            startIdx, endIdx);
        residual_luma(&r->rl, bs, sh, ml, ppsrs,
            r->CrIntra16x16ACLevel, r->CrIntra16x16DCLevel,
            r->CrLevel4x4, r->CrLevel8x8,
            startIdx, endIdx);
    }
}

// 7.3.5 Macroblock layer syntax
void macroblock_layer(
    struct macroblock_layer_t *ml,
    struct bit_stream *bs,
    struct slice_header_t *sh,
    struct slice_data_t *sd,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs,
    int32_t CurrMbAddr)
{
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;

    enum H264_MB_PART_PRED_MODE MbPartPredMode_0 = MbPartPredMode(ml->transform_size_8x8_flag, H264_SLICE_I, ml->mb_type, 0);

    if (ppsr->entropy_coding_mode_flag)
        ml->mb_type = bs_ae(bs);
    else
        ml->mb_type = exp_golomb_ue(bs);
    if (ml->mb_type == I_PCM)
    {
        while (!bs_byte_aligned(bs))
            ml->pcm_alignment_zero_bit = bs_f(bs, 1);
        for (int i = 0; i < 256; ++i)
            ml->pcm_sample_luma[i] = bs_u(bs, spsd->BitDepthY);
        for (int i = 0; i < spsd->MbWidthC * spsd->MbHeightC; ++i)
            ml->pcm_sample_chroma[i] = bs_u(bs, spsd->BitDepthC);
    }
    else
    {
        uint8_t noSubMbPartSizeLessThan8x8Flag = 1;
        if (ml->mb_type != I_NxN &&
            MbPartPredMode_0 != Intra_16x16 &&
            NumMbPart(ml->mb_type) == 4)
        {
            sub_mb_pred(&ml->smp, bs, ml, sh, sd, spsrs, ppsrs);
            for (int mbPartIdx = 0; mbPartIdx < 4; ++mbPartIdx)
            {
                if (ml->smp.sub_mb_type[mbPartIdx] != B_Direct_8x8)
                {
                    if (NumSubMbPart(ml->smp.sub_mb_type[mbPartIdx]) > 1)
                        noSubMbPartSizeLessThan8x8Flag = 0;
                }
                else if (!spsd->direct_8x8_inference_flag)
                {
                    noSubMbPartSizeLessThan8x8Flag = 0;
                }
            }
        }
        else
        {
            if (ml->transform_size_8x8_flag && ml->mb_type == I_NxN)
            {
                if (ppsr->entropy_coding_mode_flag)
                    ml->transform_size_8x8_flag = bs_ae(bs);
                else
                    ml->transform_size_8x8_flag = bs_u(bs, 1);
            }
            mb_pred(&ml->mp, bs, ml, sh, sd, spsrs, ppsrs);
        }
        if (MbPartPredMode_0 != Intra_16x16)
        {
            if (ppsr->entropy_coding_mode_flag)
                ml->coded_block_pattern = bs_ae(bs);
            else
                ml->coded_block_pattern = exp_golomb_me(bs, spsd->ChromaArrayType, MbPartPredMode_0);
            ml->CodedBlockPatternLuma = ml->coded_block_pattern % 16;
            ml->CodedBlockPatternChroma = ml->coded_block_pattern / 16;
            if (ml->CodedBlockPatternLuma > 0 &&
                ml->transform_size_8x8_flag && ml->mb_type != I_NxN &&
                noSubMbPartSizeLessThan8x8Flag &&
                (ml->mb_type != B_Direct_16x16 || spsd->direct_8x8_inference_flag))
            {
                if (ppsr->entropy_coding_mode_flag)
                    ml->transform_size_8x8_flag = bs_ae(bs);
                else
                    ml->transform_size_8x8_flag = bs_u(bs, 1);
            }
        }
        if (ml->CodedBlockPatternLuma > 0 || ml->CodedBlockPatternChroma > 0 ||
            MbPartPredMode_0 != Intra_16x16)
        {
            if (ppsr->entropy_coding_mode_flag)
                ml->mb_qp_delta = bs_ae(bs);
            else
                ml->mb_qp_delta = exp_golomb_se(bs);
            residual(&ml->r, bs, sh, ml, spsrs, ppsrs, CurrMbAddr, 0, 15);
        }
    }
}