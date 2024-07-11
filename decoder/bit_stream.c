#include "bit_stream.h"
#include "misc.h"

bool bs_byte_aligned(struct bit_stream *bs)
{
    return bs->bit_offset % 8 == 0;
}

bool bs_more_data_in_byte_stream(struct bit_stream *bs)
{
    // [TODO]
    return false;
}

bool bs_more_rbsp_data(struct bit_stream *bs)
{
    // [TODO]
    return false;
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
    // [TODO]
    return 0;
}

uint8_t bs_b8(struct bit_stream *bs)
{
    return bs_read_bits(bs, 8);
}

uint64_t bs_f(struct bit_stream *bs, int n)
{
    return bs_read_bits(bs, n);
}

int32_t bs_i(struct bit_stream *bs, int n);
uint64_t bs_me(struct bit_stream *bs);

// 9.1.1 Mapping process for signed Exp-Golomb codes
// Table 9-3
int32_t bs_se(struct bit_stream *bs)
{
    uint64_t codeNum = bs_ue(bs);

    return codeNum % 2 == 0 ? -1 * (codeNum >> 1) : (codeNum + 1) >> 1; // ceil when odd
}

int32_t bs_st(struct bit_stream *bs);
int32_t bs_te(struct bit_stream *bs);

int32_t bs_u(struct bit_stream *bs, int n)
{
    return bs_read_bits(bs, n);
}

// 9.1 Parsing process for Exp-Golomb codes
// (9-1)
// (9-2)
// Table 9-1
// Table 9-2
int32_t bs_ue(struct bit_stream *bs)
{
    int leadingZeroBits = -1;
    for (uint8_t b = 0; !b; leadingZeroBits++)
        b = bs_read_bits(bs, 1);

    return (1 << leadingZeroBits) - 1 + bs_read_bits(bs, leadingZeroBits);
}