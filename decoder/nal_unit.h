#ifndef NAL_UNIT_H__
#define NAL_UNIT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "common/h264.h"

#define NALU_STARTCODE_LEN 3 // [TODO] 暂时认为startcode长度为3

    void read_nal_unit(struct NAL_unit *nal);

    void dump_nal_unit(FILE *fp, struct NAL_unit *nal);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // NAL_UNIT_H__
