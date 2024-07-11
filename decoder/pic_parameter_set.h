#ifndef PIC_PARAMETER_SET_H__
#define PIC_PARAMETER_SET_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"

    // 7.3.2.2 Picture parameter set RBSP syntax
    // 7.4.2.2 Picture parameter set RBSP semantics
    struct pic_parameter_set_rbsp_t
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

        uint32_t SliceGroupChangeRate;    // (7-23)
        uint32_t UnusedShortTermFrameNum; // (7-24)
    };

    void pic_paramster_set_rbsp(struct pic_parameter_set_rbsp_t *ppsr, struct bit_stream *bs);

    void free_pic_parameter_set(struct pic_parameter_set_rbsp_t *ppsr);

    void dump_pic_paramster_set_rbsp(FILE *fp, struct pic_parameter_set_rbsp_t *ppsr);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PIC_PARAMETER_SET_H__