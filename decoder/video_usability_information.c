#include "video_usability_information.h"
#include "misc.h"

void vui_parameters(struct video_usability_information *vui, struct bit_stream *bs)
{
    vui->aspect_ratio_info_present_flag = bs_u(bs, 1);
    if (vui->aspect_ratio_info_present_flag)
    {
        vui->aspect_ratio_idc = bs_u(bs, 8);
        if (vui->aspect_ratio_idc == 0) // todo
        {
            vui->sar_width = bs_u(bs, 16);
            vui->sar_height = bs_u(bs, 16);
        }
    }
    vui->overscan_info_present_flag = bs_u(bs, 1);
    if (vui->overscan_info_present_flag)
        vui->overscan_appropriate_flag = bs_u(bs, 1);
    vui->video_signal_type_present_flag = bs_u(bs, 1);
    if (vui->video_signal_type_present_flag)
    {
        vui->video_format = bs_u(bs, 3);
        vui->video_full_range_flag = bs_u(bs, 1);
        vui->colour_description_present_flag = bs_u(bs, 1);
        if (vui->colour_description_present_flag)
        {
            vui->colour_primaries = bs_u(bs, 8);
            vui->transfer_characteristics = bs_u(bs, 8);
            vui->matrix_coefficients = bs_u(bs, 8);
        }
    }
    vui->chroma_loc_info_present_flag = bs_u(bs, 1);
    if (vui->chroma_loc_info_present_flag)
    {
        vui->chroma_sample_loc_type_top_field = bs_ue(bs);
        vui->chroma_sample_loc_type_bottom_field = bs_ue(bs);
    }
    vui->timing_info_present_flag = bs_u(bs, 1);
    if (vui->timing_info_present_flag)
    {
        vui->num_units_in_tick = bs_u(bs, 32);
        vui->time_scale = bs_u(bs, 32);
        vui->fixed_frame_rate_flag = bs_u(bs, 1);
    }
    vui->nal_hrd_parameters_present_flag = bs_u(bs, 1);
    if (vui->nal_hrd_parameters_present_flag)
    {
        // [TODO] hrd_parameters()
    }
    vui->vcl_hrd_parameters_present_flag = bs_u(bs, 1);
    if (vui->vcl_hrd_parameters_present_flag)
    {
        // [TODO] hrd_parameters()
    }
    if (vui->nal_hrd_parameters_present_flag || vui->vcl_hrd_parameters_present_flag)
        vui->low_delay_hrd_flag = bs_u(bs, 1);
    vui->pic_struct_present_flag = bs_u(bs, 1);
    vui->bitstream_restriction_flag = bs_u(bs, 1);
    if (vui->bitstream_restriction_flag)
    {
        vui->motion_vectors_over_pic_boundaries_flag = bs_u(bs, 1);
        vui->max_bytes_per_pic_denom = bs_ue(bs);
        vui->max_bits_per_mb_denom = bs_ue(bs);
        vui->log2_max_mv_length_horizontal = bs_ue(bs);
        vui->log2_max_mv_length_vertical = bs_ue(bs);
        vui->max_num_reorder_frames = bs_ue(bs);
        vui->max_dec_frame_buffering = bs_ue(bs);
    }
}

void dump_video_usability_information(FILE *fp, struct video_usability_information *vui)
{
#define dump(indents, name, placeholder) fprintf(fp, "%s%s: %" placeholder "\n", make_indents(indents), #name, vui->name)
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