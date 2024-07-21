#ifndef MACROBLOCK_H__
#define MACROBLOCK_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "bit_stream.h"
#include "slice_common.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"

    void macroblock_layer(
        struct macroblock_layer_t *ml,
        struct bit_stream *bs,
        struct slice_header_t *sh,
        struct slice_data_t *sd,
        struct seq_parameter_set_rbsp_t *spsrs,
        struct pic_parameter_set_rbsp_t *ppsrs,
        int32_t CurrMbAddr);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MACROBLOCK_H__