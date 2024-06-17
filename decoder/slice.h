#ifndef SLICE_H__
#define SLICE_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "common/h264.h"

    void read_slice_header(struct seq_parameter_set *sps, struct NAL_unit *nal);

    void dump_slice_header(FILE *fp, struct NAL_unit *nal);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SLICE_H__