#ifndef H264_H__
#define H264_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <stdint.h>

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
        uint64_t seq_parameter_set_id;                    // ue(v)
        uint64_t log2_max_frame_num_minus4;               // ue(v)
        uint64_t pic_order_cnt_type;                      // ue(v)
        uint64_t log2_max_pic_order_cnt_lsb_minus4;       // ue(v)
        uint8_t delta_pic_order_always_zero_flag : 1;     // u(1)
        int32_t offset_for_non_ref_pic;                   // se(v)
        int32_t offset_for_top_to_bottom_field;           // se(v)
        uint64_t num_ref_frames_in_pic_order_cnt_cycle;   // ue(v)
        int32_t *offset_for_ref_frame;                    // se(v)
        uint64_t max_num_ref_frames;                      // ue(v)
        uint8_t gaps_in_frame_num_value_allowed_flag : 1; // u(1)
        uint64_t pic_width_in_mbs_minus1;                 // ue(v)
        uint64_t pic_height_in_map_units_minus1;          // ue(v)
        uint8_t frame_mbs_only_flag : 1;                  // u(1)
        uint8_t mb_adaptive_frame_field_flag : 1;         // u(1)
        uint8_t direct_8x8_inference_flag : 1;            // u(1)
        uint8_t frame_cropping_flag : 1;                  // u(1)
        uint64_t frame_crop_left_offset;                  // ue(v)
        uint64_t frame_crop_right_offset;                 // ue(v)
        uint64_t frame_crop_top_offset;                   // ue(v)
        uint64_t frame_crop_bottom_offset;                // ue(v)
        uint8_t vui_parameters_present_flag : 1;          // u(1)
    };

    struct pic_parameter_set
    {
        uint64_t pic_parameter_set_id;                            // ue(v)
        uint64_t seq_parameter_set_id;                            // ue(v)
        uint8_t entropy_coding_mode_flag : 1;                     // u(1)
        uint8_t bottom_field_pic_order_in_frame_present_flag : 1; // u(1)
        uint64_t num_slice_groups_minus1;                         // ue(v)
        uint64_t slice_group_map_type;                            // ue(v)
        uint64_t *run_length_minus1;                              // ue(v)
        uint64_t *top_left;                                       // ue(v)
        uint64_t *bottom_right;                                   // ue(v)
        uint8_t slice_group_change_direction_flag;                // u(1)
        uint64_t slice_group_change_rate_minus1;                  // ue(v)
        uint64_t pic_size_in_map_units_minus1;                    // ue(v)
        uint32_t *slice_group_id;                                 // u(v)
        uint64_t num_ref_idx_l0_default_active_minus1;            // ue(v)
        uint64_t num_ref_idx_l1_default_active_minus1;            // ue(v)
        uint8_t weighted_pred_flag;                               // u(1)
        uint8_t weighted_bipred_idc;                              // u(2)
        int32_t pic_init_qp_minus26;                              // se(v)
        int32_t pic_init_qs_minus26;                              // se(v)
        int32_t chroma_qp_index_offset;                           // se(v)
        uint8_t deblocking_filter_control_present_flag;           // u(1)
        uint8_t constrained_intra_pred_flag;                      // u(1)
        uint8_t redundant_pic_cnt_present_flag;                   // u(1)
        uint8_t transform_8x8_mode_flag;                          // u(1)
        uint8_t pic_scaling_matrix_present_flag;                  // u(1)
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
        } header;
        size_t nalUnitHeaderBytes; // header size;

        uint8_t emulation_prevention_three_byte; // f(8)

        uint8_t *rbsp_byte; // rbsp data
        struct
        {
            union
            {
                struct seq_parameter_set sps;
                struct pic_parameter_set pps;
                struct sei sei;
            };
        } rbsp;
        size_t NumBytesInRBSP; // rbsp size
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // H264_H__