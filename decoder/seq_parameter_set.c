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

    log_debug("profile: %u, (%u, %u, %u, %u, %u, %u), z: %u, level: %u, sps_id: %u, mfnm4: %u, poct: %u, mpoclm4: %u, dpoazf: %u",
        sps->profile_idc,
        sps->constraint_set0_flag,
        sps->constraint_set1_flag,
        sps->constraint_set2_flag,
        sps->constraint_set3_flag,
        sps->constraint_set4_flag,
        sps->constraint_set5_flag,
        sps->reserved_zero_2bits,
        sps->level_idc,
        sps->seq_parameter_set_id,
        sps->log2_max_frame_num_minus4,
        sps->pic_order_cnt_type,
        sps->log2_max_pic_order_cnt_lsb_minus4,
        sps->delta_pic_order_always_zero_flag);

    log_debug("mnrf: %lu, gifnva: %u, pw: %lu, ph: %lu, mbs_o: %u, m_ada: %u, 8x8: %u, cr: %u, (%lu, %lu, %lu, %lu), vui: %u",
        sps->max_num_ref_frames,
        sps->gaps_in_frame_num_value_allowed_flag,
        sps->pic_width_in_mbs_minus1,
        sps->pic_height_in_map_units_minus1,
        sps->frame_mbs_only_flag,
        sps->mb_adaptive_frame_field_flag,
        sps->direct_8x8_inference_flag,
        sps->frame_cropping_flag,
        sps->frame_crop_left_offset,
        sps->frame_crop_right_offset,
        sps->frame_crop_top_offset,
        sps->frame_crop_bottom_offset,
        sps->vui_parameters_present_flag);
}