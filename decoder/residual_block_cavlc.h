#ifndef RESIDUAL_BLOCK_CAVLC_H__
#define RESIDUAL_BLOCK_CAVLC_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"

    // 7.3.5.3.2 Residual block CAVLC syntax
    struct residual_block_cavlc_t
    {
        int32_t coeff_token;                 // ce(v)
        uint8_t trailing_ones_sign_flag : 1; // u(1)

        // level_prefix 3 | 4 ce(v)
        // level_suffix 3 | 4 u(v)
        // total_zeros 3 | 4 ce(v)
        // run_before 3 | 4 ce(v)
    };

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // RESIDUAL_BLOCK_CAVLC_H__