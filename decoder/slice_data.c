#include <stdlib.h>
#include "Exp-Golomb.h"
#include "slice_header.h"
#include "slice_data.h"
#include "macroblock.h"

// 7.3.4 Slice data syntax
void slice_data(
    struct slice_data_t *sd,
    struct bit_stream *bs,
    struct nal_unit_t *nu,
    struct slice_header_t *sh,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs)
{
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;

    sd->mls = calloc(sh->PicSizeInMbs, sizeof(struct macroblock_layer_t));

    if (ppsr->entropy_coding_mode_flag)
    {
        while (!bs_byte_aligned(bs))
        {
            sd->cabac_alignment_one_bit = bs_f(bs, 1);
        }
    }
    int32_t CurrMbAddr = sh->first_mb_in_slice * (1 + sh->MbaffFrameFlag);
    uint8_t moreDataFlag = 1;
    uint32_t prevMbSkipped = 0;
    do
    {
        if (sh->slice_type != H264_SLICE_I && sh->slice_type != H264_SLICE_SI)
        {
            if (!ppsr->entropy_coding_mode_flag)
            {
                sd->mb_skip_run = exp_golomb_ue(bs);
                prevMbSkipped = (sd->mb_skip_run > 0);
                for (int i = 0; i < sd->mb_skip_run; ++i)
                    CurrMbAddr = NextMbAddress(sh, CurrMbAddr);
                if (sd->mb_skip_run > 0)
                {
                    moreDataFlag = bs_more_rbsp_data(bs);
                }
            }
            else
            {
                sd->mb_skip_flag = bs_ae(bs);
                moreDataFlag = !sd->mb_skip_flag;
            }
        }
        if (moreDataFlag)
        {
            if (sh->MbaffFrameFlag &&
                (CurrMbAddr % 2 == 0 || (CurrMbAddr % 2 == 1 && prevMbSkipped)))
            {
                if (ppsr->entropy_coding_mode_flag)
                    sd->mb_field_decoding_flag = bs_ae(bs);
                else
                    sd->mb_field_decoding_flag = exp_golomb_ue(bs);
            }
            macroblock_layer(&sd->mls[CurrMbAddr], bs, sh, sd, spsrs, ppsrs, CurrMbAddr); // [TODO] working in progress, so stop here
            exit(0);
        }
        if (!ppsr->entropy_coding_mode_flag)
        {
            moreDataFlag = bs_more_rbsp_data(bs);
        }
        else
        {
            if (sh->slice_type != H264_SLICE_I && sh->slice_type != H264_SLICE_SI)
            {
                prevMbSkipped = sd->mb_skip_flag;
            }
            if (sh->MbaffFrameFlag && CurrMbAddr % 2 == 0)
            {
                moreDataFlag = 1;
            }
            else
            {
                sd->end_of_slice_flag = bs_ae(bs);
                moreDataFlag = !sd->end_of_slice_flag;
            }
        }
        CurrMbAddr = NextMbAddress(sh, CurrMbAddr);
    }
    while (moreDataFlag);
}