#ifndef H264_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdio.h>
#include <stdint.h>

    struct NAL_unit
    {
        uint8_t *buffer;
        size_t size;
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // H264_H__