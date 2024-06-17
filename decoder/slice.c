#include "common/misc.h"
#include "slice.h"

#define u_n(n) read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, (n))
#define ue_v() read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)
#define se_v() read_se_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset)

void read_slice_header(struct seq_parameter_set *sps, struct NAL_unit *nal)
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
    if (!sps->frame_mbs_only_flag)
    {
        slice_header->field_pic_flag = u_n(1);
        if (slice_header->field_pic_flag)
            slice_header->bottom_field_flag = u_n(1);
        if (nal->header.nal_unit_type == H264_NAL_IDR_SLICE)
            slice_header->idr_pic_id = ue_v();
    }

    slice_header->frame_num = u_n(sps->log2_max_frame_num_minus4 + 4);
    // [TODO]
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
    // fprintf(fp, "        \n");
    // fprintf(fp, "        \n");
    fprintf(fp, "    }\n");
}