#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include "decoder.h"
#include "common/h264.h"
#include "common/log.h"

// NALU: Network Abstraction Layer Unit
// EBSP: Encapsulated Byte Sequence Packets
// RBSP: Raw Byte Sequence Payload
// SODB: String Of Data Bits
// EPB: Emulation Prevention Bytes

#define NALU_STARTCODE_LEN 4 // [TODO] 暂时认为startcode长度为4
uint8_t startcode[NALU_STARTCODE_LEN] = {0x00, 0x00, 0x00, 0x01};

// Table 7-1 - NAL unit type codes
enum
{
    H264_NAL_UNSPECIFIED = 0,
    H264_NAL_SLICE = 1,
    H264_NAL_DPA = 2,
    H264_NAL_DPB = 3,
    H264_NAL_DPC = 4,
    H264_NAL_IDR_SLICE = 5,
    H264_NAL_SEI = 6,
    H264_NAL_SPS = 7,
    H264_NAL_PPS = 8,
    H264_NAL_AUD = 9,
    H264_NAL_END_SEQUENCE = 10,
    H264_NAL_END_STREAM = 11,
    H264_NAL_FILLER_DATA = 12,
    H264_NAL_SPS_EXT = 13,
    H264_NAL_PREFIX = 14,
    H264_NAL_SUB_SPS = 15,
    H264_NAL_DPS = 16,
    H264_NAL_RESERVED17 = 17,
    H264_NAL_RESERVED18 = 18,
    H264_NAL_AUXILIARY_SLICE = 19,
    H264_NAL_EXTEN_SLICE = 20,
    H264_NAL_DEPTH_EXTEN_SLICE = 21,
    H264_NAL_RESERVED22 = 22,
    H264_NAL_RESERVED23 = 23,
    H264_NAL_UNSPECIFIED24 = 24,
    H264_NAL_UNSPECIFIED25 = 25,
    H264_NAL_UNSPECIFIED26 = 26,
    H264_NAL_UNSPECIFIED27 = 27,
    H264_NAL_UNSPECIFIED28 = 28,
    H264_NAL_UNSPECIFIED29 = 29,
    H264_NAL_UNSPECIFIED30 = 30,
    H264_NAL_UNSPECIFIED31 = 31,
};

// 处理nalu信息
struct h264_NALU
{
    struct NAL_unit m;

    int index; // 调试用的序号
    // int count_rbsp_trailing_bits;

    // // parse
    // union
    // {
    //     struct
    //     {
    //         uint8_t nal_unit_type : 5;      // 0-4
    //         uint8_t nal_ref_idc : 2;        // 5-6
    //         uint8_t forbidden_zero_bit : 1; // 7
    //     };
    //     uint8_t byte;
    // } header;

    struct h264_NALU *prev;
    struct h264_NALU *next;
};

struct h264_decoder_context
{
    struct h264_decoder_config cfg;

    uint8_t *pending_buffer;
    size_t pending_size;

    // head指向第一个nalu，rear指向最后一个nalu，proc指向处理过的最后一个nalu
    struct h264_NALU *nalus_head, *nalus_rear, *nalu_proc;
    int count_nalus;
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
        log_warn("wait more stream (%lu < %d)", ctx->pending_size, NALU_STARTCODE_LEN);
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
        nalu->index = ctx->count_nalus++;
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

// ebsp是去掉startcode
// static int get_ebsp(int startcode_len, struct h264_nalu *nalu)
// {
//     nalu->size -= startcode_len;
//     memcpy(nalu->buffer, nalu->buffer + startcode_len, nalu->size);
//     // nalu->buffer = realloc(nalu->buffer, nalu->size); // 有必要吗
//     return H264_SUCCESS;
// }

// // rbsp是去掉防竞争字节：00 00 [03] 00/01/02/03中的[03]
// static int get_rbsp(struct h264_nalu *nalu)
// {
//     uint8_t epb_prefix[] = {0x00, 0x00, 0x03};
//     for (int i = 0; i < nalu->size - 3; ++i)
//     {
//         if (check_code(nalu->buffer + i, epb_prefix, 3))
//         {
//             if (nalu->buffer[i + 3] == 0x00 || nalu->buffer[i + 3] == 0x01 || nalu->buffer[i + 3] == 0x02 || nalu->buffer[i + 3] == 0x03)
//             {
//                 log_debug("[%d]before:\t%02x %02x %02x %02x %02x", nalu->index, nalu->buffer[i + 0], nalu->buffer[i + 1], nalu->buffer[i + 2], nalu->buffer[i + 3], nalu->buffer[i + 4]);
//                 nalu->size -= 1;
//                 memcpy(nalu->buffer + i + 2, nalu->buffer + i + 3, nalu->size - (i + 2));
//             }
//             log_debug("[%d]after:\t%02x %02x %02x %02x", nalu->index, nalu->buffer[i + 0], nalu->buffer[i + 1], nalu->buffer[i + 2], nalu->buffer[i + 3]);
//         }
//     }
//     return H264_SUCCESS;
// }

// static int get_sodb(struct h264_nalu *nalu)
// {
//     uint8_t last_byte = nalu->buffer[nalu->size - 1];

//     for (int i = 1; i <= 8; ++i)
//     {
//         if (((last_byte >> i) & 0x01) == 0x01)
//         {
//             nalu->count_rbsp_trailing_bits = i;
//             break;
//         }
//     }
//     return H264_SUCCESS;
// }

static void read_nal_unit(struct NAL_unit *nal)
{
    uint8_t *ptr = nal->buffer + NALU_STARTCODE_LEN;
    size_t num_bytes_in_NAL_unit = nal->size;

    
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
        // 此处，nalu还是原始数据
        // get_ebsp(NALU_STARTCODE_LEN, nalu);
        // 此处，nalu被去掉了startcode
        // get_rbsp(nalu);
        // // 此处，nalu被去掉了防竞争字节
        // get_sodb(nalu);
        // // 此处，计算得到了最后的补齐用的bit长度，buffer本身没有被修改
        // nalu->header.byte = nalu->buffer[0];
        // // 此处，nalu的第一个字节得到了解析
        // switch (nalu->header.nal_unit_type)
        // {
        // case H264_NAL_SLICE:
        //     break;
        // case H264_NAL_SPS:
        //     break;
        // case H264_NAL_PPS:
        //     break;
        // default:
        //     log_error("unhandled nal unit type: %d", nalu->header.nal_unit_type);
        //     break;
        // }
    }

    ctx->nalu_proc = ctx->nalus_rear;

    return H264_SUCCESS;
}

static int process(struct h264_decoder_context *ctx)
{
    int ret = split_nalu(ctx);
    if (ret != H264_SUCCESS)
    {
        log_error("split_nalu failed");
        return ret;
    }

    ret = parse_nalu(ctx);
    if (ret != H264_SUCCESS)
    {
        log_error("parse_nalu failed");
        return ret;
    }

    return H264_SUCCESS;
}

int h264_decoder_create(h264_decoder_handle *h, struct h264_decoder_config *cfg)
{
    if (!h || !cfg)
    {
        log_error("invalid argument");
        return H264_INVALID_ARGUMENT;
    }

    struct h264_decoder_context *ctx = calloc(1, sizeof(struct h264_decoder_context));
    if (!ctx)
    {
        log_error("calloc failed: %s", strerror(errno));
        return H264_MEM_ALLOC_FAILED;
    }

    // [TODO] check config

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
        log_error("invalid argument");
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
        log_error("invalid argument");
        return;
    }

    struct h264_NALU *nalu = ctx->nalus_head;

    while (nalu)
    {
        // log_info("index: %d, size: %lu, rbsp_trailing_bits: %d", nalu->index, nalu->size, nalu->count_rbsp_trailing_bits);
        // log_info("header: %02x, forbidden_zero_bit: %d, nal_ref_idc: %d, nal_unit_type: %d",
        //     nalu->header.byte, nalu->header.forbidden_zero_bit, nalu->header.nal_ref_idc, nalu->header.nal_unit_type);

        nalu = nalu->next;
    }
}