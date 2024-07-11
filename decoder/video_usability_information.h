#ifndef VIDEO_USABILITY_INFORMATION_H__
#define VIDEO_USABILITY_INFORMATION_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"

    struct video_usability_information
    {
        uint8_t aspect_ratio_info_present_flag : 1;          // u(1)
        uint8_t aspect_ratio_idc;                            // u(8)
        uint16_t sar_width;                                  // u(16)
        uint16_t sar_height;                                 // u(16)
        uint8_t overscan_info_present_flag : 1;              // u(1)
        uint8_t overscan_appropriate_flag : 1;               // u(1)
        uint8_t video_signal_type_present_flag : 1;          // u(1)
        uint8_t video_format : 3;                            // u(3)
        uint8_t video_full_range_flag : 1;                   // u(1)
        uint8_t colour_description_present_flag : 1;         // u(1)
        uint8_t colour_primaries;                            // u(8)
        uint8_t transfer_characteristics;                    // u(8)
        uint8_t matrix_coefficients;                         // u(8)
        uint8_t chroma_loc_info_present_flag : 1;            // u(1)
        uint32_t chroma_sample_loc_type_top_field;           // ue(v)
        uint32_t chroma_sample_loc_type_bottom_field;        // ue(v)
        uint8_t timing_info_present_flag : 1;                // u(1)
        uint32_t num_units_in_tick;                          // u(32)
        uint32_t time_scale;                                 // u(32)
        uint8_t fixed_frame_rate_flag : 1;                   // u(1)
        uint8_t nal_hrd_parameters_present_flag : 1;         // u(1)
        uint8_t vcl_hrd_parameters_present_flag : 1;         // u(1)
        uint8_t low_delay_hrd_flag : 1;                      // u(1)
        uint8_t pic_struct_present_flag : 1;                 // u(1)
        uint8_t bitstream_restriction_flag : 1;              // u(1)
        uint8_t motion_vectors_over_pic_boundaries_flag : 1; // u(1)
        uint32_t max_bytes_per_pic_denom;                    // ue(v)
        uint32_t max_bits_per_mb_denom;                      // ue(v)
        uint32_t log2_max_mv_length_horizontal;              // ue(v)
        uint32_t log2_max_mv_length_vertical;                // ue(v)
        uint32_t max_num_reorder_frames;                     // ue(v)
        uint32_t max_dec_frame_buffering;                    // ue(v)
    };

    void vui_parameters(struct video_usability_information *vui, struct bit_stream *bs);

    void dump_video_usability_information(FILE *fp, struct video_usability_information *vui);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // VIDEO_USABILITY_INFORMATION_H__