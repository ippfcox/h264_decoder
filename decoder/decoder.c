#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include "decoder.h"
#include "nal_unit.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"
#include "supplemental_enhancement_information.h"
#include "slice.h"
#include "common/misc.h"
#include "common/log.h"

// NALU: Network Abstraction Layer Unit
// EBSP: Encapsulated Byte Sequence Packets
// RBSP: Raw Byte Sequence Payload
// SODB: String Of Data Bits
// EPB: Emulation Prevention Bytes

uint8_t startcode[NALU_STARTCODE_LEN] = {0x00, 0x00, 0x01};

// 处理nalu信息
struct h264_NALU
{
    struct NAL_unit m;

    struct h264_NALU *prev;
    struct h264_NALU *next;
};

struct h264_decoder_context
{
    struct h264_decoder_config cfg;

    uint8_t *pending_buffer;
    size_t pending_size;

    struct seq_parameter_set *curr_sps;
    struct pic_parameter_set *curr_pps;

    // head指向第一个nalu，rear指向最后一个nalu，proc指向处理过的最后一个nalu
    struct h264_NALU *nalus_head, *nalus_rear, *nalu_proc;
    int count_nalus;

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

// 将pending_buffer中的数据按nalu进行拆分，保存到双向链表中
static int split_nalu(struct h264_decoder_context *ctx)
{
    if (ctx->pending_size < NALU_STARTCODE_LEN)
    {
        logwarn("wait more stream (%lu < %d)", ctx->pending_size, NALU_STARTCODE_LEN);
        return H264_WAIT_MORE_STREAM;
    }

    // 查找一整块内存中的start code
    int *nalu_offsets = NULL;
    int count_nalu_offsets = 0;
    for (int i = 0; i < ctx->pending_size; ++i)
    {
        if (check_code(ctx->pending_buffer + i, startcode, NALU_STARTCODE_LEN))
        {
            nalu_offsets = realloc(nalu_offsets, (++count_nalu_offsets) * sizeof(int));
            nalu_offsets[count_nalu_offsets - 1] = i;
        }
    }

    // 将所有完整的nalu提取出来，保存在双向链表中
    for (int i = 1; i < count_nalu_offsets; ++i)
    {
        struct h264_NALU *nalu = calloc(1, sizeof(struct h264_NALU));
        nalu->m.size = nalu_offsets[i] - nalu_offsets[i - 1];
        nalu->m.buffer = calloc(nalu->m.size, sizeof(uint8_t));
        nalu->m.index = ctx->count_nalus++;
        memcpy(nalu->m.buffer, ctx->pending_buffer + nalu_offsets[i - 1], nalu->m.size);

        if (ctx->nalus_head == NULL)
        {
            ctx->nalus_head = nalu;
            ctx->nalus_rear = nalu;
        }
        else
        {
            nalu->prev = ctx->nalus_rear;
            ctx->nalus_rear->next = nalu;

            ctx->nalus_rear = nalu;
        }
    }

    // 剩余的部分挪到内存开头
    size_t rest_size = ctx->pending_size - nalu_offsets[count_nalu_offsets - 1];
    if (rest_size > 0)
    {
        memcpy(ctx->pending_buffer, ctx->pending_buffer + nalu_offsets[count_nalu_offsets - 1], rest_size);
        ctx->pending_buffer = realloc(ctx->pending_buffer, rest_size);
        ctx->pending_size = rest_size;
    }

    free(nalu_offsets);

    return H264_SUCCESS;
}

// 对每个nalu进行解析
static int parse_nalu(struct h264_decoder_context *ctx)
{
    struct h264_NALU *nalu = NULL;
    if (ctx->nalu_proc == NULL)
    {
        nalu = ctx->nalus_head;
    }
    else
    {
        nalu = ctx->nalu_proc->next;
    }

    for (; nalu != NULL; nalu = nalu->next)
    {
        read_nal_unit(&nalu->m);
        switch (nalu->m.header.nal_unit_type)
        {
        case H264_NAL_SLICE:
            read_slice_header(ctx->curr_sps, &nalu->m);
            break;
        case H264_NAL_IDR_SLICE:
            read_slice_header(ctx->curr_sps, &nalu->m);
            break;
        case H264_NAL_SEI:
            read_sei_rbsp(&nalu->m);
            break;
        case H264_NAL_SPS:
            read_seq_parameter_set_rbsp(&nalu->m);
            ctx->curr_sps = &nalu->m.rbsp.sps;
            break;
        case H264_NAL_PPS:
            read_pic_paramster_set_rbsp(&nalu->m);
            ctx->curr_pps = &nalu->m.rbsp.pps;
            break;
        default:
            logerror("unhandled nal unit type: %d", nalu->m.header.nal_unit_type);
            break;
        }

        dump_nal_unit(ctx->fp_dump_info, &nalu->m);
    }

    ctx->nalu_proc = ctx->nalus_rear;

    return H264_SUCCESS;
}

static int process(struct h264_decoder_context *ctx)
{
    int ret = split_nalu(ctx);
    if (ret != H264_SUCCESS)
    {
        logerror("split_nalu failed");
        return ret;
    }

    ret = parse_nalu(ctx);
    if (ret != H264_SUCCESS)
    {
        logerror("parse_nalu failed");
        return ret;
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
    ctx->pending_buffer = realloc(ctx->pending_buffer, ctx->pending_size + stream->size);
    memcpy(ctx->pending_buffer + ctx->pending_size, stream->buffer, stream->size);
    ctx->pending_size += stream->size;

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

    struct h264_NALU *nalu = ctx->nalus_head;

    while (nalu)
    {
        // loginfo("index: %d, size: %lu, rbsp_trailing_bits: %d", nalu->index, nalu->size, nalu->count_rbsp_trailing_bits);
        // loginfo("header: %02x, forbidden_zero_bit: %d, nal_ref_idc: %d, nal_unit_type: %d",
        //     nalu->header.byte, nalu->header.forbidden_zero_bit, nalu->header.nal_ref_idc, nalu->header.nal_unit_type);

        nalu = nalu->next;
    }
}