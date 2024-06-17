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
    fprintf(fp, "    pic_parameter_set_rbsp() {\n");
    fprintf(fp, "        pic_parameter_set_id: %u\n", nal->rbsp.pps.pic_parameter_set_id);
    fprintf(fp, "        seq_parameter_set_id: %u\n", nal->rbsp.pps.seq_parameter_set_id);
    fprintf(fp, "        entropy_coding_mode_flag: %u\n", nal->rbsp.pps.entropy_coding_mode_flag);
    fprintf(fp, "        bottom_field_pic_order_in_frame_present_flag: %u\n", nal->rbsp.pps.bottom_field_pic_order_in_frame_present_flag);
    fprintf(fp, "        num_slice_groups_minus1: %u\n", nal->rbsp.pps.num_slice_groups_minus1);
    fprintf(fp, "        if (num_slice_groups_minus1 > 0) {\n");
    if (nal->rbsp.pps.num_slice_groups_minus1 > 0)
    {
        fprintf(fp, "            slice_group_map_type: %u\n", nal->rbsp.pps.slice_group_map_type);
        fprintf(fp, "            if (slice_group_map_type == 0)\n");
        if (nal->rbsp.pps.slice_group_map_type == 0)
        {
            fprintf(fp, "                for(iGroup = 0; iGroup <= num_slice_groups_minus1; iGroup++)\n");
            for (int iGroup = 0; iGroup <= nal->rbsp.pps.num_slice_groups_minus1; iGroup++)
            {
                fprintf(fp, "                    run_length_minus1[%d]: %u\n", iGroup, nal->rbsp.pps.run_length_minus1[iGroup]);
            }
        }
        else
        {
            fprintf(fp, "                N/A\n");
        }
        fprintf(fp, "            else if (slice_group_map_type == 2)\n");
        if (nal->rbsp.pps.slice_group_map_type == 2)
        {
            fprintf(fp, "                for(iGroup = 0; iGroup <= num_slice_groups_minus1; iGroup++) {\n");
            for (int iGroup = 0; iGroup <= nal->rbsp.pps.num_slice_groups_minus1; iGroup++)
            {
                fprintf(fp, "                    top_left[%d]: %u\n", iGroup, nal->rbsp.pps.top_left[iGroup]);
                fprintf(fp, "                    bottom_right[%d]: %u\n", iGroup, nal->rbsp.pps.bottom_right[iGroup]);
            }
            fprintf(fp, "                }\n");
        }
        else
        {
            fprintf(fp, "                N/A\n");
        }
        fprintf(fp, "            else if (slice_group_map_type == 3 ||\n");
        fprintf(fp, "                     slice_group_map_type == 4 ||\n");
        fprintf(fp, "                     slice_group_map_type == 5) {\n");
        if (nal->rbsp.pps.slice_group_map_type == 3 ||
            nal->rbsp.pps.slice_group_map_type == 4 ||
            nal->rbsp.pps.slice_group_map_type == 5)
        {
            fprintf(fp, "                slice_group_change_direction_flag: %u\n", nal->rbsp.pps.slice_group_change_direction_flag);
            fprintf(fp, "                slice_group_change_rate_minus1: %u\n", nal->rbsp.pps.slice_group_change_rate_minus1);
        }
        else
        {
            fprintf(fp, "                N/A\n");
        }
        fprintf(fp, "            } else if (slice_group_map_type == 6) {\n");
        if (nal->rbsp.pps.slice_group_map_type == 6)
        {
            fprintf(fp, "                pic_size_in_map_units_minus1: %u\n", nal->rbsp.pps.pic_size_in_map_units_minus1);
            fprintf(fp, "                for (i = 0; i <= pic_size_in_map_units_minus1; i++)\n");
            for (int i = 0; i <= nal->rbsp.pps.pic_size_in_map_units_minus1; i++)
            {
                fprintf(fp, "                    slice_group_id[%d]: %u\n", i, nal->rbsp.pps.slice_group_id[i]);
            }
        }
        else
        {
            fprintf(fp, "                N/A\n");
        }
        fprintf(fp, "            }\n");
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        }\n");
    fprintf(fp, "        num_ref_idx_l0_default_active_minus1: %u\n", nal->rbsp.pps.num_ref_idx_l0_default_active_minus1);
    fprintf(fp, "        num_ref_idx_l1_default_active_minus1: %u\n", nal->rbsp.pps.num_ref_idx_l1_default_active_minus1);
    fprintf(fp, "        weighted_pred_flag: %d\n", nal->rbsp.pps.weighted_pred_flag);
    fprintf(fp, "        weighted_bipred_idc: %d\n", nal->rbsp.pps.weighted_bipred_idc);
    fprintf(fp, "        pic_init_qp_minus26: %d\n", nal->rbsp.pps.pic_init_qp_minus26);
    fprintf(fp, "        pic_init_qs_minus26: %d\n", nal->rbsp.pps.pic_init_qs_minus26);
    fprintf(fp, "        chroma_qp_index_offset: %d\n", nal->rbsp.pps.chroma_qp_index_offset);
    fprintf(fp, "        deblocking_filter_control_present_flag: %d\n", nal->rbsp.pps.deblocking_filter_control_present_flag);
    fprintf(fp, "        constrained_intra_pred_flag: %d\n", nal->rbsp.pps.constrained_intra_pred_flag);
    fprintf(fp, "        redundant_pic_cnt_present_flag: %d\n", nal->rbsp.pps.redundant_pic_cnt_present_flag);
    fprintf(fp, "        if (more_rbsp_data()) {\n");
    if (more_rbsp_data())
    {
        fprintf(fp, "            [TODO]\n");
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        }\n");
    fprintf(fp, "        rbsp_trailing_bits()\n");
    fprintf(fp, "    }\n\n");
}