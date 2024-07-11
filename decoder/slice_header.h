#ifndef SLICE_HEADER_H__
#define SLICE_HEADER_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"
#include "nal_unit.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"

    // H.7.3.3.1.1 Reference picture list MVC modification syntax
    // 7.4.3.1 Reference picture list modification semantics
    struct ref_pic_list_mvc_modification_t
    {
        uint8_t ref_pic_list_modification_flag_l0; // u(1)
        uint8_t modification_of_pic_nums_idc;      // ue(v)
        uint8_t abs_diff_pic_num_minus1;           // ue(v)
        uint8_t long_term_pic_num;                 // ue(v)
        uint8_t abs_diff_view_idx_minus1;          // ue(v)
        uint8_t ref_pic_list_modification_flag_l1; // u(1)
    };

    // 7.3.3.1 Reference picture list modification syntax
    // 7.4.3.1 Reference picture list modification semantics
    struct ref_pic_list_modification_t
    {
        uint8_t ref_pic_list_modification_flag_l0; // u(1)
        uint8_t modification_of_pic_nums_idc;      // ue(v)
        uint8_t abs_diff_pic_num_minus1;           // ue(v)
        uint8_t long_term_pic_num;                 // ue(v)
        uint8_t ref_pic_list_modification_flag_l1; // u(1)
    };

    // 7.3.3.2 Prediction weight table syntax
    // 7.4.3.2 Prediction weight table semantics
    struct pred_weight_table_t
    {
        // luma_log2_weight_denom 2 ue(v)
        // chroma_log2_weight_denom 2 ue(v)
        // luma_weight_l0_flag 2 u(1)
        // luma_weight_l0[ i ] 2 se(v)
        // luma_offset_l0[ i ] 2 se(v)
        // chroma_weight_l0_flag 2 u(1)
        // chroma_weight_l0[ i ][ j ] 2 se(v)
        // chroma_offset_l0[ i ][ j ] 2 se(v)
        // luma_weight_l1_flag 2 u(1)
        // luma_weight_l1[ i ] 2 se(v)
        // luma_offset_l1[ i ] 2 se(v)
        // chroma_weight_l1_flag 2 u(1)
        // chroma_weight_l1[ i ][ j ] 2 se(v)
        // chroma_offset_l1[ i ][ j ] 2 se(v)
    };

    // 7.3.3.3 Decoded reference picture marking syntax
    // 7.4.3.3 Decoded reference picture marking semantics
    struct dec_ref_pic_marking_t
    {
        uint8_t no_output_of_prior_pics_flag : 1;       // u(1)
        uint8_t long_term_reference_flag : 1;           // u(1)
        uint8_t adaptive_ref_pic_marking_mode_flag : 1; // u(1)
        uint32_t memory_management_control_operation;   // ue(v)
        uint32_t difference_of_pic_nums_minus1;         // ue(v)
        uint32_t long_term_pic_num;                     // ue(v)
        uint32_t long_term_frame_idx;                   // ue(v)
        uint32_t max_long_term_frame_idx_plus1;         // ue(v)
    };

    // 7.3.3 Slice header syntax
    // 7.4.3 Slice header semantics
    struct slice_header_t
    {
        uint32_t first_mb_in_slice;                   // ue(v)
        uint32_t slice_type;                          // ue(v)
        uint32_t pic_parameter_set_id;                // ue(v)
        uint8_t colour_plane_id : 2;                  // u(2)
        uint32_t frame_num;                           // u(v)
        uint8_t field_pic_flag : 1;                   // u(1)
        uint8_t bottom_field_flag : 1;                // u(1)
        uint32_t idr_pic_id;                          // ue(v)
        uint32_t pic_order_cnt_lsb;                   // u(v)
        int32_t delta_pic_order_cnt_bottom;           // se(v)
        int32_t delta_pic_order_cnt[2];               // se(v)
        uint32_t redundant_pic_cnt;                   // ue(v)
        uint8_t direct_spatial_mv_pred_flag : 1;      // u(1)
        uint8_t num_ref_idx_active_override_flag : 1; // u(1)
        uint32_t num_ref_idx_l0_active_minus1;        // ue(v)
        uint32_t num_ref_idx_l1_active_minus1;        // ue(v)
        struct ref_pic_list_mvc_modification_t rplmm;
        struct ref_pic_list_modification_t rplm;
        struct dec_ref_pic_marking_t drpm;
        uint32_t cabac_init_idc;                // ue(v)
        int32_t slice_qp_delta;                 // se(v)
        uint8_t sp_for_switch_flag : 1;         // u(1)
        int32_t slice_qs_delta;                 // se(v)
        uint32_t disable_deblocking_filter_idc; // ue(v)
        int32_t slice_alpha_c0_offset_div2;     // se(v)
        int32_t slice_beta_offset_div2;         // se(v)
        uint32_t slice_group_change_cycle;      // u(v) (7-35)

        uint32_t PrevRefFrameNum;         // 7.4.3
        uint32_t UnusedShortTermFrameNum; // (7-24)
        uint32_t MbaffFrameFlag;          // (7-25)
        uint32_t PicHeightInMbs;          // (7-26)
        uint32_t PicHeightInSamplesL;     // (7-27)
        uint32_t PicHeightInSamplesC;     // (7-28)
        uint32_t PicSizeInMbs;            // (7-29)
        uint32_t MaxPicNum;               // 7.4.3
        uint32_t CurrPicNum;              // 7.4.3
        int32_t SliceQPY;                 // (7-30)
        int32_t QSY;                      // (7-31)
        uint32_t FilterOffsetA;           // (7-32)
        uint32_t FilterOffsetB;           // (7-33)
        uint32_t MapUnitsInSliceGroup0;   // (7-34)
    };

    void slice_header(
        struct slice_header_t *sh,
        struct bit_stream *bs,
        struct nal_unit_t *nu,
        struct seq_parameter_set_rbsp_t *spsrs,
        struct pic_parameter_set_rbsp_t *ppsrs);

    void dump_slice_header(FILE *fp, struct slice_header_t *sh);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SLICE_HEADER_H__