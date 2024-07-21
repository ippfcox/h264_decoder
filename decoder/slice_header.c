#include <stdlib.h>
#include "Exp-Golomb.h"
#include "slice_header.h"
#include "misc.h"
#include "mathmatical.h"

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
                rplmm->modification_of_pic_nums_idc = exp_golomb_ue(bs);
                if (rplmm->modification_of_pic_nums_idc == 0 ||
                    rplmm->modification_of_pic_nums_idc == 1)
                    rplmm->abs_diff_pic_num_minus1 = exp_golomb_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 2)
                    rplmm->long_term_pic_num = exp_golomb_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 4 ||
                         rplmm->modification_of_pic_nums_idc == 5)
                    rplmm->abs_diff_view_idx_minus1 = exp_golomb_ue(bs);
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
                rplmm->modification_of_pic_nums_idc = exp_golomb_ue(bs);
                if (rplmm->modification_of_pic_nums_idc == 0 ||
                    rplmm->modification_of_pic_nums_idc == 1)
                    rplmm->abs_diff_pic_num_minus1 = exp_golomb_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 2)
                    rplmm->long_term_pic_num = exp_golomb_ue(bs);
                else if (rplmm->modification_of_pic_nums_idc == 4 ||
                         rplmm->modification_of_pic_nums_idc == 5)
                    rplmm->abs_diff_view_idx_minus1 = exp_golomb_ue(bs);
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
                rplm->modification_of_pic_nums_idc = exp_golomb_ue(bs);
                if (rplm->modification_of_pic_nums_idc == 0 ||
                    rplm->modification_of_pic_nums_idc == 1)
                    rplm->abs_diff_pic_num_minus1 = exp_golomb_ue(bs);
                else if (rplm->modification_of_pic_nums_idc == 2)
                    rplm->long_term_pic_num = exp_golomb_ue(bs);
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
                rplm->modification_of_pic_nums_idc = exp_golomb_ue(bs);
                if (rplm->modification_of_pic_nums_idc == 0 ||
                    rplm->modification_of_pic_nums_idc == 1)
                    rplm->abs_diff_pic_num_minus1 = exp_golomb_ue(bs);
                else if (rplm->modification_of_pic_nums_idc == 2)
                    rplm->long_term_pic_num = exp_golomb_ue(bs);
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
                drpm->memory_management_control_operation = exp_golomb_ue(bs);
                if (drpm->memory_management_control_operation == 1 ||
                    drpm->memory_management_control_operation == 3)
                    drpm->difference_of_pic_nums_minus1 = exp_golomb_ue(bs);
                if (drpm->memory_management_control_operation == 2)
                    drpm->long_term_pic_num = exp_golomb_ue(bs);
                if (drpm->memory_management_control_operation == 3 ||
                    drpm->memory_management_control_operation == 6)
                    drpm->long_term_frame_idx = exp_golomb_ue(bs);
                if (drpm->memory_management_control_operation == 4)
                    drpm->max_long_term_frame_idx_plus1 = exp_golomb_ue(bs);
            }
            while (drpm->memory_management_control_operation != 0);
        }
    }
}

// 8.2.2 Decoding process for macroblock to slice group map
static void macroblock_to_slice_group_map(
    struct slice_header_t *sh,
    struct bit_stream *bs,
    struct nal_unit_t *nu,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs)
{

    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    // check sps id
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;

    int sizeOfUpperLeftGroup = 0;
    if (ppsr->num_slice_groups_minus1 == 1 && (ppsr->slice_group_map_type == 4 || ppsr->slice_group_map_type == 5))
    {
        // (8-14)
        int sizeOfUpperLeftGroup = (ppsr->slice_group_change_direction_flag ? (spsd->PicSizeInMapUnits - sh->MapUnitsInSliceGroup0) : sh->MapUnitsInSliceGroup0);
    }

    sh->mapUnitToSliceGroupMap = calloc(spsd->PicSizeInMapUnits, sizeof(int32_t));
    sh->MbToSliceGroupMap = calloc(sh->PicSizeInMbs, sizeof(int32_t));
    if (ppsr->num_slice_groups_minus1 == 0)
    {
        for (int i = 0; i <= spsd->PicSizeInMapUnits - 1; ++i)
            sh->mapUnitToSliceGroupMap[i] = 0; // (8-15)
    }
    else
    {
        // 8.2.2.1 Specification for interleaved slice group map type
        if (ppsr->slice_group_map_type == 0) // (8-17)
        {
            int i = 0;
            do
            {
                for (int iGroup = 0; iGroup <= ppsr->num_slice_groups_minus1 && i < spsd->PicSizeInMapUnits; i += ppsr->run_length_minus1[iGroup++] + 1)
                {
                    for (int j = 0; j < ppsr->run_length_minus1[iGroup] && i + j < spsd->PicSizeInMapUnits; j++)
                    {
                        sh->mapUnitToSliceGroupMap[i + j] = iGroup;
                    }
                }
            }
            while (i < spsd->PicSizeInMapUnits);
        }
        // 8.2.2.2 Specification for dispersed slice group map type
        else if (ppsr->slice_group_map_type == 1) // (8-18)
        {
            for (int i = 0; i < spsd->PicSizeInMapUnits; i++)
            {
                sh->mapUnitToSliceGroupMap[i] = ((i % spsd->PicWidthInMbs) + (((i / spsd->PicWidthInMbs) * (ppsr->num_slice_groups_minus1 + 1)) / 2)) % (ppsr->num_slice_groups_minus1 + 1);
            }
        }
        // 8.2.2.3 Specification for foreground with left-over slice group map type
        else if (ppsr->slice_group_map_type == 2) // (8-19)
        {
            for (int i = 0; i < spsd->PicSizeInMapUnits; i++)
            {
                sh->mapUnitToSliceGroupMap[i] = ppsr->num_slice_groups_minus1;
            }
            for (int iGroup = ppsr->num_slice_groups_minus1 - 1; iGroup >= 0; iGroup--)
            {
                int yTopLeft = ppsr->top_left[iGroup] / spsd->PicWidthInMbs;
                int xTopLeft = ppsr->top_left[iGroup] % spsd->PicWidthInMbs;
                int yBottomRight = ppsr->bottom_right[iGroup] / spsd->PicWidthInMbs;
                int xBottomRight = ppsr->bottom_right[iGroup] % spsd->PicWidthInMbs;
                for (int y = yTopLeft; y <= yBottomRight; y++)
                {
                    for (int x = xTopLeft; x <= xBottomRight; x++)
                    {
                        sh->mapUnitToSliceGroupMap[y * spsd->PicWidthInMbs + x] = iGroup;
                    }
                }
            }
        }
        // 8.2.2.4 Specification for box-out slice group map types
        else if (ppsr->slice_group_map_type == 3) // (8-20)
        {
            for (int i = 0; i < spsd->PicSizeInMapUnits; i++)
                sh->mapUnitToSliceGroupMap[i] = 1;
            int x = (spsd->PicWidthInMbs - ppsr->slice_group_change_direction_flag) / 2;
            int y = (spsd->PicHeightInMapUnits - ppsr->slice_group_change_direction_flag) / 2;
            int leftBound = x;
            int topBound = y;
            int rightBound = x;
            int bottomBound = y;
            int xDir = ppsr->slice_group_change_direction_flag - 1;
            int yDir = ppsr->slice_group_change_direction_flag;
            int mapUnitVacant = 0;
            for (int k = 0; k < sh->MapUnitsInSliceGroup0; k += mapUnitVacant)
            {
                mapUnitVacant = (sh->mapUnitToSliceGroupMap[y * spsd->PicWidthInMbs + x] == 1);
                if (mapUnitVacant)
                    sh->mapUnitToSliceGroupMap[y * spsd->PicWidthInMbs + x] = 0;
                if (xDir == -1 && x == leftBound)
                {
                    leftBound = Max(leftBound - 1, 0);
                    x = leftBound;
                    xDir = 0;
                    yDir = 2 * ppsr->slice_group_change_direction_flag - 1;
                }
                else if (xDir == 1 && x == rightBound)
                {
                    rightBound = Min(rightBound + 1, spsd->PicWidthInMbs - 1);
                    x = rightBound;
                    xDir = 0;
                    yDir = 1 - 2 * ppsr->slice_group_change_direction_flag;
                }
                else if (yDir == -1 && y == topBound)
                {
                    topBound = Max(topBound - 1, 0);
                    y = topBound;
                    xDir = 1 - 2 * ppsr->slice_group_change_direction_flag;
                    yDir = 0;
                }
                else if (yDir == 1 && y == bottomBound)
                {
                    bottomBound = Min(bottomBound + 1, spsd->PicHeightInMapUnits - 1);
                    y = bottomBound;
                    xDir = 2 * ppsr->slice_group_change_direction_flag - 1;
                    yDir = 0;
                }
                else
                {
                    x = x + xDir;
                    y = y + yDir;
                }
            }
        }
        // 8.2.2.5 Specification for raster scan slice group map types
        else if (ppsr->slice_group_map_type == 4) // (8-21)
        {
            for (int i = 0; i < spsd->PicSizeInMapUnits; i++)
            {
                if (i < sizeOfUpperLeftGroup)
                    sh->mapUnitToSliceGroupMap[i] = ppsr->slice_group_change_direction_flag;
                else
                    sh->mapUnitToSliceGroupMap[i] = 1 - ppsr->slice_group_change_direction_flag;
            }
        }
        // 8.2.2.6 Specification for wipe slice group map types
        else if (ppsr->slice_group_map_type == 5) // (8-22)
        {
            int k = 0;
            for (int j = 0; j < spsd->PicWidthInMbs; j++)
            {
                for (int i = 0; i < spsd->PicHeightInMapUnits; i++)
                {
                    if (k++ < sizeOfUpperLeftGroup)
                        sh->mapUnitToSliceGroupMap[i * spsd->PicWidthInMbs + j] = ppsr->slice_group_change_direction_flag;
                    else
                        sh->mapUnitToSliceGroupMap[i * spsd->PicWidthInMbs + j] == 1 - ppsr->slice_group_change_direction_flag;
                }
            }
        }
        // 8.2.2.7 Specification for explicit slice group map type
        else if (ppsr->slice_group_map_type == 6)
        {
            for (int i = 0; i < spsd->PicSizeInMapUnits; ++i)
                sh->mapUnitToSliceGroupMap[i] = ppsr->slice_group_id[i]; // (8-23)
        }

        // 8.2.2.8 Specification for conversion of map unit to slice group map to macroblock to slice group map
        if (spsd->frame_mbs_only_flag == 1 || sh->field_pic_flag == 1)
        {
            for (int i = 0; i < sh->PicSizeInMbs; ++i)
                sh->MbToSliceGroupMap[i] = sh->mapUnitToSliceGroupMap[i]; // (8-24)
        }
        else if (sh->MbaffFrameFlag == 1)
        {
            for (int i = 0; i < sh->PicSizeInMbs; ++i)
                sh->MbToSliceGroupMap[i] = sh->mapUnitToSliceGroupMap[i / 2]; // (8-25)
        }
        else if (spsd->frame_mbs_only_flag == 0 && spsd->mb_adaptive_frame_field_flag == 0 && sh->field_pic_flag == 0)
        {
            for (int i = 0; i < sh->PicSizeInMbs; ++i)
                sh->MbToSliceGroupMap[i] = sh->mapUnitToSliceGroupMap[(i / (2 * spsd->PicWidthInMbs)) * spsd->PicWidthInMbs + (i % spsd->PicWidthInMbs)]; // (8-26)
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
    sh->first_mb_in_slice = exp_golomb_ue(bs);
    sh->slice_type = exp_golomb_ue(bs);
    if (sh->slice_type >= H264_SLICE_P2)
        sh->slice_type -= H264_SLICE_P2;
    sh->pic_parameter_set_id = exp_golomb_ue(bs);
    // check pps id
    struct pic_parameter_set_rbsp_t *ppsr = &ppsrs[sh->pic_parameter_set_id];
    // check sps id
    struct seq_parameter_set_data_t *spsd = &spsrs[ppsr->seq_parameter_set_id].spsd;
    if (spsd->separate_colour_plane_flag == 1)
    {
        sh->colour_plane_id = bs_u(bs, 2);
    }
    sh->frame_num = bs_u(bs, spsd->log2_max_frame_num_minus4 + 4);
    if (!spsd->frame_mbs_only_flag)
    {
        sh->field_pic_flag = bs_u(bs, 1);
        if (sh->field_pic_flag)
            sh->bottom_field_flag = bs_u(bs, 1);
    }
    if (nu->IdrPicFlag)
        sh->idr_pic_id = exp_golomb_ue(bs);
    if (spsd->pic_order_cnt_type == 0)
    {
        sh->pic_order_cnt_lsb = bs_u(bs, spsd->log2_max_pic_order_cnt_lsb_minus4 + 4);
        if (ppsr->bottom_field_pic_order_in_frame_present_flag && !sh->field_pic_flag)
            sh->delta_pic_order_cnt_bottom = exp_golomb_se(bs);
    }
    if (spsd->pic_order_cnt_type == 1 && !spsd->delta_pic_order_always_zero_flag)
    {
        sh->delta_pic_order_cnt[0] = exp_golomb_se(bs);
        if (ppsr->bottom_field_pic_order_in_frame_present_flag && !sh->field_pic_flag)
            sh->delta_pic_order_cnt[1] = exp_golomb_se(bs);
    }
    if (ppsr->redundant_pic_cnt_present_flag)
        sh->redundant_pic_cnt = exp_golomb_ue(bs);
    if (sh->slice_type == H264_SLICE_B)
        sh->direct_spatial_mv_pred_flag = bs_u(bs, 1);
    if (sh->slice_type == H264_SLICE_P || sh->slice_type == H264_SLICE_SP || sh->slice_type == H264_SLICE_B)
    {
        sh->num_ref_idx_active_override_flag = bs_u(bs, 1);
        if (sh->num_ref_idx_active_override_flag)
        {
            sh->num_ref_idx_l0_active_minus1 = exp_golomb_ue(bs);
            if (sh->slice_type == H264_SLICE_B)
                sh->num_ref_idx_l1_active_minus1 = exp_golomb_ue(bs);
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
        sh->cabac_init_idc = exp_golomb_ue(bs);
    sh->slice_qp_delta = exp_golomb_se(bs);
    if (sh->slice_type == H264_SLICE_SP || sh->slice_type == H264_SLICE_SI)
    {
        if (sh->slice_type == H264_SLICE_SP)
            sh->sp_for_switch_flag = bs_u(bs, 1);
        sh->slice_qs_delta = exp_golomb_se(bs);
    }
    if (ppsr->deblocking_filter_control_present_flag)
    {
        sh->disable_deblocking_filter_idc = exp_golomb_ue(bs);
        if (sh->disable_deblocking_filter_idc != 1)
        {
            sh->slice_alpha_c0_offset_div2 = exp_golomb_se(bs);
            sh->slice_beta_offset_div2 = exp_golomb_se(bs);
        }
    }
    if (ppsr->num_slice_groups_minus1 > 0 &&
        ppsr->slice_group_map_type >= 3 && ppsr->slice_group_map_type <= 5)
    {
        sh->slice_group_change_cycle = bs_u(bs, get_log2(spsd->PicSizeInMapUnits / ppsr->SliceGroupChangeRate + 1));
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
    sh->UnusedShortTermFrameNum = (sh->PrevRefFrameNum + 1) % spsd->MaxFrameNum;
    while (sh->UnusedShortTermFrameNum != sh->frame_num)
        sh->UnusedShortTermFrameNum = (sh->UnusedShortTermFrameNum + 1) % spsd->MaxFrameNum;
    sh->MbaffFrameFlag = (spsd->mb_adaptive_frame_field_flag && !sh->field_pic_flag);
    sh->PicHeightInMbs = spsd->FrameHeightInMbs / (1 + sh->field_pic_flag);
    sh->PicHeightInSamplesL = sh->PicHeightInMbs * 16;
    sh->PicHeightInSamplesC = sh->PicHeightInMbs * spsd->MbHeightC;
    sh->PicSizeInMbs = spsd->PicWidthInMbs * sh->PicHeightInMbs;
    if (sh->field_pic_flag == 0)
    {
        sh->MaxPicNum = spsd->MaxFrameNum;
        sh->CurrPicNum = sh->frame_num;
    }
    else
    {
        sh->MaxPicNum = 2 * spsd->MaxFrameNum;
        sh->CurrPicNum = 2 * sh->frame_num + 1;
    }
    sh->SliceQPY = 26 + ppsr->pic_init_qp_minus26 + sh->slice_qp_delta;
    sh->QSY = 26 + ppsr->pic_init_qs_minus26 + sh->slice_qs_delta;
    sh->FilterOffsetA = sh->slice_alpha_c0_offset_div2 << 1;
    sh->FilterOffsetB = sh->slice_beta_offset_div2 << 1;
    sh->MapUnitsInSliceGroup0 = Min(sh->slice_group_change_cycle * sh->MapUnitsInSliceGroup0, spsd->PicSizeInMapUnits);

    macroblock_to_slice_group_map(sh, bs, nu, spsrs, ppsrs);
}

// (8-16)
int32_t NextMbAddress(struct slice_header_t *sh, int32_t n)
{
    int i = n + 1;
    while (i < sh->PicSizeInMbs && sh->MbToSliceGroupMap[i] != sh->MbToSliceGroupMap[n])
        i++;

    return i;
}

void dump_slice_header(FILE *fp, struct slice_header_t *sh)
{
#define dv(indents, placeholder, member) dump_value(indents, placeholder, *sh, member)
#define da(indents, placeholder, member, count) dump_array(indents, placeholder, *sh, member, count)

    dv(1, "u", first_mb_in_slice);
    dv(1, "u", slice_type);
    dv(1, "u", pic_parameter_set_id);
    dv(2, "u", colour_plane_id);
    dv(1, "u", frame_num);
    dv(2, "u", field_pic_flag);
    dv(3, "u", bottom_field_flag);
    dv(2, "u", idr_pic_id);
    dv(2, "u", pic_order_cnt_lsb);
    dv(3, "d", delta_pic_order_cnt_bottom);
    da(2, "d", delta_pic_order_cnt, 2);
    dv(2, "u", redundant_pic_cnt);
    dv(2, "u", direct_spatial_mv_pred_flag);
    dv(2, "u", num_ref_idx_active_override_flag);
    dv(3, "u", num_ref_idx_l0_active_minus1);
    dv(4, "u", num_ref_idx_l1_active_minus1);

    dv(2, "u", cabac_init_idc);
    dv(1, "d", slice_qp_delta);
    dv(3, "u", sp_for_switch_flag);
    dv(2, "d", slice_qs_delta);
    dv(2, "u", disable_deblocking_filter_idc);
    dv(3, "d", slice_alpha_c0_offset_div2);
    dv(3, "d", slice_beta_offset_div2);
    dv(2, "u", slice_group_change_cycle);

    dv(1, "u", PrevRefFrameNum);
    dv(1, "u", UnusedShortTermFrameNum);
    dv(1, "u", MbaffFrameFlag);
    dv(1, "u", PicHeightInMbs);
    dv(1, "u", PicHeightInSamplesL);
    dv(1, "u", PicHeightInSamplesC);
    dv(1, "u", PicSizeInMbs);
    dv(1, "u", MaxPicNum);
    dv(1, "u", CurrPicNum);
    dv(1, "d", SliceQPY);
    dv(1, "d", QSY);
    dv(1, "u", FilterOffsetA);
    dv(1, "u", FilterOffsetB);
    dv(1, "u", MapUnitsInSliceGroup0);

    fprintf(fp, "\n");
}