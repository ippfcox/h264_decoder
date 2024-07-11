#ifndef SLICE_DATA_H__
#define SLICE_DATA_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"
#include "nal_unit.h"
#include "slice_header.h"
#include "seq_parameter_set.h"
#include "pic_parameter_set.h"

    // 7.3.4 Slice data syntax
    // 7.4.4 Slice data semantics
    struct slice_data_t
    {
        uint8_t cabac_alignment_one_bit : 1; // f(1)
        uint32_t mb_skip_run;                // ue(v)
        uint32_t mb_skip_flag;               // ae(v)
        uint32_t mb_field_decoding_flag;     // u(1) | ae(v)
        uint32_t end_of_slice_flag;          // ae(v)
    };

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