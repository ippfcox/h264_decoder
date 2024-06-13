#ifndef SUPPLEMENTAL_ENHANCEMENT_INFORMATION_H__
#define SUPPLEMENTAL_ENHANCEMENT_INFORMATION_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "common/h264.h"

    void read_sei_rbsp(struct NAL_unit *nal);

    void dump_sei_rbsp(FILE *fp, struct NAL_unit *nal);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SUPPLEMENTAL_ENHANCEMENT_INFORMATION_H__