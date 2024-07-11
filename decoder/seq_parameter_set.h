#ifndef SEQ_PARAMETER_SET_H__
#define SEQ_PARAMETER_SET_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"
#include "video_usability_information.h"

    // 7.3.2.1.1 Sequence parameter set data syntax
    // 7.4.2.1.1 Sequence parameter set data semantics
    struct seq_parameter_set_data_t
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
        struct video_usability_information vui;

        uint8_t SubWidthC;                         // (Table 6-1)
        uint8_t SubHeightC;                        // (Table 6-1)
        uint8_t MbWidthC;                          // (6-1)
        uint8_t MbHeightC;                         // (6-2)
        uint8_t ChromaArrayType;                   // 7.4.2.1.1
        uint32_t BitDepthY;                        // (7-3)
        uint32_t QpBdOffsetY;                      // (7-4)
        uint32_t BitDepthC;                        // (7-5)
        uint32_t QpBdOffsetC;                      // (7-6)
        uint32_t RawMbBits;                        // (7-7)
        uint8_t Flat_4x4_16[16];                   // (7-8)
        uint8_t Flat_8x8_16[64];                   // (7-9)
        uint64_t MaxFrameNum;                      // (7-10)
        uint64_t MaxPicOrderCntLsb;                // (7-11)
        uint64_t ExpectedDeltaPerPicOrderCntCycle; // (7-12)
        uint32_t PicWidthInMbs;                    // (7-13)
        uint32_t PicWidthInSamplesL;               // (7-14)
        uint32_t PicWidthInSamplesC;               // (7-15)
        uint32_t PicHeightInMapUnits;              // (7-16)
        uint32_t PicSizeInMapUnits;                // (7-17)
        uint32_t FrameHeightInMbs;                 // (7-18)
        uint32_t CropUnitX;                        // (7-19)(7-21)
        uint32_t CropUnitY;                        // (7-20)(7-22)
    };

    // 7.3.2.1 Sequence parameter set RBSP syntax
    // 7.4.2.1 Sequence parameter set RBSP semantics
    struct seq_parameter_set_rbsp_t
    {
        struct seq_parameter_set_data_t spsd;
        // rbsp_tailing_bits
    };

    void seq_parameter_set_rbsp(struct seq_parameter_set_rbsp_t *spsr, struct bit_stream *bs);

    void free_seq_parameter_set(struct seq_parameter_set_rbsp_t *spsr);

    void dump_seq_parameter_set_rbsp(FILE *fp, struct seq_parameter_set_rbsp_t *spsr);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SEQ_PARAMETER_SET_H__