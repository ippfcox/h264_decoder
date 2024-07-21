#ifndef SLICE_HEADER_H__
#define SLICE_HEADER_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "bit_stream.h"
#include "slice_common.h"
#include "nal_unit.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"

    void slice_header(
        struct slice_header_t *sh,
        struct bit_stream *bs,
        struct nal_unit_t *nu,
        struct seq_parameter_set_rbsp_t *spsrs,
        struct pic_parameter_set_rbsp_t *ppsrs);

    int32_t NextMbAddress(struct slice_header_t *sh, int32_t n);

    void dump_slice_header(FILE *fp, struct slice_header_t *sh);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SLICE_HEADER_H__