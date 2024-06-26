#ifndef H264_H__
#define H264_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

    // Table 7-1 - NAL unit type codes
    enum H264_NAL_TYPE
    {
        H264_NAL_UNSPECIFIED = 0,
        H264_NAL_SLICE = 1,
        H264_NAL_DPA = 2,
        H264_NAL_DPB = 3,
        H264_NAL_DPC = 4,
        H264_NAL_IDR_SLICE = 5,
        H264_NAL_SEI = 6,
        H264_NAL_SPS = 7,
        H264_NAL_PPS = 8,
        H264_NAL_AUD = 9,
        H264_NAL_END_SEQUENCE = 10,
        H264_NAL_END_STREAM = 11,
        H264_NAL_FILLER_DATA = 12,
        H264_NAL_SPS_EXT = 13,
        H264_NAL_PREFIX = 14,
        H264_NAL_SUB_SPS = 15,
        H264_NAL_DPS = 16,
        H264_NAL_RESERVED17 = 17,
        H264_NAL_RESERVED18 = 18,
        H264_NAL_AUXILIARY_SLICE = 19,
        H264_NAL_EXTEN_SLICE = 20,
        H264_NAL_DEPTH_EXTEN_SLICE = 21,
        H264_NAL_RESERVED22 = 22,
        H264_NAL_RESERVED23 = 23,
        H264_NAL_UNSPECIFIED24 = 24,
        H264_NAL_UNSPECIFIED25 = 25,
        H264_NAL_UNSPECIFIED26 = 26,
        H264_NAL_UNSPECIFIED27 = 27,
        H264_NAL_UNSPECIFIED28 = 28,
        H264_NAL_UNSPECIFIED29 = 29,
        H264_NAL_UNSPECIFIED30 = 30,
        H264_NAL_UNSPECIFIED31 = 31,
    };

    enum H264_SLICE_TYPE
    {
        H264_SLICE_P = 0,
        H264_SLICE_B = 1,
        H264_SLICE_I = 2,
        H264_SLICE_SP = 3,
        H264_SLICE_SI = 4,
        H264_SLICE_P2 = 5,
        H264_SLICE_B2 = 6,
        H264_SLICE_I2 = 7,
        H264_SLICE_SP2 = 8,
        H264_SLICE_SI2 = 9,
    };

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

    struct seq_parameter_set
    {
        uint8_t profile_idc;                              // u(8)
        uint8_t constraint_set0_flag : 1;                 // u(1)
        uint8_t constraint_set1_flag : 1;                 // u(1)
        uint8_t constraint_set2_flag : 1;                 // u(1)
        uint8_t constraint_set3_flag : 1;                 // u(1)
        uint8_t constraint_set4_flag : 1;                 // u(1)
        uint8_t constraint_set5_flag : 1;                 // u(1)
        uint8_t reserved_zero_2bits : 2;                  // u(2)
        uint8_t level_idc;                                // u(8)
        uint32_t seq_parameter_set_id;                    // ue(v)
        uint32_t chroma_format_idc;                       // ue(v)
        uint8_t separate_colour_plane_flag : 1;           // u(1)
        uint32_t bit_depth_luma_minus8;                   // ue(v)
        uint32_t bit_depth_chroma_minus8;                 // ue(v)
        uint8_t qpprime_y_zero_transform_bypass_flag : 1; // u(1)
        uint8_t seq_scaling_matrix_present_flag : 1;      // u(1)
        uint8_t *seq_scaling_list_present_flag;           // u(1)
        uint32_t log2_max_frame_num_minus4;               // ue(v)
        uint32_t pic_order_cnt_type;                      // ue(v)
        uint32_t log2_max_pic_order_cnt_lsb_minus4;       // ue(v)
        uint8_t delta_pic_order_always_zero_flag : 1;     // u(1)
        int32_t offset_for_non_ref_pic;                   // se(v)
        int32_t offset_for_top_to_bottom_field;           // se(v)
        uint32_t num_ref_frames_in_pic_order_cnt_cycle;   // ue(v)
        int32_t *offset_for_ref_frame;                    // se(v)
        uint32_t max_num_ref_frames;                      // ue(v)
        uint8_t gaps_in_frame_num_value_allowed_flag : 1; // u(1)
        uint32_t pic_width_in_mbs_minus1;                 // ue(v)
        uint32_t pic_height_in_map_units_minus1;          // ue(v)
        uint8_t frame_mbs_only_flag : 1;                  // u(1)
        uint8_t mb_adaptive_frame_field_flag : 1;         // u(1)
        uint8_t direct_8x8_inference_flag : 1;            // u(1)
        uint8_t frame_cropping_flag : 1;                  // u(1)
        uint32_t frame_crop_left_offset;                  // ue(v)
        uint32_t frame_crop_right_offset;                 // ue(v)
        uint32_t frame_crop_top_offset;                   // ue(v)
        uint32_t frame_crop_bottom_offset;                // ue(v)
        uint8_t vui_parameters_present_flag : 1;          // u(1)
        struct video_usability_information *vui;
    };

    struct pic_parameter_set
    {
        uint32_t pic_parameter_set_id;                            // ue(v)
        uint32_t seq_parameter_set_id;                            // ue(v)
        uint8_t entropy_coding_mode_flag : 1;                     // u(1)
        uint8_t bottom_field_pic_order_in_frame_present_flag : 1; // u(1)
        uint32_t num_slice_groups_minus1;                         // ue(v)
        uint32_t slice_group_map_type;                            // ue(v)
        uint32_t *run_length_minus1;                              // ue(v)
        uint32_t *top_left;                                       // ue(v)
        uint32_t *bottom_right;                                   // ue(v)
        uint8_t slice_group_change_direction_flag : 1;            // u(1)
        uint32_t slice_group_change_rate_minus1;                  // ue(v)
        uint32_t pic_size_in_map_units_minus1;                    // ue(v)
        uint32_t *slice_group_id;                                 // u(v)
        uint32_t num_ref_idx_l0_default_active_minus1;            // ue(v)
        uint32_t num_ref_idx_l1_default_active_minus1;            // ue(v)
        uint8_t weighted_pred_flag : 1;                           // u(1)
        uint8_t weighted_bipred_idc : 2;                          // u(2)
        int32_t pic_init_qp_minus26;                              // se(v)
        int32_t pic_init_qs_minus26;                              // se(v)
        int32_t chroma_qp_index_offset;                           // se(v)
        uint8_t deblocking_filter_control_present_flag : 1;       // u(1)
        uint8_t constrained_intra_pred_flag : 1;                  // u(1)
        uint8_t redundant_pic_cnt_present_flag : 1;               // u(1)
        uint8_t transform_8x8_mode_flag : 1;                      // u(1)
        uint8_t pic_scaling_matrix_present_flag : 1;              // u(1)
        uint8_t *pic_scaling_list_present_flag;                   // u(1)
        int32_t second_chroma_qp_index_offset;                    // se(v)
    };

    struct sei_message
    {
        // TODO
    };

    struct sei
    {
        struct sei_message *messages;
        int count_messages;
    };

    struct slice_header
    {
        uint32_t first_mb_in_slice;
        uint32_t slice_type;
        uint32_t pic_parameter_set_id;
        uint8_t colour_plane_id : 2;
        uint32_t frame_num;
        uint8_t field_pic_flag : 1;
        uint8_t bottom_field_flag : 1;
        uint32_t idr_pic_id;
        uint32_t pic_order_cnt_lsb;
        int32_t delta_pic_order_cnt_bottom;
        int32_t delta_pic_order_cnt[2];
        uint32_t redundant_pic_cnt;
        uint8_t direct_spatial_mv_pred_flag : 1;
        uint8_t num_ref_idx_active_override_flag : 1;
        uint32_t num_ref_idx_l0_active_minus1;
        uint32_t num_ref_idx_l1_active_minus1;

        uint32_t cabac_init_idc;
        int32_t slice_qp_delta;
        uint8_t sp_for_switch_flag : 1;
        int32_t slice_qs_delta;
        uint32_t disable_deblocking_filter_idc;
        int32_t slice_alpha_c0_offset_div2;
        int32_t slice_beta_offset_div2;
        uint32_t slice_group_change_cycle;
    };

    struct slice_data
    {
        uint8_t cabac_alignment_one_bit : 1; // f(1)
        uint32_t mb_skip_run;                // ue(v)
        uint32_t mb_skip_flag;               // ae(v)
        uint32_t mb_field_decoding_flag;     // u(1) | ae(v)
        uint32_t end_of_slice_flag;          // ae(v)
    };

    struct slice
    {
        struct slice_header header;
        bool MbaffFrameFlag;
        struct slice_data data;
    };

    // nal = startcode+nalu header+rbsp
    struct NAL_unit
    {
        // 原始数据
        uint8_t *buffer;
        size_t size;
        int index;

        size_t NumBytesInNALunit; // == size

        struct
        {
            uint8_t forbidden_zero_bit : 1; // f(1)
            uint8_t nal_ref_idc : 2;        // u(2)
            uint8_t nal_unit_type : 5;      // u(5)
            uint8_t svc_extension_flag : 1; // u(1)
            // [TODO]
            uint8_t avc_3d_extension_flag : 1; // u(1)
            // [TODO]

            bool IdrPicFlag;
            bool DepthFlag;
        } header;
        size_t nalUnitHeaderBytes; // header size;

        uint8_t emulation_prevention_three_byte; // f(8)

        struct seq_parameter_set *sps; // 当前有效的sps

        uint8_t *rbsp_byte; // rbsp data
        struct
        {
            union
            {
                struct seq_parameter_set sps;
                struct pic_parameter_set pps;
                struct sei sei;
                struct slice slice;
            };
        } rbsp;
        size_t NumBytesInRBSP; // rbsp size
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // H264_H__