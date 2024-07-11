#ifndef GROUP_OF_PICTURES_H__
#define GROUP_OF_PICTURES_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "seq_parameter_set.h"
#include "pic_parameter_set.h"

#define MAX_SPS_COUNT 32
#define MAX_PPS_COUNT 32

    struct group_of_pictures
    {
        struct seq_parameter_set_rbsp_t spsrs[MAX_SPS_COUNT];
        struct pic_parameter_set_rbsp_t ppsrs[MAX_PPS_COUNT];
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // GROUP_OF_PICTURES_H__