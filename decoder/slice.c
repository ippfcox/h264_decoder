#include "common/misc.h"
#include "slice.h"

#define u_n(n) read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, (n))
#define ue_v() read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)
#define se_v() read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)

void read_slice_header(struct seq_parameter_set *sps, struct pic_parameter_set *pps, struct NAL_unit *nal)
{
    int bit_offset = 0;
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
    if (sps->pic_order_cnt_type == 1 && sps->delta_pic_order_always_zero_flag)
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

void dump_slice_header(FILE *fp, struct NAL_unit *nal)
{
    fprintf(fp, "    slice_header() {\n");
    fprintf(fp, "        first_mb_in_slice: %u\n", nal->rbsp.slice.header.first_mb_in_slice);
    fprintf(fp, "        slice_type: %u\n", nal->rbsp.slice.header.slice_type);
    fprintf(fp, "        pic_parameter_set_id: %u\n", nal->rbsp.slice.header.pic_parameter_set_id);
    fprintf(fp, "        if (separate_colour_plane_flag == 1)\n");
    if (nal->sps->separate_colour_plane_flag)
        fprintf(fp, "            colour_plane_id: %u\n", nal->rbsp.slice.header.colour_plane_id);
    else
        fprintf(fp, "            N/A\n");
    fprintf(fp, "        frame_num: %u\n", nal->rbsp.slice.header.frame_num);
    fprintf(fp, "        if (!frame_mbs_only_flag) {\n");
    if (!nal->sps->frame_mbs_only_flag)
    {
        fprintf(fp, "            field_pic_flag: %u\n", nal->rbsp.slice.header.field_pic_flag);
        fprintf(fp, "            if (field_pic_flag)\n");
        if (nal->rbsp.slice.header.field_pic_flag)
            fprintf(fp, "                bottom_field_flag: %u\n", nal->rbsp.slice.header.bottom_field_flag);
        else
            fprintf(fp, "                N/A\n");
    }
    else
    {
        fprintf(fp, "            N/A\n");
    }
    fprintf(fp, "        }\n");
    fprintf(fp, "        if (IdrPicFlag)\n");
    if (nal->header.nal_unit_type == H264_NAL_IDR_SLICE)
        fprintf(fp, "            idr_pic_id: %u\n", nal->rbsp.slice.header.idr_pic_id);
    else
        fprintf(fp, "            N/A\n");
    fprintf(fp, "        if (pic_order_cnt_type == 0) {\n");

    fprintf(fp, "        }\n");

    fprintf(fp, "        if (pic_order_cnt_type = = 1 && !delta_pic_order_always_zero_flag) {\n");
    fprintf(fp, "        }\n");

    fprintf(fp, "        if (redundant_pic_cnt_present_flag)\n");

    fprintf(fp, "        if (slice_type = = B)\n");

    fprintf(fp, "        if (slice_type = = P | | slice_type = = SP | | slice_type = = B) {\n");
    fprintf(fp, "        }\n");

    fprintf(fp, "        if (nal_unit_type = = 20 | | nal_unit_type = = 21) {\n");
    fprintf(fp, "        }\n");
    
    fprintf(fp, "        if () {\n");
    fprintf(fp, "        }\n");
    fprintf(fp, "        if () {\n");
    fprintf(fp, "        }\n");
    fprintf(fp, "        if () {\n");
    fprintf(fp, "        }\n");
    fprintf(fp, "        if () {\n");
    fprintf(fp, "        }\n");
    fprintf(fp, "        if () {\n");
    fprintf(fp, "        }\n");
    // fprintf(fp, "        \n");
    // fprintf(fp, "        \n");
    fprintf(fp, "    }\n");
}