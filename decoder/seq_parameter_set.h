#ifndef SEQ_PARAMETER_SET_H__
#define SEQ_PARAMETER_SET_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "common/h264.h"

    void read_seq_parameter_set_rbsp(struct NAL_unit *nal);
    void read_seq_parameter_set(struct NAL_unit *nal);

    void dump_seq_parameter_set(FILE *fp, struct NAL_unit *nal);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SEQ_PARAMETER_SET_H__