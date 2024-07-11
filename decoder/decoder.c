#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include "decoder.h"
#include "group_of_pictures.h"
#include "slice.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"
#include "bit_stream.h"
#include "nal_unit.h"
#include "misc.h"
#include "common/log.h"

// NALU: Network Abstraction Layer Unit
// EBSP: Encapsulated Byte Sequence Packets
// RBSP: Raw Byte Sequence Payload
// SODB: String Of Data Bits
// EPB: Emulation Prevention Bytes

uint8_t startcode[NALU_STARTCODE_LEN] = {0x00, 0x00, 0x01};

struct h264_decoder_context
{
    struct h264_decoder_config cfg;

    struct bit_stream bs; // current processing stream

    int nal_count;

    struct group_of_pictures gop;

    FILE *fp_dump_info;
};

// 判断给定buffer在offset位置是否是一个startcode
static bool check_code(uint8_t *buffer, uint8_t codes[], int count_code)
{
    for (int i = 0; i < count_code; ++i)
    {
        if (buffer[i] != codes[i])
            return false;
    }

    return true;
}

static int process(struct h264_decoder_context *ctx)
{
    struct bit_stream *bs = &ctx->bs;
    if (bs->size < NALU_STARTCODE_LEN)
    {
        logwarn("wait more stream (%lu < %d)", bs->size, NALU_STARTCODE_LEN);
        return H264_WAIT_MORE_STREAM;
    }

    size_t startcode_byte_offsets[2] = {0};
    int count_startcode = 0;
    for (int i = 0; i < bs->size; ++i)
    {
        if (count_startcode >= 2)
            break;
        if (check_code(bs->data + i, startcode, NALU_STARTCODE_LEN))
        {
            startcode_byte_offsets[count_startcode++] = i;
        }
    }

    if (count_startcode < 2)
        return H264_WAIT_MORE_STREAM;

    struct byte_stream_nal_unit_t bsnu = {0};
    size_t bsnu_size = startcode_byte_offsets[1] - startcode_byte_offsets[0];
    byte_stream_nal_unit(&bsnu, bsnu_size, &ctx->bs);
    ++ctx->nal_count;
    dump_byte_stream_nal_unit(ctx->fp_dump_info, &bsnu, ctx->nal_count);

    bs->size -= bsnu_size;
    memcpy(bs->data, bs->data + bsnu_size, bs->size);
    bs->bit_offset = 0;

    // Table 7-1 – NAL unit type codes, syntax element categories, and NAL unit type classes
    switch (bsnu.nu.nal_unit_type)
    {
    case H264_NAL_IDR_SLICE:
    {
        struct slice_layer_without_partitioning_t slwp = {0};
        struct bit_stream bs = {bsnu.nu.rbsp_byte, bsnu.nu.NumBytesInRBSP};
        slice_layer_without_partitioning_rbsp(&slwp, &bs, &bsnu.nu, ctx->gop.spsrs, ctx->gop.ppsrs);
        dump_slice_layer_without_partitioning_rbsp(ctx->fp_dump_info, &slwp);
    }
    break;
    case H264_NAL_SPS:
    {
        struct seq_parameter_set_rbsp_t spsr = {0};
        struct bit_stream bs = {bsnu.nu.rbsp_byte, bsnu.nu.NumBytesInRBSP};
        seq_parameter_set_rbsp(&spsr, &bs);
        if (spsr.spsd.seq_parameter_set_id >= MAX_SPS_COUNT)
            logfatal("sps id %d exceeds max %d", spsr.spsd.seq_parameter_set_id, MAX_SPS_COUNT);
        free_seq_parameter_set(&ctx->gop.spsrs[spsr.spsd.seq_parameter_set_id]);
        ctx->gop.spsrs[spsr.spsd.seq_parameter_set_id] = spsr;
        dump_seq_parameter_set_rbsp(ctx->fp_dump_info, &spsr);
    }
    break;
    case H264_NAL_PPS:
    {
        struct pic_parameter_set_rbsp_t ppsr = {0};
        struct bit_stream bs = {bsnu.nu.rbsp_byte, bsnu.nu.NumBytesInRBSP};
        pic_paramster_set_rbsp(&ppsr, &bs);
        if (ppsr.pic_parameter_set_id >= MAX_PPS_COUNT)
            logfatal("pps id %d exceeds max %d", ppsr.pic_parameter_set_id, MAX_PPS_COUNT);
        free_pic_parameter_set(&ctx->gop.ppsrs[ppsr.pic_parameter_set_id]);
        ctx->gop.ppsrs[ppsr.pic_parameter_set_id] = ppsr;
        dump_pic_paramster_set_rbsp(ctx->fp_dump_info, &ppsr);
    }
    break;
    default:
        logerror("nal %s(%d) is not supported", nal_type_name(bsnu.nu.nal_unit_type), bsnu.nu.nal_unit_type);
    }

    return H264_SUCCESS;
}

int h264_decoder_create(h264_decoder_handle *h, struct h264_decoder_config *cfg)
{
    if (!h || !cfg)
    {
        logerror("invalid argument");
        return H264_INVALID_ARGUMENT;
    }

    struct h264_decoder_context *ctx = calloc(1, sizeof(struct h264_decoder_context));
    if (!ctx)
    {
        logerror("calloc failed: %s", strerror(errno));
        return H264_MEM_ALLOC_FAILED;
    }

    // [TODO] check config

    ctx->fp_dump_info = fopen("debug_dump.txt", "w");

    ctx->cfg = *cfg;

    *h = ctx;

    return H264_SUCCESS;
}

void h264_decoder_destroy(h264_decoder_handle h)
{
    struct h264_decoder_context *ctx = h;
    if (!h)
        return;

    free(ctx);
}

int h264_decoder_send_stream(h264_decoder_handle h, struct h264_stream *stream)
{
    struct h264_decoder_context *ctx = h;
    if (!h)
    {
        logerror("invalid argument");
        return H264_INVALID_ARGUMENT;
    }

    // 将新的数据拷贝进来
    ctx->bs.data = realloc(ctx->bs.data, ctx->bs.size + stream->size);
    memcpy(ctx->bs.data + ctx->bs.size, stream->buffer, stream->size);
    ctx->bs.size += stream->size;

    return process(ctx);
}

void h264_decoder_debug(h264_decoder_handle h)
{
    struct h264_decoder_context *ctx = h;
    if (!h)
    {
        logerror("invalid argument");
        return;
    }

    // struct h264_NALU *nalu = ctx->nalus_head;

    // while (nalu)
    // {
    // loginfo("index: %d, size: %lu, rbsp_trailing_bits: %d", nalu->index, nalu->size, nalu->count_rbsp_trailing_bits);
    // loginfo("header: %02x, forbidden_zero_bit: %d, nal_ref_idc: %d, nal_unit_type: %d",
    //     nalu->header.byte, nalu->header.forbidden_zero_bit, nalu->header.nal_ref_idc, nalu->header.nal_unit_type);

    // nalu = nalu->next;
    // }
}