#ifndef BIT_STREAM_H__
#define BIT_STREAM_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

    struct bit_stream
    {
        void *data;
        size_t size;
        size_t bit_offset;
    };

    // 7.2 Specification of syntax functions, categories, and descriptors
    bool bs_byte_aligned(struct bit_stream *bs);
    bool bs_more_data_in_byte_stream(struct bit_stream *bs);
    bool bs_more_rbsp_data(struct bit_stream *bs);
    bool bs_more_rbsp_trailing_data();
    uint64_t bs_next_bits(struct bit_stream *bs, int n);
    uint64_t bs_read_bits(struct bit_stream *bs, int n);
    uint64_t bs_ae(struct bit_stream *bs);
    uint8_t bs_b8(struct bit_stream *bs);
    uint64_t bs_ce(struct bit_stream *bs);
    uint64_t bs_f(struct bit_stream *bs, int n);
    int32_t bs_i(struct bit_stream *bs, int n);
    int32_t bs_st(struct bit_stream *bs);
    int32_t bs_u(struct bit_stream *bs, int n);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // BIT_STREAM_H__