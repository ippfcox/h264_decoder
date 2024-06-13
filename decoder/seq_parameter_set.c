#include <stdlib.h>
#include "seq_parameter_set.h"
#include "common/misc.h"
#include "common/log.h"

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
    sps->profile_idc = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 8);
    sps->constraint_set0_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->constraint_set1_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->constraint_set2_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->constraint_set3_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->constraint_set4_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->constraint_set5_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->reserved_zero_2bits = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 2);
    sps->level_idc = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 8);
    sps->seq_parameter_set_id = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    if (sps->profile_idc == 100 || sps->profile_idc == 110 ||
        sps->profile_idc == 122 || sps->profile_idc == 244 || sps->profile_idc == 44 ||
        sps->profile_idc == 83 || sps->profile_idc == 86 || sps->profile_idc == 118 ||
        sps->profile_idc == 128 || sps->profile_idc == 138 || sps->profile_idc == 139 ||
        sps->profile_idc == 134 || sps->profile_idc == 135)
    {
        // [TODO]
    }
    sps->log2_max_frame_num_minus4 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    sps->pic_order_cnt_type = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    if (sps->pic_order_cnt_type == 0)
    {
        sps->log2_max_pic_order_cnt_lsb_minus4 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    }
    else if (sps->pic_order_cnt_type == 1)
    {
        sps->delta_pic_order_always_zero_flag = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        sps->offset_for_non_ref_pic = read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        sps->offset_for_top_to_bottom_field = read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        sps->num_ref_frames_in_pic_order_cnt_cycle = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        sps->offset_for_ref_frame = calloc(sps->num_ref_frames_in_pic_order_cnt_cycle, sizeof(int32_t));
        for (int i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle; ++i)
        {
            sps->offset_for_ref_frame[i] = read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        }
    }

    sps->max_num_ref_frames = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    sps->gaps_in_frame_num_value_allowed_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->pic_width_in_mbs_minus1 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    sps->pic_height_in_map_units_minus1 = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    sps->frame_mbs_only_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    if (!sps->frame_mbs_only_flag)
        sps->mb_adaptive_frame_field_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->direct_8x8_inference_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    sps->frame_cropping_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
    if (sps->frame_cropping_flag)
    {
        sps->frame_crop_left_offset = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        sps->frame_crop_right_offset = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        sps->frame_crop_top_offset = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
        sps->frame_crop_bottom_offset = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    }
    sps->vui_parameters_present_flag = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 1);
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
    fprintf(fp, "        seq_parameter_set_id: %lu\n", nal->rbsp.sps.seq_parameter_set_id);
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
    fprintf(fp, "        log2_max_frame_num_minus4: %lu\n", nal->rbsp.sps.log2_max_frame_num_minus4);
    fprintf(fp, "        pic_order_cnt_type: %lu\n", nal->rbsp.sps.pic_order_cnt_type);
    fprintf(fp, "        if (pic_order_cnt_type == 0)\n");
    if (nal->rbsp.sps.pic_order_cnt_type == 0)
    {
        fprintf(fp, "            log2_max_pic_order_cnt_lsb_minus4: %lu", nal->rbsp.sps.log2_max_pic_order_cnt_lsb_minus4);
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
        fprintf(fp, "            num_ref_frames_in_pic_order_cnt_cycle: %lu\n", nal->rbsp.sps.num_ref_frames_in_pic_order_cnt_cycle);
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
    fprintf(fp, "        max_num_ref_frames: %lu\n", nal->rbsp.sps.max_num_ref_frames);
    fprintf(fp, "        gaps_in_frame_num_value_allowed_flag: %u\n", nal->rbsp.sps.gaps_in_frame_num_value_allowed_flag);
    fprintf(fp, "        pic_width_in_mbs_minus1: %lu\n", nal->rbsp.sps.pic_width_in_mbs_minus1);
    fprintf(fp, "        pic_height_in_map_units_minus1: %lu\n", nal->rbsp.sps.pic_height_in_map_units_minus1);
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
        fprintf(fp, "            frame_crop_left_offset: %lu\n", nal->rbsp.sps.frame_crop_left_offset);
        fprintf(fp, "            frame_crop_right_offset: %lu\n", nal->rbsp.sps.frame_crop_right_offset);
        fprintf(fp, "            frame_crop_top_offset: %lu\n", nal->rbsp.sps.frame_crop_top_offset);
        fprintf(fp, "            frame_crop_bottom_offset: %lu\n", nal->rbsp.sps.frame_crop_bottom_offset);
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