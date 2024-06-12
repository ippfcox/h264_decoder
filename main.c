#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "common/log.h"
#include "decoder/decoder.h"

struct context
{
    FILE *fp;
    size_t length;
};

void usage(const char *name)
{
    fprintf(stderr, "%s <filename>\n", name);
}

int main(int argc, char *argv[])
{
    set_log_level(LOG_LEVEL_DEBUG);

    struct context *ctx = calloc(1, sizeof(struct context));
    if (!ctx)
    {
        logerror("calloc failed: %s", strerror(errno));
        return -1;
    }

    if (argc < 2)
    {
        usage(argv[0]);
        goto error;
    }

    ctx->fp = fopen(argv[1], "rb");
    if (!ctx->fp)
    {
        logerror("fopen `%s` failed: %s", argv[1], strerror(errno));
        goto error;
    }

    fseek(ctx->fp, 0, SEEK_END);
    ctx->length = ftell(ctx->fp);
    rewind(ctx->fp);

    h264_decoder_handle decoder;
    struct h264_decoder_config cfg = {};
    int ret = h264_decoder_create(&decoder, &cfg);
    if (ret != H264_SUCCESS)
    {
        logerror("h264_decoder_create failed: %d", ret);
        return -1;
    }

    int size = 1024;
    uint8_t *buffer = calloc(size, sizeof(uint8_t));
    while (!feof(ctx->fp))
    {
        fread(buffer, sizeof(uint8_t), size, ctx->fp);
        struct h264_stream stream = {buffer, size};
        ret = h264_decoder_send_stream(decoder, &stream);
        if (ret < 0)
        {
            logerror("h264_decoder_send_stream failed: %d", ret);
            break;
        }
    }

    h264_decoder_debug(decoder);

error:
    if (!ctx)
        return 0;
    if (ctx->fp)
        fclose(ctx->fp);
    free(ctx);
}