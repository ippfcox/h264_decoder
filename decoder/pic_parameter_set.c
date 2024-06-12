#include <stdlib.h>
#include "pic_parameter_set.h"
#include "common/misc.h"
#include "common/log.h"

// 7.3.2.2 Picture parameter set RBSP syntax
// pic_parameter_set_rbsp()
void read_pic_paramster_set_rbsp(struct NAL_unit *nal)
{
    int bit_offset = 0;
    struct pic_parameter_set *pps = &nal->rbsp.pps;

    pps->pic_parameter_set_id = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    pps->seq_parameter_set_id = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    pps->entropy_coding_mode_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    pps->bottom_field_pic_order_in_frame_present_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    pps->num_slice_groups_minus1 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    if (pps->num_slice_groups_minus1 > 0)
    {
        pps->slice_group_map_type = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        if (pps->slice_group_map_type == 0)
        {
            pps->run_length_minus1 = calloc(pps->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= pps->num_slice_groups_minus1; ++iGroup)
            {
                pps->run_length_minus1[iGroup] = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
            }
        }
        else if (pps->slice_group_map_type == 2)
        {
            pps->top_left = calloc(pps->num_slice_groups_minus1, sizeof(uint64_t));
            pps->bottom_right = calloc(pps->num_slice_groups_minus1, sizeof(uint64_t));
            for (int iGroup = 0; iGroup <= pps->num_slice_groups_minus1; ++iGroup)
            {
                pps->top_left[iGroup] = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
                pps->bottom_right[iGroup] = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
            }
        }
        else if (pps->slice_group_map_type == 3 ||
                 pps->slice_group_map_type == 4 ||
                 pps->slice_group_map_type == 5)
        {
            pps->slice_group_change_direction_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
            pps->slice_group_change_rate_minus1 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        }
        else if (pps->slice_group_map_type == 6)
        {
            pps->pic_size_in_map_units_minus1 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
            int v = get_log2(pps->num_slice_groups_minus1 + 1);
            pps->slice_group_id = calloc(pps->pic_size_in_map_units_minus1, sizeof(uint32_t));
            for (int i = 0; i < pps->pic_size_in_map_units_minus1; ++i)
            {
                pps->slice_group_id[i] = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, v);
            }
        }
    }
    pps->num_ref_idx_l0_default_active_minus1 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    pps->num_ref_idx_l1_default_active_minus1 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    pps->weighted_pred_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    pps->weighted_bipred_idc = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 2);
    pps->pic_init_qp_minus26 = read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    pps->pic_init_qs_minus26 = read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    pps->chroma_qp_index_offset = read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    pps->deblocking_filter_control_present_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    pps->constrained_intra_pred_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    pps->redundant_pic_cnt_present_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    if (more_rbsp_data()) // [TODO]
    {
        pps->transform_8x8_mode_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
        pps->pic_scaling_matrix_present_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
        if (pps->pic_scaling_matrix_present_flag)
        {
            // [TODO]
            // uint8_t *pic_scaling_list_present_flag  // u(1)
        }
        pps->second_chroma_qp_index_offset = read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    }

    log_debug("chroma_qp_index_offset: %d, %d", pps->chroma_qp_index_offset, pps->deblocking_filter_control_present_flag);
}