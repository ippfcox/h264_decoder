#ifndef SLICE_H__
#define SLICE_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "slice_header.h"
#include "slice_data.h"

    // 7.3.2.8 Slice layer without partitioning RBSP syntax
    // 7.4.2.8 Slice layer without partitioning RBSP semantics
    struct slice_layer_without_partitioning_t
    {
        struct slice_header_t sh;
        struct slice_data_t sd;
        // rbsp_trailing
    };

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