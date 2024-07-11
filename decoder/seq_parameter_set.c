#include <stdlib.h>
#include <math.h>
#include "seq_parameter_set.h"
#include "misc.h"
#include "common/log.h"

struct chroma_format
{
    uint32_t chroma_format_idc;
    uint8_t separate_colour_plane_flag;
    enum CHROMA_FORMAT chroma_format;
    uint8_t SubWidthC;
    uint8_t SubHeightC;
};

struct chroma_format chroma_formats[] = {
    {0, 0, CHROMA_MONOCHROME, -1, -1},
    {1, 0, CHROMA_420, 2, 2},
    {2, 0, CHROMA_422, 2, 1},
    {3, 0, CHROMA_444, 1, 1},
    {3, 1, CHROMA_444, -1, -1},
};

// 7.3.2.1.1 Sequence parameter set data syntax
// seq_parameter_set_data()
static void seq_parameter_set_data(struct seq_parameter_set_data_t *spsd, struct bit_stream *bs)
{
    spsd->profile_idc = bs_u(bs, 8);
    spsd->constraint_set0_flag = bs_u(bs, 1);
    spsd->constraint_set1_flag = bs_u(bs, 1);
    spsd->constraint_set2_flag = bs_u(bs, 1);
    spsd->constraint_set3_flag = bs_u(bs, 1);
    spsd->constraint_set4_flag = bs_u(bs, 1);
    spsd->constraint_set5_flag = bs_u(bs, 1);
    spsd->reserved_zero_2bits = bs_u(bs, 2);
    spsd->level_idc = bs_u(bs, 8);
    spsd->seq_parameter_set_id = bs_ue(bs);
    // 7.4.2.1.1 Sequence parameter set data semantics
    // When chroma_format_idc is not present, it shall be inferred to be equal to 1 (4:2:0 chroma format).
    spsd->chroma_format_idc = 1;
    if (spsd->profile_idc == 100 || spsd->profile_idc == 110 ||
        spsd->profile_idc == 122 || spsd->profile_idc == 244 || spsd->profile_idc == 44 ||
        spsd->profile_idc == 83 || spsd->profile_idc == 86 || spsd->profile_idc == 118 ||
        spsd->profile_idc == 128 || spsd->profile_idc == 138 || spsd->profile_idc == 139 ||
        spsd->profile_idc == 134 || spsd->profile_idc == 135)
    {
        spsd->chroma_format_idc = bs_ue(bs);
        if (spsd->chroma_format_idc == 3)
        {
            spsd->separate_colour_plane_flag = bs_u(bs, 1);
        }
        spsd->bit_depth_luma_minus8 = bs_ue(bs);
        spsd->bit_depth_chroma_minus8 = bs_ue(bs);
        spsd->qpprime_y_zero_transform_bypass_flag = bs_u(bs, 1);
        spsd->seq_scaling_matrix_present_flag = bs_u(bs, 1);
        if (spsd->seq_scaling_matrix_present_flag)
        {
            spsd->seq_scaling_list_present_flag = calloc((spsd->chroma_format_idc != 3 ? 8 : 12), sizeof(uint8_t));
            for (int i = 0; i < (spsd->chroma_format_idc != 3 ? 8 : 12); ++i)
            {
                spsd->seq_scaling_list_present_flag[i] = bs_u(bs, 1);
                if (spsd->seq_scaling_list_present_flag[i])
                {
                    if (i < 6)
                    {
                        // [TODO]
                    }
                    else
                    {
                        // [TODO]
                    }
                }
            }
        }
    }
    spsd->log2_max_frame_num_minus4 = bs_ue(bs);
    spsd->pic_order_cnt_type = bs_ue(bs);
    if (spsd->pic_order_cnt_type == 0)
    {
        spsd->log2_max_pic_order_cnt_lsb_minus4 = bs_ue(bs);
    }
    else if (spsd->pic_order_cnt_type == 1)
    {
        spsd->delta_pic_order_always_zero_flag = bs_u(bs, 1);
        spsd->offset_for_non_ref_pic = bs_se(bs);
        spsd->offset_for_top_to_bottom_field = bs_se(bs);
        spsd->num_ref_frames_in_pic_order_cnt_cycle = bs_ue(bs);
        spsd->offset_for_ref_frame = calloc(spsd->num_ref_frames_in_pic_order_cnt_cycle, sizeof(int32_t));
        for (int i = 0; i < spsd->num_ref_frames_in_pic_order_cnt_cycle; ++i)
        {
            spsd->offset_for_ref_frame[i] = bs_se(bs);
        }
    }

    spsd->max_num_ref_frames = bs_ue(bs);
    spsd->gaps_in_frame_num_value_allowed_flag = bs_u(bs, 1);
    spsd->pic_width_in_mbs_minus1 = bs_ue(bs);
    spsd->pic_height_in_map_units_minus1 = bs_ue(bs);
    spsd->frame_mbs_only_flag = bs_u(bs, 1);
    if (!spsd->frame_mbs_only_flag)
        spsd->mb_adaptive_frame_field_flag = bs_u(bs, 1);
    spsd->direct_8x8_inference_flag = bs_u(bs, 1);
    spsd->frame_cropping_flag = bs_u(bs, 1);
    if (spsd->frame_cropping_flag)
    {
        spsd->frame_crop_left_offset = bs_ue(bs);
        spsd->frame_crop_right_offset = bs_ue(bs);
        spsd->frame_crop_top_offset = bs_ue(bs);
        spsd->frame_crop_bottom_offset = bs_ue(bs);
    }
    spsd->vui_parameters_present_flag = bs_u(bs, 1);
    if (spsd->vui_parameters_present_flag)
    {
        vui_parameters(&spsd->vui, bs);
    }

    if (spsd->chroma_format_idc != 0 && spsd->separate_colour_plane_flag != 1)
    {
        spsd->SubWidthC = chroma_formats[spsd->chroma_format_idc].SubWidthC;
        spsd->SubHeightC = chroma_formats[spsd->chroma_format_idc].SubHeightC;
    }
    // [TODO] check SubWidthC and SubHeightC
    spsd->MbWidthC = 16 / spsd->SubWidthC;
    spsd->MbHeightC = 16 / spsd->SubHeightC;
    if (spsd->separate_colour_plane_flag == 0)
    {
        spsd->ChromaArrayType = spsd->chroma_format_idc;
    }
    else if (spsd->separate_colour_plane_flag == 1)
    {
        spsd->ChromaArrayType = 0;
    }
    spsd->BitDepthY = 8 + spsd->bit_depth_luma_minus8;
    spsd->QpBdOffsetY = 6 * spsd->bit_depth_luma_minus8;
    spsd->BitDepthC = 8 + spsd->bit_depth_chroma_minus8;
    spsd->QpBdOffsetC = 6 * spsd->bit_depth_chroma_minus8;
    spsd->RawMbBits = 256 * spsd->BitDepthY + 2 * spsd->MbWidthC * spsd->MbHeightC * spsd->BitDepthC;
    spsd->MaxFrameNum = pow(2, spsd->log2_max_frame_num_minus4 + 4);
    spsd->MaxPicOrderCntLsb = pow(2, spsd->log2_max_pic_order_cnt_lsb_minus4 + 4);
    spsd->ExpectedDeltaPerPicOrderCntCycle = 0;
    for (int i = 0; i < spsd->num_ref_frames_in_pic_order_cnt_cycle; ++i)
    {
        spsd->ExpectedDeltaPerPicOrderCntCycle += spsd->offset_for_ref_frame[i];
    }
    spsd->PicWidthInMbs = spsd->pic_width_in_mbs_minus1 + 1;
    spsd->PicWidthInSamplesL = spsd->PicWidthInMbs * 16;
    spsd->PicWidthInSamplesC = spsd->PicWidthInMbs * spsd->MbWidthC;
    spsd->PicHeightInMapUnits = spsd->pic_height_in_map_units_minus1 + 1;
    spsd->PicSizeInMapUnits = spsd->PicWidthInMbs * spsd->PicHeightInMapUnits;
    spsd->FrameHeightInMbs = (2 - spsd->frame_mbs_only_flag) * spsd->PicHeightInMapUnits;
    if (spsd->ChromaArrayType == 0)
    {
        spsd->CropUnitX = 1;
        spsd->CropUnitY = 2 - spsd->frame_mbs_only_flag;
    }
    else if (spsd->ChromaArrayType == 1 || spsd->ChromaArrayType == 2 || spsd->ChromaArrayType == 3)
    {
        spsd->CropUnitX = spsd->SubWidthC;
        spsd->CropUnitY = spsd->SubHeightC * (2 - spsd->frame_mbs_only_flag);
    }
}

// 7.3.2.1 Sequence parameter set RBSP syntax
// seq_parameter_set_rbsp()
void seq_parameter_set_rbsp(struct seq_parameter_set_rbsp_t *spsr, struct bit_stream *bs)
{
    seq_parameter_set_data(&spsr->spsd, bs);
}

void free_seq_parameter_set(struct seq_parameter_set_rbsp_t *spsr)
{
    if (!spsr)
        return;
    if (spsr->spsd.seq_scaling_list_present_flag)
        free(spsr->spsd.seq_scaling_list_present_flag);
    if (spsr->spsd.offset_for_ref_frame)
        free(spsr->spsd.offset_for_ref_frame);
}

void dump_seq_parameter_set_rbsp(FILE *fp, struct seq_parameter_set_rbsp_t *spsr)
{
#define dump(indents, name, placeholder) fprintf(fp, "%s%s: %" placeholder "\n", make_indents(indents), #name, spsr->spsd.name)

    dump(1, profile_idc, "u");
    dump(1, constraint_set0_flag, "u");
    dump(1, constraint_set1_flag, "u");
    dump(1, constraint_set2_flag, "u");
    dump(1, constraint_set3_flag, "u");
    dump(1, constraint_set4_flag, "u");
    dump(1, constraint_set5_flag, "u");
    dump(1, reserved_zero_2bits, "u");
    dump(1, level_idc, "u");
    dump(1, seq_parameter_set_id, "u");
    dump(2, chroma_format_idc, "u");
    dump(3, separate_colour_plane_flag, "u");
    dump(2, bit_depth_luma_minus8, "u");
    dump(2, bit_depth_chroma_minus8, "u");
    dump(2, qpprime_y_zero_transform_bypass_flag, "u");
    dump(2, seq_scaling_matrix_present_flag, "u");
    if (spsr->spsd.seq_scaling_list_present_flag)
        for (int i = 0; i < (spsr->spsd.chroma_format_idc != 3 ? 8 : 12); ++i)
            dump(4, seq_scaling_list_present_flag[i], "u");
    dump(1, log2_max_frame_num_minus4, "u");
    dump(1, pic_order_cnt_type, "u");
    dump(2, log2_max_pic_order_cnt_lsb_minus4, "u");
    dump(2, delta_pic_order_always_zero_flag, "u");
    dump(2, offset_for_non_ref_pic, "d");
    dump(2, offset_for_top_to_bottom_field, "d");
    dump(2, num_ref_frames_in_pic_order_cnt_cycle, "u");
    if (spsr->spsd.offset_for_ref_frame)
        for (int i = 0; i < spsr->spsd.num_ref_frames_in_pic_order_cnt_cycle; ++i)
            dump(4, offset_for_ref_frame[i], "d");
    dump(1, max_num_ref_frames, "u");
    dump(1, gaps_in_frame_num_value_allowed_flag, "u");
    dump(1, pic_width_in_mbs_minus1, "u");
    dump(1, pic_height_in_map_units_minus1, "u");
    dump(1, frame_mbs_only_flag, "u");
    dump(2, mb_adaptive_frame_field_flag, "u");
    dump(1, direct_8x8_inference_flag, "u");
    dump(1, frame_cropping_flag, "u");
    dump(2, frame_crop_left_offset, "u");
    dump(2, frame_crop_right_offset, "u");
    dump(2, frame_crop_top_offset, "u");
    dump(2, frame_crop_bottom_offset, "u");
    dump(1, vui_parameters_present_flag, "u");
    if (spsr->spsd.vui_parameters_present_flag)
        dump_video_usability_information(fp, &spsr->spsd.vui);

    dump(1, SubWidthC, "u");
    dump(1, SubHeightC, "u");
    dump(1, MbWidthC, "u");
    dump(1, MbHeightC, "u");
    dump(1, ChromaArrayType, "u");
    dump(1, BitDepthY, "u");
    dump(1, QpBdOffsetY, "u");
    dump(1, BitDepthC, "u");
    dump(1, QpBdOffsetC, "u");
    dump(1, RawMbBits, "u");
    // uint8_t Flat_4x4_16[16];
    // uint8_t Flat_8x8_16[64];
    dump(1, MaxFrameNum, "lu");
    dump(1, MaxPicOrderCntLsb, "lu");
    dump(1, ExpectedDeltaPerPicOrderCntCycle, "lu");
    dump(1, PicWidthInMbs, "u");
    dump(1, PicWidthInSamplesL, "u");
    dump(1, PicWidthInSamplesC, "u");
    dump(1, PicHeightInMapUnits, "u");
    dump(1, PicSizeInMapUnits, "u");
    dump(1, FrameHeightInMbs, "u");
    dump(1, CropUnitX, "u");
    dump(1, CropUnitY, "u");
#undef dump

    fprintf(fp, "\n");
}