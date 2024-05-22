#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "decoder.h"
#include "log.h"

struct h264_decoder_context
{
    struct h264_decoder_config cfg;
};

int h264_decoder_create(h264_decoder_handle *h, struct h264_decoder_config *cfg)
{
    if (!h || !cfg)
    {
        log_("invalid argument\n");
        return H264_INVALID_ARGUMENT;
    }

    struct h264_decoder_context *ctx = calloc(1, sizeof(struct h264_decoder_context));
    if (!ctx)
    {
        log_("calloc failed: %s\n", strerror(errno));
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
    
}