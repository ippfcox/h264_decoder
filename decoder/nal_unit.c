#include <stdlib.h>
#include "seq_parameter_set.h"
#include "misc.h"
#include "nal_unit.h"
#include "common/log.h"

// 7.3.1 NAL unit syntax
// nal_unit(NumBytesInNALunit)
void nal_unit(struct nal_unit_t *nu, size_t nal_size, struct bit_stream *bs)
{
    int bit_offset = 0;
    nu->NumBytesInNALunit = nal_size;

    nu->forbidden_zero_bit = bs_f(bs, 1); // f(1) 0
    // check forced 0
    nu->nal_ref_idc = bs_u(bs, 2);   // u(2)
    nu->nal_unit_type = bs_u(bs, 5); // u(5)
    nu->NumBytesInRBSP = 0;
    nu->nalUnitHeaderBytes = 1;

    if (nu->nal_unit_type == H264_NAL_PREFIX ||
        nu->nal_unit_type == H264_NAL_EXTEN_SLICE ||
        nu->nal_unit_type == H264_NAL_DEPTH_EXTEN_SLICE)
    {
        nu->svc_extension_flag = 0;
        nu->avc_3d_extension_flag = 0;
        if (nu->nal_unit_type != H264_NAL_DEPTH_EXTEN_SLICE)
        {
            nu->svc_extension_flag = bs_u(bs, 1); // u(1)
        }
        else
        {
            nu->avc_3d_extension_flag = bs_u(bs, 1); // u(1)
        }

        if (nu->svc_extension_flag)
        {
            // nal_unit_header_svc_extension() specified in Annex G
            nu->svc_extension.idr_flag = bs_u(bs, 1);
            nu->svc_extension.priority_id = bs_u(bs, 6);
            nu->svc_extension.no_inter_layer_pred_flag = bs_u(bs, 1);
            nu->svc_extension.dependency_id = bs_u(bs, 3);
            nu->svc_extension.quality_id = bs_u(bs, 4);
            nu->svc_extension.temporal_id = bs_u(bs, 3);
            nu->svc_extension.use_ref_base_pic_flag = bs_u(bs, 1);
            nu->svc_extension.discardable_flag = bs_u(bs, 1);
            nu->svc_extension.output_flag = bs_u(bs, 1);
            nu->svc_extension.reserved_three_2bits = bs_u(bs, 2);

            nu->nalUnitHeaderBytes += 3;
        }
        else if (nu->avc_3d_extension_flag)
        {
            // nal_unit_header_3davc_extension() specified in Annex J
            nu->_3davc_extension.view_idx = bs_u(bs, 8);
            nu->_3davc_extension.depth_flag = bs_u(bs, 1);
            nu->_3davc_extension.non_idr_flag = bs_u(bs, 1);
            nu->_3davc_extension.temporal_id = bs_u(bs, 3);
            nu->_3davc_extension.anchor_pic_flag = bs_u(bs, 1);
            nu->_3davc_extension.inter_view_flag = bs_u(bs, 1);

            nu->nalUnitHeaderBytes += 2;
        }
        else
        {
            // nal_unit_header_mvc_extension() specified in Annex H
            nu->mvc_extension.non_idr_flag = bs_u(bs, 1);
            nu->mvc_extension.priority_id = bs_u(bs, 6);
            nu->mvc_extension.view_id = bs_u(bs, 10);
            nu->mvc_extension.temporal_id = bs_u(bs, 3);
            nu->mvc_extension.anchor_pic_flag = bs_u(bs, 1);
            nu->mvc_extension.inter_view_flag = bs_u(bs, 1);
            nu->mvc_extension.reserved_one_bit = bs_u(bs, 1);

            nu->nalUnitHeaderBytes += 3;
        }
    }

    nu->rbsp_byte = calloc(nu->NumBytesInNALunit, sizeof(uint8_t)); // 先申请个大的
    for (int i = nu->nalUnitHeaderBytes; i < nu->NumBytesInNALunit; ++i)
    {
        bit_offset = i * 8;
        if (i + 2 < nu->NumBytesInNALunit && bs_next_bits(bs, 24) == 0x000003)
        {
            nu->rbsp_byte[nu->NumBytesInRBSP++] = bs_u(bs, 8); // b(8) 0x00
            nu->rbsp_byte[nu->NumBytesInRBSP++] = bs_u(bs, 8); // b(8) 0x00
            i += 2;
            nu->emulation_prevention_three_byte = bs_f(bs, 8); // f(8) 0x03
            // check forced 0x03
        }
        else
        {
            nu->rbsp_byte[nu->NumBytesInRBSP++] = bs_u(bs, 8); // b(8)
        }
    }
    nu->rbsp_byte = realloc(nu->rbsp_byte, nu->NumBytesInRBSP);

    nu->IdrPicFlag = nu->nal_unit_type == H264_NAL_IDR_SLICE ? true : false;
    nu->DepthFlag = (nu->nal_unit_type != H264_NAL_DEPTH_EXTEN_SLICE) ? 0 : (nu->avc_3d_extension_flag ? nu->_3davc_extension.depth_flag : 1);

    logdebug("NBINALu: %u, nUHB: %lu, NBIRBSP: %lu, (%u, %u, %u), (%u, %u), (%u)",
        nu->NumBytesInNALunit,
        nu->nalUnitHeaderBytes,
        nu->NumBytesInRBSP,
        nu->forbidden_zero_bit, nu->nal_ref_idc, nu->nal_unit_type,
        nu->svc_extension_flag, nu->avc_3d_extension_flag,
        nu->emulation_prevention_three_byte);
}

// B.1.1 Byte stream NAL unit syntax
// byte_stream_nal_unit(NumBytesInNALunit)
void byte_stream_nal_unit(struct byte_stream_nal_unit_t *bsnu, size_t bsnu_size, struct bit_stream *bs)
{
    while (bs_next_bits(bs, 24) != 0x000001 &&
           bs_next_bits(bs, 32) != 0x00000001)
    {
        uint8_t leading_zero_8bits = bs_f(bs, 8);
    }
    if (bs_next_bits(bs, 24) != 0x000001)
    {
        uint8_t zero_byte = bs_f(bs, 8);
    }
    uint32_t start_code_prefix_one_3bytes = bs_f(bs, 24);
    nal_unit(&bsnu->nu, bsnu_size - 3, bs);
    // while (bs_more_data_in_byte_stream(bs) && // [FIXME] return false all the time, for I dont implement all nal_unit
    //        bs_next_bits(bs, 24) != 0x000001 &&
    //        bs_next_bits(bs, 32) != 0x00000001)
    // {
    //     uint8_t trailing_zero_bits = bs_f(bs, 8);
    // }
}

void dump_byte_stream_nal_unit(FILE *fp, struct byte_stream_nal_unit_t *bsnu, size_t index)
{
    fprintf(fp, "nal #%ld %s(%d), ref_idc: %u\n", index, nal_type_name(bsnu->nu.nal_unit_type), bsnu->nu.nal_unit_type, bsnu->nu.nal_ref_idc);
}