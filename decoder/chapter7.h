#ifndef CHAPTER7_H__
#define CHAPTER7_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"

    enum H264_MB_PART_PRED_MODE MbPartPredMode(
        int32_t transform_size_8x8_flag, enum H264_SLICE_TYPE slice_type,
        int32_t mb_type, int32_t mbPartIdx);
    enum H264_MB_PART_PRED_MODE SubMbPredMode(int32_t sub_my_type);
    int32_t NumMbPart(int32_t mb_type);
    int32_t NumSubMbPart(int32_t sub_my_type);
    int32_t MbPartWidth(
        enum H264_SLICE_TYPE slice_type,
        int32_t mb_type);
    int32_t MbPartHeight(
        enum H264_SLICE_TYPE slice_type,
        int32_t mb_type);
    int32_t SubMbPartWidth(
        enum H264_SLICE_TYPE slice_type,
        int32_t sub_mb_type);
    int32_t SubMbPartHeight(
        enum H264_SLICE_TYPE slice_type,
        int32_t sub_mb_type);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CHAPTER7_H__