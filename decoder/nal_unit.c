#include <stdlib.h>
#include "common/misc.h"
#include "nal_unit.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"
#include "common/log.h"

// 7.3.1 NAL unit syntax
// nal_unit(NumBytesInNALunit)
void read_nal_unit(struct NAL_unit *nal)
{
    uint8_t *ptr = nal->buffer + NALU_STARTCODE_LEN;
    int bit_offset = 0;
    nal->NumBytesInNALunit = nal->size;

    nal->header.forbidden_zero_bit = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 1); // f(1) 0
    // check forced 0
    nal->header.nal_ref_idc = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 2);   // u(2)
    nal->header.nal_unit_type = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 5); // u(5)

    nal->NumBytesInRBSP = 0;
    nal->nalUnitHeaderBytes = 1;

    if (nal->header.nal_unit_type == H264_NAL_PREFIX ||
        nal->header.nal_unit_type == H264_NAL_EXTEN_SLICE ||
        nal->header.nal_unit_type == H264_NAL_DEPTH_EXTEN_SLICE)
    {
        nal->header.svc_extension_flag = 0;
        nal->header.avc_3d_extension_flag = 0;
        if (nal->header.nal_unit_type != H264_NAL_DEPTH_EXTEN_SLICE)
        {
            nal->header.svc_extension_flag = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 1); // u(1)
        }
        else
        {
            nal->header.avc_3d_extension_flag = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 1); // u(1)
        }

        if (nal->header.svc_extension_flag)
        {
            // [TODO] nal_unit_header_svc_extension() // specified in Annex G
            nal->nalUnitHeaderBytes += 3;
        }
        else if (nal->header.avc_3d_extension_flag)
        {
            // [TODO] nal_unit_header_3davc_extension() // specified in Annex J
            nal->nalUnitHeaderBytes += 2;
        }
        else
        {
            // [TODO] nal_unit_header_mvc_extension( ) // specified in Annex H
            nal->nalUnitHeaderBytes += 3;
        }
    }

    nal->rbsp_byte = calloc(nal->NumBytesInNALunit, sizeof(uint8_t)); // 先申请个大的
    for (int i = nal->nalUnitHeaderBytes; i < nal->NumBytesInNALunit; ++i)
    {
        bit_offset = i * 8;
        if (i + 2 < nal->NumBytesInNALunit && next_bits(ptr, nal->NumBytesInNALunit, bit_offset, 24) == 0x000003)
        {
            nal->rbsp_byte[nal->NumBytesInRBSP++] = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 8); // b(8) 0x00
            nal->rbsp_byte[nal->NumBytesInRBSP++] = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 8); // b(8) 0x00
            i += 2;
            nal->emulation_prevention_three_byte = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 8); // f(8) 0x03
            // check forced 0x03
        }
        else
        {
            nal->rbsp_byte[nal->NumBytesInRBSP++] = read_bits(ptr, nal->NumBytesInNALunit, &bit_offset, 8); // b(8)
        }
    }
    nal->rbsp_byte = realloc(nal->rbsp_byte, nal->NumBytesInRBSP);

    logdebug("NBINALu: %u, nUHB: %lu, NBIRBSP: %lu, (%u, %u, %u), (%u, %u), (%u)",
        nal->NumBytesInNALunit,
        nal->nalUnitHeaderBytes,
        nal->NumBytesInRBSP,
        nal->header.forbidden_zero_bit, nal->header.nal_ref_idc, nal->header.nal_unit_type,
        nal->header.svc_extension_flag, nal->header.avc_3d_extension_flag,
        nal->emulation_prevention_three_byte);
}

void dump_nal_unit(FILE *fp, struct NAL_unit *nal)
{
    fprintf(fp, "nal #%d %s(%d)\n", nal->index, nal_type_name(nal->header.nal_unit_type), nal->header.nal_unit_type);
    switch (nal->header.nal_unit_type)
    {
    case H264_NAL_SPS:
        dump_seq_parameter_set(fp, nal);
        break;
    case H264_NAL_PPS:
        dump_pic_parameter_set(fp, nal);
        break;
    default:
        fprintf(fp, "    not supported now\n\n");
        break;
    }
}