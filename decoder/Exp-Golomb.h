#ifndef EXP_GOLOMB_H__
#define EXP_GOLOMB_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"

uint32_t exp_golomb_ue(struct bit_stream *bs);
int32_t exp_golomb_se(struct bit_stream *bs);
int32_t exp_golomb_me(struct bit_stream *bs, uint8_t ChromaArrayType, enum H264_MB_PART_PRED_MODE MbPartPredMode);
int32_t exp_golomb_te(struct bit_stream *bs, int x);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // EXP_GOLOMB_H__