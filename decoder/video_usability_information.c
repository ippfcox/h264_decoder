#include "Exp-Golomb.h"
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
        vui->chroma_sample_loc_type_top_field = exp_golomb_ue(bs);
        vui->chroma_sample_loc_type_bottom_field = exp_golomb_ue(bs);
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
        vui->max_bytes_per_pic_denom = exp_golomb_ue(bs);
        vui->max_bits_per_mb_denom = exp_golomb_ue(bs);
        vui->log2_max_mv_length_horizontal = exp_golomb_ue(bs);
        vui->log2_max_mv_length_vertical = exp_golomb_ue(bs);
        vui->max_num_reorder_frames = exp_golomb_ue(bs);
        vui->max_dec_frame_buffering = exp_golomb_ue(bs);
    }
}

void dump_video_usability_information(FILE *fp, struct video_usability_information *vui)
{
#define dv(indents, placeholder, member) dump_value(indents, placeholder, *vui, member)

    dv(2, "u", aspect_ratio_info_present_flag);
    dv(3, "u", aspect_ratio_idc);
    dv(4, "u", sar_width);
    dv(4, "u", sar_height);
    dv(2, "u", overscan_info_present_flag);
    dv(3, "u", overscan_appropriate_flag);
    dv(2, "u", video_signal_type_present_flag);
    dv(3, "u", video_format);
    dv(3, "u", video_full_range_flag);
    dv(3, "u", colour_description_present_flag);
    dv(4, "u", colour_primaries);
    dv(4, "u", transfer_characteristics);
    dv(4, "u", matrix_coefficients);
    dv(2, "u", chroma_loc_info_present_flag);
    dv(3, "u", chroma_sample_loc_type_top_field);
    dv(3, "u", chroma_sample_loc_type_bottom_field);
    dv(2, "u", timing_info_present_flag);
    dv(3, "u", num_units_in_tick);
    dv(3, "u", time_scale);
    dv(3, "u", fixed_frame_rate_flag);
    dv(2, "u", nal_hrd_parameters_present_flag);
    dv(2, "u", vcl_hrd_parameters_present_flag);
    dv(3, "u", low_delay_hrd_flag);
    dv(2, "u", pic_struct_present_flag);
    dv(2, "u", bitstream_restriction_flag);
    dv(3, "u", motion_vectors_over_pic_boundaries_flag);
    dv(3, "u", max_bytes_per_pic_denom);
    dv(3, "u", max_bits_per_mb_denom);
    dv(3, "u", log2_max_mv_length_horizontal);
    dv(3, "u", log2_max_mv_length_vertical);
    dv(3, "u", max_num_reorder_frames);
    dv(3, "u", max_dec_frame_buffering);
#undef dv
}