#include "common/misc.h"
#include "slice.h"

void read_slice_header(struct seq_parameter_set *sps, struct NAL_unit *nal)
{
    int bit_offset = 0;
    struct slice_header *slice_header = &nal->rbsp.slice.header;

    slice_header->first_mb_in_slice = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    slice_header->slice_type = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    slice_header->pic_parameter_set_id = read_ue_v(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset);
    if (sps->separate_colour_plane_flag == 1)
    {
        slice_header->colour_plane_id = read_bits(nal->rbsp_byte, nal->NumBytesInRBSP, &bit_offset, 2);
    }
    // [TODO]
}

void dump_slice_header(FILE *fp, struct NAL_unit *nal)
{
    fprintf(fp, "    slice_header() {\n");
    fprintf(fp, "        first_mb_in_slice: %u\n", nal->rbsp.slice.header.first_mb_in_slice);
    fprintf(fp, "        slice_type: %u\n", nal->rbsp.slice.header.slice_type);
    fprintf(fp, "        pic_parameter_set_id: %u\n", nal->rbsp.slice.header.pic_parameter_set_id);
    fprintf(fp, "    }\n");
}