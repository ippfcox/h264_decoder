#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "log.h"

struct context
{
    FILE *fp;
    size_t length;
};

void usage(const char *name)
{
    log_("%s <filename>\n", name);
}

int main(int argc, char *argv[])
{
    struct context *ctx = calloc(1, sizeof(struct context));
    if (!ctx)
    {
        log_("calloc failed: %s\n", strerror(errno));
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
        log_("fopen `%s` failed: %s\n", argv[1], strerror(errno));
        goto error;
    }

    fseek(ctx->fp, 0, SEEK_END);
    ctx->length = ftell(ctx->fp);
    rewind(ctx->fp);

error:
    if(!ctx)
        return 0;
    if (ctx->fp)
        fclose(ctx->fp);
    free(ctx);
}