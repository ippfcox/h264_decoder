#include "h264.h"
#include "bit_stream.h"
#include "misc.h"
#include "common/log.h"

bool bs_byte_aligned(struct bit_stream *bs)
{
    return bs->bit_offset % 8 == 0;
}

bool bs_more_data_in_byte_stream(struct bit_stream *bs)
{
    // [TODO]
    return false;
}

// 7.2 Specification of syntax functions, categories, and descriptors 
bool bs_more_rbsp_data(struct bit_stream *bs)
{
    if (bs->size * 8 <= bs->bit_offset)
        return false;
    // find last non-zero byte
    size_t non_zero_byte_index = bs->size - 1;
    while (*(uint8_t *)(bs->data + non_zero_byte_index) == 0)
        --non_zero_byte_index;
    // find last 1 bit
    uint8_t non_zero_bit_index = 7;
    while (((*(uint8_t *)(bs->data + non_zero_byte_index) >> (7 - non_zero_bit_index)) & 0x01) == 0)
        --non_zero_bit_index;

    loginfo("%ld vs %d", non_zero_byte_index * 8 + non_zero_bit_index , bs->bit_offset);

    if (non_zero_byte_index * 8 + non_zero_bit_index <= bs->bit_offset)
        return false;

    return true;
}

// bool bs_more_rbsp_trailing_data();

uint64_t bs_next_bits(struct bit_stream *bs, int n)
{
    uint8_t *data = bs->data;

    if (n + bs->bit_offset > bs->size * 8 || n < 0 || n > 64)
        return 0;

    uint64_t result = 0;
    data += bs->bit_offset / 8;                  // 指向目标字节起始字节
    int bit_offset_in_byte = bs->bit_offset % 8; // 起始点在目标字节中的偏移量

    // 当差的字节数小于等于0，说明起始的这1个字节就够用了
    if (n + bit_offset_in_byte < 8)
    {
        return (*data >> (8 - (n + bit_offset_in_byte))) & make_mask(n);
    }
    else
    {
        // 第一部分，起始字节的剩余bit
        n -= (8 - bit_offset_in_byte);
        result |= ((*data & make_mask(8 - bit_offset_in_byte))) << n;
        // 第二部分，涉及的所有完整字节
        while (n >= 8)
        {
            n -= 8;
            data += 1;
            result |= *data << n;
        }
        // 第三部分，末尾字节的前面部分
        if (n > 0)
        {
            data += 1;
            result |= (*data >> (8 - n)) & make_mask(n);
        }

        return result;
    }
}

uint64_t bs_read_bits(struct bit_stream *bs, int n)
{
    if (n + bs->bit_offset > bs->size * 8 || n < 0 || n > 64)
        return 0;

    uint64_t value = bs_next_bits(bs, n);
    bs->bit_offset += n;

    return value;
}

// 9.3 CABAC parsing process for slice data
// This process is invoked when parsing syntax elements with descriptor ae(v) in clauses 7.3.4 and 7.3.5 when entropy_coding_mode_flag is equal to 1.
uint64_t bs_ae(struct bit_stream *bs)
{
    logfatal("cabac is not supported for now");
    // [TODO]
    return 0;
}

uint8_t bs_b8(struct bit_stream *bs)
{
    return bs_read_bits(bs, 8);
}

// 9.2?
uint64_t bs_ce(struct bit_stream *bs);

uint64_t bs_f(struct bit_stream *bs, int n)
{
    return bs_read_bits(bs, n);
}

int32_t bs_i(struct bit_stream *bs, int n);

int32_t bs_st(struct bit_stream *bs);

int32_t bs_u(struct bit_stream *bs, int n)
{
    return bs_read_bits(bs, n);
}
