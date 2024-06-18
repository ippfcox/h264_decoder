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
#define dump(name, placeholder) fprintf(fp, "    %s: %" placeholder "\n", #name, nal->rbsp.sps.name)

    dump(profile_idc, "u");
    dump(constraint_set0_flag, "u");
    dump(constraint_set1_flag, "u");
    dump(constraint_set2_flag, "u");
    dump(constraint_set3_flag, "u");
    dump(constraint_set4_flag, "u");
    dump(constraint_set5_flag, "u");
    dump(reserved_zero_2bits, "u");
    dump(level_idc, "u");
    dump(seq_parameter_set_id, "u");
    dump(chroma_format_idc, "u");
    dump(separate_colour_plane_flag, "u");
    dump(bit_depth_luma_minus8, "u");
    dump(bit_depth_chroma_minus8, "u");
    dump(qpprime_y_zero_transform_bypass_flag, "u");
    dump(seq_scaling_matrix_present_flag, "u");
    if (nal->rbsp.sps.seq_scaling_list_present_flag)
        for (int i = 0; i < (nal->rbsp.sps.chroma_format_idc != 3 ? 8 : 12); ++i)
            dump(seq_scaling_list_present_flag[i], "u");
    dump(log2_max_frame_num_minus4, "u");
    dump(pic_order_cnt_type, "u");
    dump(log2_max_pic_order_cnt_lsb_minus4, "u");
    dump(delta_pic_order_always_zero_flag, "u");
    dump(offset_for_non_ref_pic, "d");
    dump(offset_for_top_to_bottom_field, "d");
    dump(num_ref_frames_in_pic_order_cnt_cycle, "u");
    if (nal->rbsp.sps.offset_for_ref_frame)
        for (int i = 0; i < nal->rbsp.sps.num_ref_frames_in_pic_order_cnt_cycle; ++i)
            dump(offset_for_ref_frame[i], "d");
    dump(max_num_ref_frames, "u");
    dump(gaps_in_frame_num_value_allowed_flag, "u");
    dump(pic_width_in_mbs_minus1, "u");
    dump(pic_height_in_map_units_minus1, "u");
    dump(frame_mbs_only_flag, "u");
    dump(mb_adaptive_frame_field_flag, "u");
    dump(direct_8x8_inference_flag, "u");
    dump(frame_cropping_flag, "u");
    dump(frame_crop_left_offset, "u");
    dump(frame_crop_right_offset, "u");
    dump(frame_crop_top_offset, "u");
    dump(frame_crop_bottom_offset, "u");
    dump(vui_parameters_present_flag, "u");

    fprintf(fp, "\n");
}