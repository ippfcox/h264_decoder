#ifndef DECODER_H__
#define DECODER_H__

#include <stdio.h>

#define H264_SUCCESS 0
#define H264_INVALID_ARGUMENT -1
#define H264_MEM_ALLOC_FAILED -2

typedef void *h264_decoder_handle;

struct h264_decoder_config
{
};

int h264_decoder_create(h264_decoder_handle *h, struct h264_decoder_config *cfg);
void h264_decoder_destroy(h264_decoder_handle h);

struct h264_stream
{
    void *buffer;
    size_t size;
};

int h264_decoder_send_stream(h264_decoder_handle h, struct h264_stream *stream);

struct h264_frame
{
};

int h264_decoder_get_frame(h264_decoder_handle h, struct h264_frame *frame);

#endif // DECODER_H__