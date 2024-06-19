#include <stdlib.h>
#include "seq_parameter_set.h"
#include "common/misc.h"
#include "common/log.h"

#define u_n(n) read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, bit_offset, (n))
#define ue_v() read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, bit_offset)
#define se_v() read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, bit_offset)

static void read_vui_parameters(struct NAL_unit *nal, int *bit_offset)
{
    nal->rbsp.sps.vui = calloc(1, sizeof(struct video_usability_information));
    struct video_usability_information *vui = nal->rbsp.sps.vui;

    vui->aspect_ratio_info_present_flag = u_n(1);
    if (vui->aspect_ratio_info_present_flag)
    {
        vui->aspect_ratio_idc = u_n(8);
        if (vui->aspect_ratio_idc == 0) // todo
        {
            vui->sar_width = u_n(16);
            vui->sar_height = u_n(16);
        }
    }
    vui->overscan_info_present_flag = u_n(1);
    if (vui->overscan_info_present_flag)
        vui->overscan_appropriate_flag = u_n(1);
    vui->video_signal_type_present_flag = u_n(1);
    if (vui->video_signal_type_present_flag)
    {
        vui->video_format = u_n(3);
        vui->video_full_range_flag = u_n(1);
        vui->colour_description_present_flag = u_n(1);
        if (vui->colour_description_present_flag)
        {
            vui->colour_primaries = u_n(8);
            vui->transfer_characteristics = u_n(8);
            vui->matrix_coefficients = u_n(8);
        }
    }
    vui->chroma_loc_info_present_flag = u_n(1);
    if (vui->chroma_loc_info_present_flag)
    {
        vui->chroma_sample_loc_type_top_field = ue_v();
        vui->chroma_sample_loc_type_bottom_field = ue_v();
    }
    vui->timing_info_present_flag = u_n(1);
    if (vui->timing_info_present_flag)
    {
        vui->num_units_in_tick = u_n(32);
        vui->time_scale = u_n(32);
        vui->fixed_frame_rate_flag = u_n(1);
    }
    vui->nal_hrd_parameters_present_flag = u_n(1);
    if (vui->nal_hrd_parameters_present_flag)
    {
        // [TODO] hrd_parameters()
    }
    vui->vcl_hrd_parameters_present_flag = u_n(1);
    if (vui->vcl_hrd_parameters_present_flag)
    {
        // [TODO] hrd_parameters()
    }
    if (vui->nal_hrd_parameters_present_flag || vui->vcl_hrd_parameters_present_flag)
        vui->low_delay_hrd_flag = u_n(1);
    vui->pic_struct_present_flag = u_n(1);
    vui->bitstream_restriction_flag = u_n(1);
    if (vui->bitstream_restriction_flag)
    {
        vui->motion_vectors_over_pic_boundaries_flag = u_n(1);
        vui->max_bytes_per_pic_denom = ue_v();
        vui->max_bits_per_mb_denom = ue_v();
        vui->log2_max_mv_length_horizontal = ue_v();
        vui->log2_max_mv_length_vertical = ue_v();
        vui->max_num_reorder_frames = ue_v();
        vui->max_dec_frame_buffering = ue_v();
    }
}

// 7.3.2.1.1 Sequence parameter set data syntax
// seq_parameter_set_data()
static void read_seq_parameter_set(struct NAL_unit *nal, int *bit_offset)
{
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
    // 7.4.2.1.1 Sequence parameter set data semantics
    // When chroma_format_idc is not present, it shall be inferred to be equal to 1 (4:2:0 chroma format).
    sps->chroma_format_idc = 1;
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
        sps->delta_pic_order_always_zero_flag = u_n(1);
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
        read_vui_parameters(nal, bit_offset);
    }
}

// 7.3.2.1 Sequence parameter set RBSP syntax
// seq_parameter_set_rbsp()
void read_seq_parameter_set_rbsp(struct NAL_unit *nal)
{
    int bit_offset = 0;
    read_seq_parameter_set(nal, &bit_offset);
}

static void dump_video_usability_information(FILE *fp, struct NAL_unit *nal)
{
#define dump(indents, name, placeholder) fprintf(fp, "%s%s: %" placeholder "\n", make_indents(indents), #name, nal->rbsp.sps.vui->name)
    dump(2, aspect_ratio_info_present_flag, "u");
    dump(3, aspect_ratio_idc, "u");
    dump(4, sar_width, "u");
    dump(4, sar_height, "u");
    dump(2, overscan_info_present_flag, "u");
    dump(3, overscan_appropriate_flag, "u");
    dump(2, video_signal_type_present_flag, "u");
    dump(3, video_format, "u");
    dump(3, video_full_range_flag, "u");
    dump(3, colour_description_present_flag, "u");
    dump(4, colour_primaries, "u");
    dump(4, transfer_characteristics, "u");
    dump(4, matrix_coefficients, "u");
    dump(2, chroma_loc_info_present_flag, "u");
    dump(3, chroma_sample_loc_type_top_field, "u");
    dump(3, chroma_sample_loc_type_bottom_field, "u");
    dump(2, timing_info_present_flag, "u");
    dump(3, num_units_in_tick, "u");
    dump(3, time_scale, "u");
    dump(3, fixed_frame_rate_flag, "u");
    dump(2, nal_hrd_parameters_present_flag, "u");
    dump(2, vcl_hrd_parameters_present_flag, "u");
    dump(3, low_delay_hrd_flag, "u");
    dump(2, pic_struct_present_flag, "u");
    dump(2, bitstream_restriction_flag, "u");
    dump(3, motion_vectors_over_pic_boundaries_flag, "u");
    dump(3, max_bytes_per_pic_denom, "u");
    dump(3, max_bits_per_mb_denom, "u");
    dump(3, log2_max_mv_length_horizontal, "u");
    dump(3, log2_max_mv_length_vertical, "u");
    dump(3, max_num_reorder_frames, "u");
    dump(3, max_dec_frame_buffering, "u");
#undef dump
}

void dump_seq_parameter_set(FILE *fp, struct NAL_unit *nal)
{
#define dump(indents, name, placeholder) fprintf(fp, "%s%s: %" placeholder "\n", make_indents(indents), #name, nal->rbsp.sps.name)

    dump(1, profile_idc, "u");
    dump(1, constraint_set0_flag, "u");
    dump(1, constraint_set1_flag, "u");
    dump(1, constraint_set2_flag, "u");
    dump(1, constraint_set3_flag, "u");
    dump(1, constraint_set4_flag, "u");
    dump(1, constraint_set5_flag, "u");
    dump(1, reserved_zero_2bits, "u");
    dump(1, level_idc, "u");
    dump(1, seq_parameter_set_id, "u");
    dump(2, chroma_format_idc, "u");
    dump(3, separate_colour_plane_flag, "u");
    dump(2, bit_depth_luma_minus8, "u");
    dump(2, bit_depth_chroma_minus8, "u");
    dump(2, qpprime_y_zero_transform_bypass_flag, "u");
    dump(2, seq_scaling_matrix_present_flag, "u");
    if (nal->rbsp.sps.seq_scaling_list_present_flag)
        for (int i = 0; i < (nal->rbsp.sps.chroma_format_idc != 3 ? 8 : 12); ++i)
            dump(4, seq_scaling_list_present_flag[i], "u");
    dump(1, log2_max_frame_num_minus4, "u");
    dump(1, pic_order_cnt_type, "u");
    dump(2, log2_max_pic_order_cnt_lsb_minus4, "u");
    dump(2, delta_pic_order_always_zero_flag, "u");
    dump(2, offset_for_non_ref_pic, "d");
    dump(2, offset_for_top_to_bottom_field, "d");
    dump(2, num_ref_frames_in_pic_order_cnt_cycle, "u");
    if (nal->rbsp.sps.offset_for_ref_frame)
        for (int i = 0; i < nal->rbsp.sps.num_ref_frames_in_pic_order_cnt_cycle; ++i)
            dump(4, offset_for_ref_frame[i], "d");
    dump(1, max_num_ref_frames, "u");
    dump(1, gaps_in_frame_num_value_allowed_flag, "u");
    dump(1, pic_width_in_mbs_minus1, "u");
    dump(1, pic_height_in_map_units_minus1, "u");
    dump(1, frame_mbs_only_flag, "u");
    dump(2, mb_adaptive_frame_field_flag, "u");
    dump(1, direct_8x8_inference_flag, "u");
    dump(1, frame_cropping_flag, "u");
    dump(2, frame_crop_left_offset, "u");
    dump(2, frame_crop_right_offset, "u");
    dump(2, frame_crop_top_offset, "u");
    dump(2, frame_crop_bottom_offset, "u");
    dump(1, vui_parameters_present_flag, "u");
    if (nal->rbsp.sps.vui)
        dump_video_usability_information(fp, nal);
#undef dump

    fprintf(fp, "\n");
}