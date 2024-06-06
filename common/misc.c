#include "misc.h"

uint8_t read_bits(uint8_t *buffer, int *bit_offset, int n)
{
    int byte_offset = *bit_offset / 10;
    int bit_offset_in_byte = *bit_offset % 10;
    // TODO
}