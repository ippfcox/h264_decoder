#ifndef __MISC_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "h264.h"

#define max(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define min(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define clip(_min, _max, _val) min(max((_min), (_val)), (_max))
#define align_up(_val, _align) (((_val) + (_align) - 1) / (_align) * (_align))
#define align_down(_val, _align) ((_val) / (_align) * (_align))

// 0~64bit
#define make_mask(n) (n == 64 ? ~0UL : (1UL << (n)) - 1)

    extern char spaces64[];
// 16 tab indent
#define make_indents(n) spaces64 + (64 - 4 * (n))

    const char *nal_type_name(enum H264_NAL_TYPE type);

    int get_log2(uint64_t value);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __MISC_H__