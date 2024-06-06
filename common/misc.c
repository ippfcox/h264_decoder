#include "misc.h"

uint8_t read_bits(uint8_t *buffer, int *bit_offset, int n)
{
    uint8_t byte0 = *(buffer + *bit_offset / 8);
    uint8_t byte1 = *(buffer + *bit_offset / 8 + 1);
    int bit_offset_in_byte = *bit_offset % 8;

    int rest = 8 - bit_offset_in_byte - n;
    *bit_offset += n;

    if (rest >= 0)
    {
        return (byte0 >> rest) & make_mask_8(n);
    }
    else
    {
        return ((byte0 & make_mask_8(8 - bit_offset_in_byte)) << -rest) | ((byte1 >> -rest) & make_mask_8(-rest));
    }
}
