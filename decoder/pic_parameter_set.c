#include <stdlib.h>
#include "Exp-Golomb.h"
#include "pic_parameter_set.h"
#include "misc.h"
#include "common/log.h"

// 7.3.2.2 Picture parameter set RBSP syntax
// pic_parameter_set_rbsp()
void pic_paramster_set_rbsp(struct pic_parameter_set_rbsp_t *ppsr, struct bit_stream *bs)
{
    ppsr->pic_parameter_set_id = exp_golomb_ue(bs);
    ppsr->seq_parameter_set_id = exp_golomb_ue(bs);
    ppsr->entropy_coding_mode_flag = bs_u(bs, 1);
    ppsr->bottom_field_pic_order_in_frame_present_flag = bs_u(bs, 1);
    ppsr->num_slice_groups_minus1 = exp_golomb_ue(bs);
    if (ppsr->num_slice_groups_minus1 > 0)
    {
        ppsr->slice_group_map_type = exp_golomb_ue(bs);
        if (ppsr->slice_group_map_type == 0)
        {
            ppsr->run_length_minus1 = calloc(ppsr->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= ppsr->num_slice_groups_minus1; ++iGroup)
            {
                ppsr->run_length_minus1[iGroup] = exp_golomb_ue(bs);
            }
        }
        else if (ppsr->slice_group_map_type == 2)
        {
            ppsr->top_left = calloc(ppsr->num_slice_groups_minus1, sizeof(uint64_t));
            ppsr->bottom_right = calloc(ppsr->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= ppsr->num_slice_groups_minus1; ++iGroup)
            {
                ppsr->top_left[iGroup] = exp_golomb_ue(bs);
                ppsr->bottom_right[iGroup] = exp_golomb_ue(bs);
            }
        }
        else if (ppsr->slice_group_map_type == 3 ||
                 ppsr->slice_group_map_type == 4 ||
                 ppsr->slice_group_map_type == 5)
        {
            ppsr->slice_group_change_direction_flag = bs_u(bs, 1);
            ppsr->slice_group_change_rate_minus1 = exp_golomb_ue(bs);
        }
        else if (ppsr->slice_group_map_type == 6)
        {
            ppsr->pic_size_in_map_units_minus1 = exp_golomb_ue(bs);
            int v = get_log2(ppsr->num_slice_groups_minus1 + 1);
            ppsr->slice_group_id = calloc(ppsr->pic_size_in_map_units_minus1 + 1, sizeof(uint32_t));
            for (int i = 0; i <= ppsr->pic_size_in_map_units_minus1; ++i)
            {
                ppsr->slice_group_id[i] = bs_u(bs, v);
            }
        }
    }
    ppsr->num_ref_idx_l0_default_active_minus1 = exp_golomb_ue(bs);
    ppsr->num_ref_idx_l1_default_active_minus1 = exp_golomb_ue(bs);
    ppsr->weighted_pred_flag = bs_u(bs, 1);
    ppsr->weighted_bipred_idc = bs_u(bs, 2);
    ppsr->pic_init_qp_minus26 = exp_golomb_se(bs);
    ppsr->pic_init_qs_minus26 = exp_golomb_se(bs);
    ppsr->chroma_qp_index_offset = exp_golomb_se(bs);
    ppsr->deblocking_filter_control_present_flag = bs_u(bs, 1);
    ppsr->constrained_intra_pred_flag = bs_u(bs, 1);
    ppsr->redundant_pic_cnt_present_flag = bs_u(bs, 1);
    if (bs_more_rbsp_data(bs)) // [TODO]
    {
        ppsr->transform_8x8_mode_flag = bs_u(bs, 1);
        ppsr->pic_scaling_matrix_present_flag = bs_u(bs, 1);
        if (ppsr->pic_scaling_matrix_present_flag)
        {
            // [TODO]
            // uint8_t *pic_scaling_list_present_flag  // u(1)
        }
        ppsr->second_chroma_qp_index_offset = exp_golomb_se(bs);
    }

    ppsr->SliceGroupChangeRate = ppsr->slice_group_change_rate_minus1 + 1;
}

void free_pic_parameter_set(struct pic_parameter_set_rbsp_t *ppsr)
{
    if (!ppsr)
        return;
    if (ppsr->run_length_minus1)
        free(ppsr->run_length_minus1);
    if (ppsr->top_left)
        free(ppsr->top_left);
    if (ppsr->bottom_right)
        free(ppsr->bottom_right);
    if (ppsr->slice_group_id)
        free(ppsr->slice_group_id);
    if (ppsr->pic_scaling_list_present_flag)
        free(ppsr->pic_scaling_list_present_flag);
}

void dump_pic_paramster_set_rbsp(FILE *fp, struct pic_parameter_set_rbsp_t *ppsr)
{
#define dv(indents, placeholder, member) dump_value(indents, placeholder, *ppsr, member)
#define da(indents, placeholder, member, count) dump_array(indents, placeholder, *ppsr, member, count)

    dv(1, "u", pic_parameter_set_id);
    dv(1, "u", seq_parameter_set_id);
    dv(1, "u", entropy_coding_mode_flag);
    dv(1, "u", bottom_field_pic_order_in_frame_present_flag);
    dv(1, "u", num_slice_groups_minus1);
    dv(2, "u", slice_group_map_type);
    da(4, "u", run_length_minus1, ppsr->num_slice_groups_minus1);
    da(4, "u", top_left, ppsr->num_slice_groups_minus1);
    da(4, "u", bottom_right, ppsr->num_slice_groups_minus1);
    dv(3, "u", slice_group_change_direction_flag);
    dv(3, "u", slice_group_change_rate_minus1);
    dv(3, "u", pic_size_in_map_units_minus1);
    da(4, "u", slice_group_id, ppsr->pic_size_in_map_units_minus1);
    dv(1, "u", num_ref_idx_l0_default_active_minus1);
    dv(1, "u", num_ref_idx_l1_default_active_minus1);
    dv(1, "u", weighted_pred_flag);
    dv(1, "u", weighted_bipred_idc);
    dv(1, "d", pic_init_qp_minus26);
    dv(1, "d", pic_init_qs_minus26);
    dv(1, "d", chroma_qp_index_offset);
    dv(1, "u", deblocking_filter_control_present_flag);
    dv(1, "u", constrained_intra_pred_flag);
    dv(1, "u", redundant_pic_cnt_present_flag);
    dv(2, "u", transform_8x8_mode_flag);
    dv(2, "u", pic_scaling_matrix_present_flag);
    if (ppsr->pic_scaling_list_present_flag)
    {
        // uint8_t *pic_scaling_list_present_flag;
    }
    dv(2, "d", second_chroma_qp_index_offset);

    dv(1, "u", SliceGroupChangeRate);
    dv(1, "u", UnusedShortTermFrameNum);
#undef da
#undef dv

    fprintf(fp, "\n");
}