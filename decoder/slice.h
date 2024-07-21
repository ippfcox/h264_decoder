#ifndef SLICE_H__
#define SLICE_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "bit_stream.h"
#include "slice_common.h"
#include "nal_unit.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"

    void slice_layer_without_partitioning_rbsp(
        struct slice_layer_without_partitioning_t *slwp,
        struct bit_stream *bs,
        struct nal_unit_t *nu,
        struct seq_parameter_set_rbsp_t *spsrs,
        struct pic_parameter_set_rbsp_t *ppsrs);

    void dump_slice_layer_without_partitioning_rbsp(FILE *fp, struct slice_layer_without_partitioning_t *slwp);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SLICE_H__