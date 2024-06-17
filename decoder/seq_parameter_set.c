#include <stdlib.h>
#include "seq_parameter_set.h"
#include "common/misc.h"
#include "common/log.h"

#define u_n(n) read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, (n))
#define ue_v() read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)
#define se_v() read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)

// 7.3.2.1 Sequence parameter set RBSP syntax
// seq_parameter_set_rbsp()
void read_seq_parameter_set_rbsp(struct NAL_unit *nal)
{
    read_seq_parameter_set(nal);
}

// 7.3.2.1.1 Sequence parameter set data syntax
// seq_parameter_set_data()
void read_seq_parameter_set(struct NAL_unit *nal)
{
    int bit_offset = 0;
    struct seq_parameter_set *sps = &nal->rbsp.sps;
    sps->profile_idc = u_n(8);
    sps->constraint_set0_flag = u_n(1);
    sps->constraint_set1_flag = u_n(1);
    sps->constraint_set2_flag = u_n(1);
    sps->constraint_set3_flag = u_n(1);
    sps->constraint_set4_flag = u_n(1);
    sps->constraint_set5_flag = u_n(1);
    sps->reserved_zero_2bits = u_n(2);
    sps->level_idc = u_n(8);
    sps->seq_parameter_set_id = ue_v();
    if (sps->profile_idc == 100 || sps->profile_idc == 110 ||
        sps->profile_idc == 122 || sps->profile_idc == 244 || sps->profile_idc == 44 ||
        sps->profile_idc == 83 || sps->profile_idc == 86 || sps->profile_idc == 118 ||
        sps->profile_idc == 128 || sps->profile_idc == 138 || sps->profile_idc == 139 ||
        sps->profile_idc == 134 || sps->profile_idc == 135)
    {
        sps->chroma_format_idc = ue_v();
        if (sps->chroma_format_idc == 3)
        {
            sps->separate_colour_plane_flag = u_n(1);
        }
        sps->bit_depth_luma_minus8 = ue_v();
        sps->bit_depth_chroma_minus8 = ue_v();
        sps->qpprime_y_zero_transform_bypass_flag = u_n(1);
        sps->seq_scaling_matrix_present_flag = u_n(1);
        if (sps->seq_scaling_matrix_present_flag)
        {
            sps->seq_scaling_list_present_flag = calloc((sps->chroma_format_idc != 3 ? 8 : 12), sizeof(uint8_t));
            for (int i = 0; i < (sps->chroma_format_idc != 3 ? 8 : 12); ++i)
            {
                sps->seq_scaling_list_present_flag[i] = u_n(1);
                if (sps->seq_scaling_list_present_flag[i])
                {
                    if (i < 6)
                    {
                        // [TODO]
                    }
                    else
                    {
                        // [TODO]
                    }
                }
            }
        }
    }
    sps->log2_max_frame_num_minus4 = ue_v();
    sps->pic_order_cnt_type = ue_v();
    if (sps->pic_order_cnt_type == 0)
    {
        sps->log2_max_pic_order_cnt_lsb_minus4 = ue_v();
    }
    else if (sps->pic_order_cnt_type == 1)
    {
        sps->delta_pic_order_always_zero_flag = ue_v();
        sps->offset_for_non_ref_pic = se_v();
        sps->offset_for_top_to_bottom_field = se_v();
        sps->num_ref_frames_in_pic_order_cnt_cycle = ue_v();
        sps->offset_for_ref_frame = calloc(sps->num_ref_frames_in_pic_order_cnt_cycle, sizeof(int32_t));
        for (int i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle; ++i)
        {
            sps->offset_for_ref_frame[i] = se_v();
        }
    }

    sps->max_num_ref_frames = ue_v();
    sps->gaps_in_frame_num_value_allowed_flag = u_n(1);
    sps->pic_width_in_mbs_minus1 = ue_v();
    sps->pic_height_in_map_units_minus1 = ue_v();
    sps->frame_mbs_only_flag = u_n(1);
    if (!sps->frame_mbs_only_flag)
        sps->mb_adaptive_frame_field_flag = u_n(1);
    sps->direct_8x8_inference_flag = u_n(1);
    sps->frame_cropping_flag = u_n(1);
    if (sps->frame_cropping_flag)
    {
        sps->frame_crop_left_offset = ue_v();
        sps->frame_crop_right_offset = ue_v();
        sps->frame_crop_top_offset = ue_v();
        sps->frame_crop_bottom_offset = ue_v();
    }
    sps->vui_parameters_present_flag = u_n(1);
    if (sps->vui_parameters_present_flag)
    {
        // [TODO]
    }
}

void dump_seq_parameter_set(FILE *fp, struct NAL_unit *nal)
{
    fprintf(fp, "    seq_parameter_set_data() {\n");
    fprintf(fp, "        profile_idc: %u\n", nal->rbsp.sps.profile_idc);
    fprintf(fp, "        constraint_set0_flag: %u\n", nal->rbsp.sps.constraint_set0_flag);
    fprintf(fp, "        constraint_set1_flag: %u\n", nal->rbsp.sps.constraint_set1_flag);
    fprintf(fp, "        constraint_set2_flag: %u\n", nal->rbsp.sps.constraint_set2_flag);
    fprintf(fp, "        constraint_set3_flag: %u\n", nal->rbsp.sps.constraint_set3_flag);
    fprintf(fp, "        constraint_set4_flag: %u\n", nal->rbsp.sps.constraint_set4_flag);
    fprintf(fp, "        constraint_set5_flag: %u\n", nal->rbsp.sps.constraint_set5_flag);
    fprintf(fp, "        reserved_zero_2bits: %u\n", nal->rbsp.sps.reserved_zero_2bits);
    fprintf(fp, "        level_idc: %u\n", nal->rbsp.sps.level_idc);
    fprintf(fp, "        seq_parameter_set_id: %u\n", nal->rbsp.sps.seq_parameter_set_id);
    fprintf(fp, "        if (nal->rbsp.sps.profile_idc== 100 || nal->rbsp.sps.profile_idc== 110 ||\n");
    fprintf(fp, "            nal->rbsp.sps.profile_idc== 122 || nal->rbsp.sps.profile_idc== 244 || nal->rbsp.sps.profile_idc== 44 ||\n");
    fprintf(fp, "            nal->rbsp.sps.profile_idc== 83 || nal->rbsp.sps.profile_idc== 86 || nal->rbsp.sps.profile_idc== 118 ||\n");
    fprintf(fp, "            nal->rbsp.sps.profile_idc== 128 || nal->rbsp.sps.profile_idc== 138 || nal->rbsp.sps.profile_idc== 139 ||\n");
    fprintf(fp, "            nal->rbsp.sps.profile_idc== 134 || nal->rbsp.sps.profile_idc== 135) {\n");
    if (nal->rbsp.sps.profile_idc == 100 || nal->rbsp.sps.profile_idc == 110 ||
        nal->rbsp.sps.profile_idc == 122 || nal->rbsp.sps.profile_idc == 244 || nal->rbsp.sps.profile_idc == 44 ||
        nal->rbsp.sps.profile_idc == 83 || nal->rbsp.sps.profile_idc == 86 || nal->rbsp.sps.profile_idc == 118 ||
        nal->rbsp.sps.profile_idc == 128 || nal->rbsp.sps.profile_idc == 138 || nal->rbsp.sps.profile_idc == 139 ||
        nal->rbsp.sps.profile_idc == 134 || nal->rbsp.sps.profile_idc == 135)
    {
        fprintf(fp, "            [TODO]\n");
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        }\n");
    fprintf(fp, "        log2_max_frame_num_minus4: %u\n", nal->rbsp.sps.log2_max_frame_num_minus4);
    fprintf(fp, "        pic_order_cnt_type: %u\n", nal->rbsp.sps.pic_order_cnt_type);
    fprintf(fp, "        if (pic_order_cnt_type == 0)\n");
    if (nal->rbsp.sps.pic_order_cnt_type == 0)
    {
        fprintf(fp, "            log2_max_pic_order_cnt_lsb_minus4: %u", nal->rbsp.sps.log2_max_pic_order_cnt_lsb_minus4);
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        else if (pic_order_cnt_type == 1) {\n");
    if (nal->rbsp.sps.pic_order_cnt_type == 1)
    {
        fprintf(fp, "            delta_pic_order_always_zero_flag: %u\n", nal->rbsp.sps.delta_pic_order_always_zero_flag);
        fprintf(fp, "            offset_for_non_ref_pic: %d\n", nal->rbsp.sps.offset_for_non_ref_pic);
        fprintf(fp, "            offset_for_top_to_bottom_field: %d\n", nal->rbsp.sps.offset_for_top_to_bottom_field);
        fprintf(fp, "            num_ref_frames_in_pic_order_cnt_cycle: %u\n", nal->rbsp.sps.num_ref_frames_in_pic_order_cnt_cycle);
        fprintf(fp, "            for (i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)\n");
        for (int i = 0; i < nal->rbsp.sps.num_ref_frames_in_pic_order_cnt_cycle; ++i)
        {
            fprintf(fp, "                offset_for_ref_frame[%d]: %d\n", i, nal->rbsp.sps.offset_for_ref_frame[i]);
        }
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        }\n");
    fprintf(fp, "        max_num_ref_frames: %u\n", nal->rbsp.sps.max_num_ref_frames);
    fprintf(fp, "        gaps_in_frame_num_value_allowed_flag: %u\n", nal->rbsp.sps.gaps_in_frame_num_value_allowed_flag);
    fprintf(fp, "        pic_width_in_mbs_minus1: %u\n", nal->rbsp.sps.pic_width_in_mbs_minus1);
    fprintf(fp, "        pic_height_in_map_units_minus1: %u\n", nal->rbsp.sps.pic_height_in_map_units_minus1);
    fprintf(fp, "        frame_mbs_only_flag: %u\n", nal->rbsp.sps.frame_mbs_only_flag);
    fprintf(fp, "        if (!frame_mbs_only_flag)\n");
    if (!nal->rbsp.sps.frame_mbs_only_flag)
    {
        fprintf(fp, "            mb_adaptive_frame_field_flag: %u\n", nal->rbsp.sps.mb_adaptive_frame_field_flag);
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        direct_8x8_inference_flag: %u\n", nal->rbsp.sps.direct_8x8_inference_flag);
    fprintf(fp, "        frame_cropping_flag: %u\n", nal->rbsp.sps.frame_cropping_flag);
    fprintf(fp, "        if (frame_cropping_flag) {\n");
    if (nal->rbsp.sps.frame_cropping_flag)
    {
        fprintf(fp, "            frame_crop_left_offset: %u\n", nal->rbsp.sps.frame_crop_left_offset);
        fprintf(fp, "            frame_crop_right_offset: %u\n", nal->rbsp.sps.frame_crop_right_offset);
        fprintf(fp, "            frame_crop_top_offset: %u\n", nal->rbsp.sps.frame_crop_top_offset);
        fprintf(fp, "            frame_crop_bottom_offset: %u\n", nal->rbsp.sps.frame_crop_bottom_offset);
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        }\n");
    fprintf(fp, "        vui_parameters_present_flag: %u\n", nal->rbsp.sps.vui_parameters_present_flag);
    fprintf(fp, "        if (vui_parameters_present_flag)\n");
    fprintf(fp, "            [TODO] vui_parameters()\n");
    fprintf(fp, "    }\n\n");
}