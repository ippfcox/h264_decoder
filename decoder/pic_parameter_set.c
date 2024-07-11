#include <stdlib.h>
#include "pic_parameter_set.h"
#include "misc.h"
#include "common/log.h"

// 7.3.2.2 Picture parameter set RBSP syntax
// pic_parameter_set_rbsp()
void pic_paramster_set_rbsp(struct pic_parameter_set_rbsp_t *ppsr, struct bit_stream *bs)
{
    ppsr->pic_parameter_set_id = bs_ue(bs);
    ppsr->seq_parameter_set_id = bs_ue(bs);
    ppsr->entropy_coding_mode_flag = bs_u(bs, 1);
    ppsr->bottom_field_pic_order_in_frame_present_flag = bs_u(bs, 1);
    ppsr->num_slice_groups_minus1 = bs_ue(bs);
    if (ppsr->num_slice_groups_minus1 > 0)
    {
        ppsr->slice_group_map_type = bs_ue(bs);
        if (ppsr->slice_group_map_type == 0)
        {
            ppsr->run_length_minus1 = calloc(ppsr->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= ppsr->num_slice_groups_minus1; ++iGroup)
            {
                ppsr->run_length_minus1[iGroup] = bs_ue(bs);
            }
        }
        else if (ppsr->slice_group_map_type == 2)
        {
            ppsr->top_left = calloc(ppsr->num_slice_groups_minus1, sizeof(uint64_t));
            ppsr->bottom_right = calloc(ppsr->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= ppsr->num_slice_groups_minus1; ++iGroup)
            {
                ppsr->top_left[iGroup] = bs_ue(bs);
                ppsr->bottom_right[iGroup] = bs_ue(bs);
            }
        }
        else if (ppsr->slice_group_map_type == 3 ||
                 ppsr->slice_group_map_type == 4 ||
                 ppsr->slice_group_map_type == 5)
        {
            ppsr->slice_group_change_direction_flag = bs_u(bs, 1);
            ppsr->slice_group_change_rate_minus1 = bs_ue(bs);
        }
        else if (ppsr->slice_group_map_type == 6)
        {
            ppsr->pic_size_in_map_units_minus1 = bs_ue(bs);
            int v = get_log2(ppsr->num_slice_groups_minus1 + 1);
            ppsr->slice_group_id = calloc(ppsr->pic_size_in_map_units_minus1, sizeof(uint32_t));
            for (int i = 0; i < ppsr->pic_size_in_map_units_minus1; ++i)
            {
                ppsr->slice_group_id[i] = bs_u(bs, v);
            }
        }
    }
    ppsr->num_ref_idx_l0_default_active_minus1 = bs_ue(bs);
    ppsr->num_ref_idx_l1_default_active_minus1 = bs_ue(bs);
    ppsr->weighted_pred_flag = bs_u(bs, 1);
    ppsr->weighted_bipred_idc = bs_u(bs, 2);
    ppsr->pic_init_qp_minus26 = bs_se(bs);
    ppsr->pic_init_qs_minus26 = bs_se(bs);
    ppsr->chroma_qp_index_offset = bs_se(bs);
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
        ppsr->second_chroma_qp_index_offset = bs_se(bs);
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
#define dump(indents, name, placeholder) fprintf(fp, "%s%s: %" placeholder "\n", make_indents(indents), #name, ppsr->name)

    dump(1, pic_parameter_set_id, "u");
    dump(1, seq_parameter_set_id, "u");
    dump(1, entropy_coding_mode_flag, "u");
    dump(1, bottom_field_pic_order_in_frame_present_flag, "u");
    dump(1, num_slice_groups_minus1, "u");
    dump(2, slice_group_map_type, "u");
    if (ppsr->run_length_minus1)
        for (int iGroup = 0; iGroup <= ppsr->num_slice_groups_minus1; ++iGroup)
            dump(4, run_length_minus1[iGroup], "u");
    if (ppsr->top_left && ppsr->bottom_right)
        for (int iGroup = 0; iGroup <= ppsr->num_slice_groups_minus1; ++iGroup)
        {
            dump(4, top_left[iGroup], "u");
            dump(4, bottom_right[iGroup], "u");
        }
    dump(3, slice_group_change_direction_flag, "u");
    dump(3, slice_group_change_rate_minus1, "u");
    dump(3, pic_size_in_map_units_minus1, "u");
    if (ppsr->slice_group_id)
        for (int i = 0; i < ppsr->pic_size_in_map_units_minus1; ++i)
            dump(4, slice_group_id[i], "u");
    dump(1, num_ref_idx_l0_default_active_minus1, "u");
    dump(1, num_ref_idx_l1_default_active_minus1, "u");
    dump(1, weighted_pred_flag, "u");
    dump(1, weighted_bipred_idc, "u");
    dump(1, pic_init_qp_minus26, "d");
    dump(1, pic_init_qs_minus26, "d");
    dump(1, chroma_qp_index_offset, "d");
    dump(1, deblocking_filter_control_present_flag, "u");
    dump(1, constrained_intra_pred_flag, "u");
    dump(1, redundant_pic_cnt_present_flag, "u");
    dump(2, transform_8x8_mode_flag, "u");
    dump(2, pic_scaling_matrix_present_flag, "u");
    if (ppsr->pic_scaling_list_present_flag)
    {
        // uint8_t *pic_scaling_list_present_flag;
    }
    dump(2, second_chroma_qp_index_offset, "d");

    fprintf(fp, "\n");
}