#ifndef SLICE_DATA_H__
#define SLICE_DATA_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "bit_stream.h"
#include "slice_common.h"
#include "nal_unit.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"

    void slice_data(
        struct slice_data_t *sd,
        struct bit_stream *bs,
        struct nal_unit_t *nu,
        struct slice_header_t *sh,
        struct seq_parameter_set_rbsp_t *spsrs,
        struct pic_parameter_set_rbsp_t *ppsrs);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SLICE_DATA_H__