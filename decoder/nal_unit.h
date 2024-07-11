#ifndef NAL_UNIT_H__
#define NAL_UNIT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "h264.h"
#include "bit_stream.h"

#define NALU_STARTCODE_LEN 3 // [TODO] 暂时认为startcode长度为3

    // G.7.3.1.1 NAL unit header SVC extension syntax
    // G.7.4.1.1 NAL unit header SVC extension semantics
    struct nal_unit_header_svc_extension
    {
        uint8_t idr_flag : 1;                 // u(1)
        uint8_t priority_id : 6;              // u(6)
        uint8_t no_inter_layer_pred_flag : 1; // u(1)
        uint8_t dependency_id : 3;            // u(3)
        uint8_t quality_id : 4;               // u(4)
        uint8_t temporal_id : 3;              // u(3)
        uint8_t use_ref_base_pic_flag : 1;    // u(1)
        uint8_t discardable_flag : 1;         // u(1)
        uint8_t output_flag : 1;              // u(1)
        uint8_t reserved_three_2bits : 2;     // u(2)
    };

    // J.7.3.1.1 NAL unit header 3D-AVC extension syntax
    // J.7.4.1.1 NAL unit header MVC extension semantics
    struct nal_unit_header_3davc_extension
    {
        uint8_t view_idx;            // u(8)
        uint8_t depth_flag : 1;      // u(1)
        uint8_t non_idr_flag : 1;    // u(1)
        uint8_t temporal_id : 3;     // u(3)
        uint8_t anchor_pic_flag : 1; // u(1)
        uint8_t inter_view_flag : 1; // u(1)
    };

    // H.7.3.1.1 NAL unit header MVC extension syntax
    // J.7.4.1.1 NAL unit header MVC extension semantics
    struct nal_unit_header_mvc_extension
    {
        uint8_t non_idr_flag : 1;     // u(1)
        uint8_t priority_id : 6;      // u(6)
        uint16_t view_id : 10;        // u(10)
        uint8_t temporal_id : 3;      // u(3)
        uint8_t anchor_pic_flag : 1;  // u(1)
        uint8_t inter_view_flag : 1;  // u(1)
        uint8_t reserved_one_bit : 1; // u(1)
    };

    // 7.3.1 NAL unit syntax
    // 7.4.1 NAL unit semantics
    struct nal_unit_t
    {
        uint8_t forbidden_zero_bit : 1; // f(1)
        uint8_t nal_ref_idc : 2;        // u(2)
        uint8_t nal_unit_type : 5;      // u(5)
        uint8_t svc_extension_flag : 1; // u(1)
        struct nal_unit_header_svc_extension svc_extension;
        uint8_t avc_3d_extension_flag : 1; // u(1)
        struct nal_unit_header_3davc_extension _3davc_extension;
        struct nal_unit_header_mvc_extension mvc_extension;
        uint8_t *rbsp_byte;                      // rbsp data
        uint8_t emulation_prevention_three_byte; // f(8)

        size_t NumBytesInNALunit;  // B.2
        bool IdrPicFlag;           // (7-1)
        bool DepthFlag;            // (7-2)
        size_t NumBytesInRBSP;     // 7.3.1 NAL unit syntax
        size_t nalUnitHeaderBytes; // 7.3.1 NAL unit syntax
    };

    // B.1.1 Byte stream NAL unit syntax
    // B.1.2 Byte stream NAL unit semantics
    struct byte_stream_nal_unit_t
    {
        uint8_t leading_zero_8bits;            // f(8)
        uint8_t zero_byte;                     // f(8)
        uint32_t start_code_prefix_one_3bytes; // f(24)
        struct nal_unit_t nu;
        uint8_t trailing_zero_8bits; // f(8)
    };

    void byte_stream_nal_unit(struct byte_stream_nal_unit_t *bsnu, size_t bsnu_size, struct bit_stream *bs);

    void dump_byte_stream_nal_unit(FILE *fp, struct byte_stream_nal_unit_t *bsnu, size_t index);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // NAL_UNIT_H__
