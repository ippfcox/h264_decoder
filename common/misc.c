#include <math.h>
#include "misc.h"

// size: byte, bit_offset: bit, n: bit
uint64_t next_bits(uint8_t *buffer, size_t size, int bit_offset, int n)
{
    if (n + bit_offset > size * 8 || n < 0 || n > 64)
        return 0;

    uint64_t result = 0;
    buffer += bit_offset / 8;                // 指向目标字节起始字节
    int bit_offset_in_byte = bit_offset % 8; // 起始点在目标字节中的偏移量

    // 当差的字节数小于等于0，说明起始的这1个字节就够用了
    if (n + bit_offset_in_byte < 8)
    {
        return (*buffer >> (8 - (n + bit_offset_in_byte))) & make_mask(n);
    }
    else
    {
        // 第一部分，起始字节的剩余bit
        n -= (8 - bit_offset_in_byte);
        result |= ((*buffer & make_mask(8 - bit_offset_in_byte))) << n;
        // 第二部分，涉及的所有完整字节
        while (n >= 8)
        {
            n -= 8;
            buffer += 1;
            result |= *buffer << n;
        }
        // 第三部分，末尾字节的前面部分
        if (n > 0)
        {
            buffer += 1;
            result |= (*buffer >> n) & make_mask(n);
        }

        return result;
    }
}

uint64_t read_bits(uint8_t *buffer, size_t size, int *bit_offset, int n)
{
    if (n + *bit_offset > size * 8 || n < 0 || n > 64)
        return 0;

    *bit_offset += n;

    return next_bits(buffer, size, *bit_offset - n, n);
}

// 9.1 Parsing process for Exp-Golomb codes
// (9-1)
// (9-2)
// Table 9-1
// Table 9-2
uint64_t read_ue_v(uint8_t *buffer, size_t size, int *bit_offset)
{
    int leadingZeroBits = -1;
    for (uint8_t b = 0; !b; leadingZeroBits++)
        b = read_bits(buffer, size, bit_offset, 1);

    return pow(2, leadingZeroBits) - 1 + read_bits(buffer, size, bit_offset, leadingZeroBits);
}

// 9.1.1 Mapping process for signed Exp-Golomb codes
// Table 9-3
int32_t read_se_v(uint8_t *buffer, size_t size, int *bit_offset)
{
    uint64_t codeNum = read_ue_v(buffer, size, bit_offset);

    return codeNum % 2 == 0 ? -1 * (codeNum >> 2) : codeNum >> 2;
}