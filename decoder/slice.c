#include "slice.h"
#include "slice_header.h"
#include "slice_data.h"

// 7.3.2.8 Slice layer without partitioning RBSP syntax
// slice_layer_without_partitioning_rbsp()
void slice_layer_without_partitioning_rbsp(
    struct slice_layer_without_partitioning_t *slwp,
    struct bit_stream *bs,
    struct nal_unit_t *nu,
    struct seq_parameter_set_rbsp_t *spsrs,
    struct pic_parameter_set_rbsp_t *ppsrs)
{
    slice_header(&slwp->sh, bs, nu, spsrs, ppsrs);
    slice_data(&slwp->sd, bs, nu, &slwp->sh, spsrs, ppsrs);
}

void dump_slice_layer_without_partitioning_rbsp(FILE *fp, struct slice_layer_without_partitioning_t *slwp)
{
    dump_slice_header(fp, &slwp->sh);
}