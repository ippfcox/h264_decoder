#include <stdlib.h>
#include "pic_parameter_set.h"
#include "common/misc.h"
#include "common/log.h"

#define u_n(n) read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, (n))
#define ue_v() read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)
#define se_v() read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)

// 7.3.2.2 Picture parameter set RBSP syntax
// pic_parameter_set_rbsp()
void read_pic_paramster_set_rbsp(struct NAL_unit *nal)
{
    int bit_offset = 0;
    struct pic_parameter_set *pps = &nal->rbsp.pps;

    pps->pic_parameter_set_id = ue_v();
    pps->seq_parameter_set_id = ue_v();
    pps->entropy_coding_mode_flag = u_n(1);
    pps->bottom_field_pic_order_in_frame_present_flag = u_n(1);
    pps->num_slice_groups_minus1 = ue_v();
    if (pps->num_slice_groups_minus1 > 0)
    {
        pps->slice_group_map_type = ue_v();
        if (pps->slice_group_map_type == 0)
        {
            pps->run_length_minus1 = calloc(pps->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= pps->num_slice_groups_minus1; ++iGroup)
            {
                pps->run_length_minus1[iGroup] = ue_v();
            }
        }
        else if (pps->slice_group_map_type == 2)
        {
            pps->top_left = calloc(pps->num_slice_groups_minus1, sizeof(uint64_t));
            pps->bottom_right = calloc(pps->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= pps->num_slice_groups_minus1; ++iGroup)
            {
                pps->top_left[iGroup] = ue_v();
                pps->bottom_right[iGroup] = ue_v();
            }
        }
        else if (pps->slice_group_map_type == 3 ||
                 pps->slice_group_map_type == 4 ||
                 pps->slice_group_map_type == 5)
        {
            pps->slice_group_change_direction_flag = u_n(1);
            pps->slice_group_change_rate_minus1 = ue_v();
        }
        else if (pps->slice_group_map_type == 6)
        {
            pps->pic_size_in_map_units_minus1 = ue_v();
            int v = get_log2(pps->num_slice_groups_minus1 + 1);
            pps->slice_group_id = calloc(pps->pic_size_in_map_units_minus1, sizeof(uint32_t));
            for (int i = 0; i < pps->pic_size_in_map_units_minus1; ++i)
            {
                pps->slice_group_id[i] = u_n(v);
            }
        }
    }
    pps->num_ref_idx_l0_default_active_minus1 = ue_v();
    pps->num_ref_idx_l1_default_active_minus1 = ue_v();
    pps->weighted_pred_flag = u_n(1);
    pps->weighted_bipred_idc = u_n(2);
    pps->pic_init_qp_minus26 = se_v();
    pps->pic_init_qs_minus26 = se_v();
    pps->chroma_qp_index_offset = se_v();
    pps->deblocking_filter_control_present_flag = u_n(1);
    pps->constrained_intra_pred_flag = u_n(1);
    pps->redundant_pic_cnt_present_flag = u_n(1);
    if (more_rbsp_data()) // [TODO]
    {
        pps->transform_8x8_mode_flag = u_n(1);
        pps->pic_scaling_matrix_present_flag = u_n(1);
        if (pps->pic_scaling_matrix_present_flag)
        {
            // [TODO]
            // uint8_t *pic_scaling_list_present_flag  // u(1)
        }
        pps->second_chroma_qp_index_offset = se_v();
    }
}

void dump_pic_parameter_set(FILE *fp, struct NAL_unit *nal)
{
#define dump(name, placeholder) fprintf(fp, "    %s: %" placeholder "\n", #name, nal->rbsp.pps.name)

    dump(pic_parameter_set_id, "u");
    dump(seq_parameter_set_id, "u");
    dump(entropy_coding_mode_flag, "u");
    dump(bottom_field_pic_order_in_frame_present_flag, "u");
    dump(num_slice_groups_minus1, "u");
    dump(slice_group_map_type, "u");
    if (nal->rbsp.pps.run_length_minus1)
        for (int iGroup = 0; iGroup <= nal->rbsp.pps.num_slice_groups_minus1; ++iGroup)
            dump(run_length_minus1[iGroup], "u");
    if (nal->rbsp.pps.top_left && nal->rbsp.pps.bottom_right)
        for (int iGroup = 0; iGroup <= nal->rbsp.pps.num_slice_groups_minus1; ++iGroup)
        {
            dump(top_left[iGroup], "u");
            dump(bottom_right[iGroup], "u");
        }
    dump(slice_group_change_direction_flag, "u");
    dump(slice_group_change_rate_minus1, "u");
    dump(pic_size_in_map_units_minus1, "u");
    if (nal->rbsp.pps.slice_group_id)
        for (int i = 0; i < nal->rbsp.pps.pic_size_in_map_units_minus1; ++i)
            dump(slice_group_id[i], "u");
    dump(num_ref_idx_l0_default_active_minus1, "u");
    dump(num_ref_idx_l1_default_active_minus1, "u");
    dump(weighted_pred_flag, "u");
    dump(weighted_bipred_idc, "u");
    dump(pic_init_qp_minus26, "d");
    dump(pic_init_qs_minus26, "d");
    dump(chroma_qp_index_offset, "d");
    dump(deblocking_filter_control_present_flag, "u");
    dump(constrained_intra_pred_flag, "u");
    dump(redundant_pic_cnt_present_flag, "u");
    dump(transform_8x8_mode_flag, "u");
    dump(pic_scaling_matrix_present_flag, "u");
    if (nal->rbsp.pps.pic_scaling_list_present_flag)
    {
        // uint8_t *pic_scaling_list_present_flag;
    }
    dump(second_chroma_qp_index_offset, "d");

    fprintf(fp, "\n");
}