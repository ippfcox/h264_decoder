#include "common/misc.h"
#include "slice.h"

#define u_n(n) read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, bit_offset, (n))
#define ue_v() read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, bit_offset)
#define se_v() read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, bit_offset)

static void read_slice_header(struct seq_parameter_set *sps, struct pic_parameter_set *pps, struct NAL_unit *nal, int *bit_offset)
{
    struct slice_header *slice_header = &nal->rbsp.slice.header;
    nal->sps = sps;

    slice_header->first_mb_in_slice = ue_v();
    slice_header->slice_type = ue_v();
    slice_header->pic_parameter_set_id = ue_v();
    if (sps->separate_colour_plane_flag == 1)
    {
        slice_header->colour_plane_id = u_n(2);
    }
    slice_header->frame_num = u_n(sps->log2_max_frame_num_minus4 + 4);
    if (!sps->frame_mbs_only_flag)
    {
        slice_header->field_pic_flag = u_n(1);
        if (slice_header->field_pic_flag)
            slice_header->bottom_field_flag = u_n(1);
    }
    if (nal->header.nal_unit_type == H264_NAL_IDR_SLICE)
        slice_header->idr_pic_id = ue_v();
    if (sps->pic_order_cnt_type == 0)
    {
        slice_header->pic_order_cnt_lsb = u_n(sps->log2_max_pic_order_cnt_lsb_minus4 + 4);
        if (pps->bottom_field_pic_order_in_frame_present_flag && !slice_header->field_pic_flag)
            slice_header->delta_pic_order_cnt_bottom = se_v();
    }
    if (sps->pic_order_cnt_type == 1 && !sps->delta_pic_order_always_zero_flag)
    {
        slice_header->delta_pic_order_cnt[0] = se_v();
        if (pps->bottom_field_pic_order_in_frame_present_flag && !slice_header->field_pic_flag)
            slice_header->delta_pic_order_cnt[1] = se_v();
    }
    if (pps->redundant_pic_cnt_present_flag)
        slice_header->redundant_pic_cnt = ue_v();
    if (slice_header->slice_type == H264_SLICE_B)
        slice_header->direct_spatial_mv_pred_flag = u_n(1);
    if (slice_header->slice_type == H264_SLICE_P || slice_header->slice_type == H264_SLICE_SP || slice_header->slice_type == H264_SLICE_B)
    {
        slice_header->num_ref_idx_active_override_flag = u_n(1);
        if (slice_header->num_ref_idx_active_override_flag)
        {
            slice_header->num_ref_idx_l0_active_minus1 = ue_v();
            if (slice_header->slice_type == H264_SLICE_B)
                slice_header->num_ref_idx_l1_active_minus1 = ue_v();
        }
    }
    if (nal->header.nal_unit_type == H264_NAL_EXTEN_SLICE || nal->header.nal_unit_type == H264_NAL_DEPTH_EXTEN_SLICE)
    {
        // [TODO] ref_pic_list_mvc_modification()
    }
    else
    {
        // [TODO] ref_pic_list_modification()
    }
    if ((pps->weighted_pred_flag && (slice_header->slice_type == H264_SLICE_P || slice_header->slice_type == H264_SLICE_SP)) ||
        (pps->weighted_bipred_idc == 1 && slice_header->slice_type == H264_SLICE_B))
    {
        // [TODO] pred_weight_table()
    }
    if (nal->header.nal_ref_idc != 0)
    {
        // [TODO] // dec_ref_pic_marking()
    }
    if (pps->entropy_coding_mode_flag && slice_header->slice_type != H264_SLICE_I && slice_header->slice_type != H264_SLICE_SI)
        slice_header->cabac_init_idc = ue_v();
    slice_header->slice_qp_delta = se_v();
    if (slice_header->slice_type == H264_SLICE_SP || slice_header->slice_type == H264_SLICE_SI)
    {
        if (slice_header->slice_type == H264_SLICE_SP)
            slice_header->sp_for_switch_flag = u_n(1);
        slice_header->slice_qs_delta = se_v();
    }
    if (pps->deblocking_filter_control_present_flag)
    {
        slice_header->disable_deblocking_filter_idc = ue_v();
        if (slice_header->disable_deblocking_filter_idc != 1)
        {
            slice_header->slice_alpha_c0_offset_div2 = se_v();
            slice_header->slice_beta_offset_div2 = se_v();
        }
    }
    if (pps->num_slice_groups_minus1 > 0 &&
        pps->slice_group_map_type >= 3 && pps->slice_group_map_type <= 5)
    {
        // [TODO]
    }
    slice_header->slice_group_change_cycle = u_n(pps->num_slice_groups_minus1 + 1);
}

static void read_slice_data(struct seq_parameter_set *sps, struct pic_parameter_set *pps, struct NAL_unit *nal, int *bit_offset)
{
    struct slice_header *header = &nal->rbsp.slice.header;
    struct slice_data *data = &nal->rbsp.slice.data;

    if (pps->entropy_coding_mode_flag)
        while (!byte_aligned(*bit_offset))
            data->cabac_alignment_one_bit = u_n(1);

    // (7-25)
    bool MbaffFrameFlag = sps->mb_adaptive_frame_field_flag && !header->field_pic_flag;
}

void read_slice(struct seq_parameter_set *sps, struct pic_parameter_set *pps, struct NAL_unit *nal)
{
    int bit_offset = 0;
    read_slice_header(sps, pps, nal, &bit_offset);
    read_slice_data(sps, pps, nal, &bit_offset);
}

void dump_slice_header(FILE *fp, struct NAL_unit *nal)
{
#define dump(name, placeholder) fprintf(fp, "    %s: %" placeholder "\n", #name, nal->rbsp.slice.header.name)

    dump(first_mb_in_slice, "u");
    dump(slice_type, "u");
    dump(pic_parameter_set_id, "u");
    dump(colour_plane_id, "u");
    dump(frame_num, "u");
    dump(field_pic_flag, "u");
    dump(bottom_field_flag, "u");
    dump(idr_pic_id, "u");
    dump(pic_order_cnt_lsb, "u");
    dump(delta_pic_order_cnt_bottom, "d");
    for (int i = 0; i < 2; ++i)
        dump(delta_pic_order_cnt[i], "d");
    dump(redundant_pic_cnt, "u");
    dump(direct_spatial_mv_pred_flag, "u");
    dump(num_ref_idx_active_override_flag, "u");
    dump(num_ref_idx_l0_active_minus1, "u");
    dump(num_ref_idx_l1_active_minus1, "u");

    dump(cabac_init_idc, "u");
    dump(slice_qp_delta, "d");
    dump(sp_for_switch_flag, "u");
    dump(slice_qs_delta, "d");
    dump(disable_deblocking_filter_idc, "u");
    dump(slice_alpha_c0_offset_div2, "d");
    dump(slice_beta_offset_div2, "d");
    dump(slice_group_change_cycle, "u");

    fprintf(fp, "\n");
}