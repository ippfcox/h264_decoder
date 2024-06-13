#ifndef PIC_PARAMETER_SET_H__
#define PIC_PARAMETER_SET_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "common/h264.h"

    void read_pic_paramster_set_rbsp(struct NAL_unit *nal);

    void dump_pic_parameter_set(FILE *fp, struct NAL_unit *nal);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PIC_PARAMETER_SET_H__