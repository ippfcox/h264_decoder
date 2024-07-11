#include "slice_header.h"
#include "misc.h"

// H.7.3.3.1.1 Reference picture list MVC modification syntax
void ref_pic_list_mvc_modification(
    struct ref_pic_list_mvc_modification_t *rplmm,
    struct bit_stream *bs,
    struct slice_header_t *sh)
{
    if (sh->slice_type % 5 != H264_SLICE_I && sh->slice_type != H264_SLICE_SI)
    {
        rplmm->ref_pic_list_modification_flag_l0 = bs_u(bs, 1);
        if (rplmm->ref_pic_list_modification_flag_l0)
        {
            do
            {
                rplmm->modification_of_pic_nums_idc = bs_ue(bs);
                if (rplmm->modification_of_pic_nums_idc == 0 ||
                    rplmm->modification_of_pic_nums_idc == 1)
                    rplmm->abs_diff_pic_num_minus1 = bs_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 2)
                    rplmm->long_term_pic_num = bs_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 4 ||
                         rplmm->modification_of_pic_nums_idc == 5)
                    rplmm->abs_diff_view_idx_minus1 = bs_ue(bs);
            }
            while (rplmm->modification_of_pic_nums_idc != 3);
        }
    }
    if (sh->slice_type % 5 == H264_SLICE_B)
    {
        rplmm->ref_pic_list_modification_flag_l1 = bs_u(bs, 1);
        if (rplmm->ref_pic_list_modification_flag_l1)
        {
            do
            {
                rplmm->modification_of_pic_nums_idc = bs_ue(bs);
                if (rplmm->modification_of_pic_nums_idc == 0 ||
                    rplmm->modification_of_pic_nums_idc == 1)
                    rplmm->abs_diff_pic_num_minus1 = bs_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 2)
                    rplmm->long_term_pic_num = bs_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 4 ||
                         rplmm->modification_of_pic_nums_idc == 5)
                    rplmm->abs_diff_view_idx_minus1 = bs_ue(bs);
            }
            while (rplmm->modification_of_pic_nums_idc != 3);
        }
    }
}

void ref_pic_list_modification(
    struct ref_pic_list_modification_t *rplm,
    struct bit_stream *bs,
    struct slice_header_t *sh)
{
    if (sh->slice_type % 5 != H264_SLICE_I && sh->slice_type != H264_SLICE_SI)
    {
        rplm->ref_pic_list_modification_flag_l0 = bs_u(bs, 1);
        if (rplm->ref_pic_list_modification_flag_l0)
        {
            do
            {
                rplm->modification_of_pic_nums_idc = bs_ue(bs);
                if (rplm->modification_of_pic_nums_idc == 0 ||
                    rplm->modification_of_pic_nums_idc == 1)
                    rplm->abs_diff_pic_num_minus1 = bs_ue(bs);
                else if (rplm->modification_of_pic_nums_idc == 2)
                    rplm->long_term_pic_num = bs_ue(bs);
            }
            while (rplm->modification_of_pic_nums_idc != 3);
        }
    }
    if (sh->slice_type % 5 == H264_SLICE_B)
    {
        rplm->ref_pic_list_modification_flag_l1 = bs_u(bs, 1);
        if (rplm->ref_pic_list_modification_flag_l1)
        {
            do
            {
                rplm->modification_of_pic_nums_idc = bs_ue(bs);
                if (rplm->modification_of_pic_nums_idc == 0 ||
                    rplm->modification_of_pic_nums_idc == 1)
                    rplm->abs_diff_pic_num_minus1 = bs_ue(bs);
                else if (rplm->modification_of_pic_nums_idc == 2)
                    rplm->long_term_pic_num = bs_ue(bs);
            }
            while (rplm->modification_of_pic_nums_idc != 3);
        }
    }
}

// 7.3.3.3 Decoded reference picture marking syntax
static void dec_ref_pic_marking(
    struct dec_ref_pic_marking_t *drpm,
    struct nal_unit_t *nu,
    struct bit_stream *bs)
{
    if (nu->IdrPicFlag)
    {
        drpm->no_output_of_prior_pics_flag = bs_u(bs, 1);
        drpm->long_term_reference_flag = bs_u(bs, 1);
    }
    else
    {
        drpm->adaptive_ref_pic_marking_mode_flag = bs_u(bs, 1);
        if (drpm->adaptive_ref_pic_marking_mode_flag)
        {
            do
            {
                drpm->memory_management_control_operation = bs_ue(bs);
                if (drpm->memory_management_control_operation == 1 ||
                    drpm->memory_management_control_operation == 3)
                    drpm->difference_of_pic_nums_minus1 = bs_ue(bs);
                if (drpm->memory_management_control_operation == 2)
                    drpm->long_term_pic_num = bs_ue(bs);
                if (drpm->memory_management_control_operation == 3 ||
                    drpm->memory_management_control_operation == 6)
                    drpm->long_term_frame_idx = bs_ue(bs);
                if (drpm->memory_management_control_operation == 4)
                    drpm->max_long_term_frame_idx_plus1 = bs_ue(bs);
            }
            while (drpm->memory_management_control_operation != 0);
        }
    }
}

// 7.3.3 Slice header syntax
void slice_header(
    struct slice_header_t *sh,
    struct bit_stream *bs,
    struct nal_unit_t *nu,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs)
{
    sh->first_mb_in_slice = bs_ue(bs);
    sh->slice_type = bs_ue(bs);
    if (sh->slice_type >= H264_SLICE_P2)
        sh->slice_type -= H264_SLICE_P2;
    sh->pic_parameter_set_id = bs_ue(bs);
    // check pps id
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    // check sps id
    struct seq_parameter_set_rbsp_t *spsr = &spsrs[ppsr->seq_parameter_set_id];
    if (spsr->spsd.separate_colour_plane_flag == 1)
    {
        sh->colour_plane_id = bs_u(bs, 2);
    }
    sh->frame_num = bs_u(bs, spsr->spsd.log2_max_frame_num_minus4 + 4);
    if (!spsr->spsd.frame_mbs_only_flag)
    {
        sh->field_pic_flag = bs_u(bs, 1);
        if (sh->field_pic_flag)
            sh->bottom_field_flag = bs_u(bs, 1);
    }
    if (nu->IdrPicFlag)
        sh->idr_pic_id = bs_ue(bs);
    if (spsr->spsd.pic_order_cnt_type == 0)
    {
        sh->pic_order_cnt_lsb = bs_u(bs, spsr->spsd.log2_max_pic_order_cnt_lsb_minus4 + 4);
        if (ppsr->bottom_field_pic_order_in_frame_present_flag && !sh->field_pic_flag)
            sh->delta_pic_order_cnt_bottom = bs_se(bs);
    }
    if (spsr->spsd.pic_order_cnt_type == 1 && !spsr->spsd.delta_pic_order_always_zero_flag)
    {
        sh->delta_pic_order_cnt[0] = bs_se(bs);
        if (ppsr->bottom_field_pic_order_in_frame_present_flag && !sh->field_pic_flag)
            sh->delta_pic_order_cnt[1] = bs_se(bs);
    }
    if (ppsr->redundant_pic_cnt_present_flag)
        sh->redundant_pic_cnt = bs_ue(bs);
    if (sh->slice_type == H264_SLICE_B)
        sh->direct_spatial_mv_pred_flag = bs_u(bs, 1);
    if (sh->slice_type == H264_SLICE_P || sh->slice_type == H264_SLICE_SP || sh->slice_type == H264_SLICE_B)
    {
        sh->num_ref_idx_active_override_flag = bs_u(bs, 1);
        if (sh->num_ref_idx_active_override_flag)
        {
            sh->num_ref_idx_l0_active_minus1 = bs_ue(bs);
            if (sh->slice_type == H264_SLICE_B)
                sh->num_ref_idx_l1_active_minus1 = bs_ue(bs);
        }
    }
    if (nu->nal_unit_type == H264_NAL_EXTEN_SLICE || nu->nal_unit_type == H264_NAL_DEPTH_EXTEN_SLICE)
    {
        ref_pic_list_mvc_modification(&sh->rplmm, bs, sh);
    }
    else
    {
        ref_pic_list_modification(&sh->rplm, bs, sh);
    }
    if ((ppsr->weighted_pred_flag && (sh->slice_type == H264_SLICE_P || sh->slice_type == H264_SLICE_SP)) ||
        (ppsr->weighted_bipred_idc == 1 && sh->slice_type == H264_SLICE_B))
    {
        // [TODO] pred_weight_table()
    }
    if (nu->nal_ref_idc != 0)
    {
        dec_ref_pic_marking(&sh->drpm, nu, bs);
    }
    if (ppsr->entropy_coding_mode_flag && sh->slice_type != H264_SLICE_I && sh->slice_type != H264_SLICE_SI)
        sh->cabac_init_idc = bs_ue(bs);
    sh->slice_qp_delta = bs_se(bs);
    if (sh->slice_type == H264_SLICE_SP || sh->slice_type == H264_SLICE_SI)
    {
        if (sh->slice_type == H264_SLICE_SP)
            sh->sp_for_switch_flag = bs_u(bs, 1);
        sh->slice_qs_delta = bs_se(bs);
    }
    if (ppsr->deblocking_filter_control_present_flag)
    {
        sh->disable_deblocking_filter_idc = bs_ue(bs);
        if (sh->disable_deblocking_filter_idc != 1)
        {
            sh->slice_alpha_c0_offset_div2 = bs_se(bs);
            sh->slice_beta_offset_div2 = bs_se(bs);
        }
    }
    if (ppsr->num_slice_groups_minus1 > 0 &&
        ppsr->slice_group_map_type >= 3 && ppsr->slice_group_map_type <= 5)
    {
        sh->slice_group_change_cycle = bs_u(bs, get_log2(spsr->spsd.PicSizeInMapUnits / ppsr->SliceGroupChangeRate + 1));
    }

    if (nu->nal_unit_type == H264_NAL_IDR_SLICE)
    {
        sh->PrevRefFrameNum = 0;
    }
    else
    {
        // [TODO]8.2.5.2
        sh->PrevRefFrameNum = sh->frame_num;
    }
    sh->UnusedShortTermFrameNum = (sh->PrevRefFrameNum + 1) % spsr->spsd.MaxFrameNum;
    while (sh->UnusedShortTermFrameNum != sh->frame_num)
        sh->UnusedShortTermFrameNum = (sh->UnusedShortTermFrameNum + 1) % spsr->spsd.MaxFrameNum;
    sh->MbaffFrameFlag = (spsr->spsd.mb_adaptive_frame_field_flag && !sh->field_pic_flag);
    sh->PicHeightInMbs = spsr->spsd.FrameHeightInMbs / (1 + sh->field_pic_flag);
    sh->PicHeightInSamplesL = sh->PicHeightInMbs * 16;
    sh->PicHeightInSamplesC = sh->PicHeightInMbs * spsr->spsd.MbHeightC;
    sh->PicSizeInMbs = spsr->spsd.PicWidthInMbs * sh->PicHeightInMbs;
    if (sh->field_pic_flag == 0)
    {
        sh->MaxPicNum = spsr->spsd.MaxFrameNum;
        sh->CurrPicNum = sh->frame_num;
    }
    else
    {
        sh->MaxPicNum = 2 * spsr->spsd.MaxFrameNum;
        sh->CurrPicNum = 2 * sh->frame_num + 1;
    }
    sh->SliceQPY = 26 + ppsr->pic_init_qp_minus26 + sh->slice_qp_delta;
    sh->QSY = 26 + ppsr->pic_init_qs_minus26 + sh->slice_qs_delta;
    sh->FilterOffsetA = sh->slice_alpha_c0_offset_div2 << 1;
    sh->FilterOffsetB = sh->slice_beta_offset_div2 << 1;
    sh->MapUnitsInSliceGroup0 = min(sh->slice_group_change_cycle * sh->MapUnitsInSliceGroup0, spsr->spsd.PicSizeInMapUnits);
}

void dump_slice_header(FILE *fp, struct slice_header_t *sh)
{
#define dump(indents, name, placeholder) fprintf(fp, "%s%s: %" placeholder "\n", make_indents(indents), #name, sh->name)

    dump(1, first_mb_in_slice, "u");
    dump(1, slice_type, "u");
    dump(1, pic_parameter_set_id, "u");
    dump(2, colour_plane_id, "u");
    dump(1, frame_num, "u");
    dump(2, field_pic_flag, "u");
    dump(3, bottom_field_flag, "u");
    dump(2, idr_pic_id, "u");
    dump(2, pic_order_cnt_lsb, "u");
    dump(3, delta_pic_order_cnt_bottom, "d");
    for (int i = 0; i < 2; ++i)
        dump(2 + i, delta_pic_order_cnt[i], "d");
    dump(2, redundant_pic_cnt, "u");
    dump(2, direct_spatial_mv_pred_flag, "u");
    dump(2, num_ref_idx_active_override_flag, "u");
    dump(3, num_ref_idx_l0_active_minus1, "u");
    dump(4, num_ref_idx_l1_active_minus1, "u");

    dump(2, cabac_init_idc, "u");
    dump(1, slice_qp_delta, "d");
    dump(3, sp_for_switch_flag, "u");
    dump(2, slice_qs_delta, "d");
    dump(2, disable_deblocking_filter_idc, "u");
    dump(3, slice_alpha_c0_offset_div2, "d");
    dump(3, slice_beta_offset_div2, "d");
    dump(2, slice_group_change_cycle, "u");

    dump(1, PrevRefFrameNum, "u");
    dump(1, UnusedShortTermFrameNum, "u");
    dump(1, MbaffFrameFlag, "u");
    dump(1, PicHeightInMbs, "u");
    dump(1, PicHeightInSamplesL, "u");
    dump(1, PicHeightInSamplesC, "u");
    dump(1, PicSizeInMbs, "u");
    dump(1, MaxPicNum, "u");
    dump(1, CurrPicNum, "u");
    dump(1, SliceQPY, "d");
    dump(1, QSY, "d");
    dump(1, FilterOffsetA, "u");
    dump(1, FilterOffsetB, "u");
    dump(1, MapUnitsInSliceGroup0, "u");

    fprintf(fp, "\n");
}