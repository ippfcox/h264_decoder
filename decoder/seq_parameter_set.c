#include <stdlib.h>
#include <math.h>
#include "Exp-Golomb.h"
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
    spsd->seq_parameter_set_id = exp_golomb_ue(bs);
    // 7.4.2.1.1 Sequence parameter set data semantics
    // When chroma_format_idc is not present, it shall be inferred to be equal to 1 (4:2:0 chroma format).
    spsd->chroma_format_idc = 1;
    if (spsd->profile_idc == 100 || spsd->profile_idc == 110 ||
        spsd->profile_idc == 122 || spsd->profile_idc == 244 || spsd->profile_idc == 44 ||
        spsd->profile_idc == 83 || spsd->profile_idc == 86 || spsd->profile_idc == 118 ||
        spsd->profile_idc == 128 || spsd->profile_idc == 138 || spsd->profile_idc == 139 ||
        spsd->profile_idc == 134 || spsd->profile_idc == 135)
    {
        spsd->chroma_format_idc = exp_golomb_ue(bs);
        if (spsd->chroma_format_idc == 3)
        {
            spsd->separate_colour_plane_flag = bs_u(bs, 1);
        }
        spsd->bit_depth_luma_minus8 = exp_golomb_ue(bs);
        spsd->bit_depth_chroma_minus8 = exp_golomb_ue(bs);
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
    spsd->log2_max_frame_num_minus4 = exp_golomb_ue(bs);
    spsd->pic_order_cnt_type = exp_golomb_ue(bs);
    if (spsd->pic_order_cnt_type == 0)
    {
        spsd->log2_max_pic_order_cnt_lsb_minus4 = exp_golomb_ue(bs);
    }
    else if (spsd->pic_order_cnt_type == 1)
    {
        spsd->delta_pic_order_always_zero_flag = bs_u(bs, 1);
        spsd->offset_for_non_ref_pic = exp_golomb_se(bs);
        spsd->offset_for_top_to_bottom_field = exp_golomb_se(bs);
        spsd->num_ref_frames_in_pic_order_cnt_cycle = exp_golomb_ue(bs);
        spsd->offset_for_ref_frame = calloc(spsd->num_ref_frames_in_pic_order_cnt_cycle, sizeof(int32_t));
        for (int i = 0; i < spsd->num_ref_frames_in_pic_order_cnt_cycle; ++i)
        {
            spsd->offset_for_ref_frame[i] = exp_golomb_se(bs);
        }
    }

    spsd->max_num_ref_frames = exp_golomb_ue(bs);
    spsd->gaps_in_frame_num_value_allowed_flag = bs_u(bs, 1);
    spsd->pic_width_in_mbs_minus1 = exp_golomb_ue(bs);
    spsd->pic_height_in_map_units_minus1 = exp_golomb_ue(bs);
    spsd->frame_mbs_only_flag = bs_u(bs, 1);
    if (!spsd->frame_mbs_only_flag)
        spsd->mb_adaptive_frame_field_flag = bs_u(bs, 1);
    spsd->direct_8x8_inference_flag = bs_u(bs, 1);
    spsd->frame_cropping_flag = bs_u(bs, 1);
    if (spsd->frame_cropping_flag)
    {
        spsd->frame_crop_left_offset = exp_golomb_ue(bs);
        spsd->frame_crop_right_offset = exp_golomb_ue(bs);
        spsd->frame_crop_top_offset = exp_golomb_ue(bs);
        spsd->frame_crop_bottom_offset = exp_golomb_ue(bs);
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
#define dv(indents, placeholder, member) dump_value(indents, placeholder, spsr->spsd, member)
#define da(indents, placeholder, member, count) dump_array(indents, placeholder, spsr->spsd, member, count)

    dv(1, "u", profile_idc);
    dv(1, "u", constraint_set0_flag);
    dv(1, "u", constraint_set1_flag);
    dv(1, "u", constraint_set2_flag);
    dv(1, "u", constraint_set3_flag);
    dv(1, "u", constraint_set4_flag);
    dv(1, "u", constraint_set5_flag);
    dv(1, "u", reserved_zero_2bits);
    dv(1, "u", level_idc);
    dv(1, "u", seq_parameter_set_id);
    dv(2, "u", chroma_format_idc);
    dv(3, "u", separate_colour_plane_flag);
    dv(2, "u", bit_depth_luma_minus8);
    dv(2, "u", bit_depth_chroma_minus8);
    dv(2, "u", qpprime_y_zero_transform_bypass_flag);
    dv(2, "u", seq_scaling_matrix_present_flag);
    if (spsr->spsd.seq_scaling_matrix_present_flag)
        da(4, "u", seq_scaling_list_present_flag, (spsr->spsd.chroma_format_idc != 3 ? 8 : 12));
    dv(1, "u", log2_max_frame_num_minus4);
    dv(1, "u", pic_order_cnt_type);
    dv(2, "u", log2_max_pic_order_cnt_lsb_minus4);
    dv(2, "u", delta_pic_order_always_zero_flag);
    dv(2, "d", offset_for_non_ref_pic);
    dv(2, "d", offset_for_top_to_bottom_field);
    dv(2, "u", num_ref_frames_in_pic_order_cnt_cycle);
    da(4, "d", offset_for_ref_frame, spsr->spsd.num_ref_frames_in_pic_order_cnt_cycle);
    dv(1, "u", max_num_ref_frames);
    dv(1, "u", gaps_in_frame_num_value_allowed_flag);
    dv(1, "u", pic_width_in_mbs_minus1);
    dv(1, "u", pic_height_in_map_units_minus1);
    dv(1, "u", frame_mbs_only_flag);
    dv(2, "u", mb_adaptive_frame_field_flag);
    dv(1, "u", direct_8x8_inference_flag);
    dv(1, "u", frame_cropping_flag);
    dv(2, "u", frame_crop_left_offset);
    dv(2, "u", frame_crop_right_offset);
    dv(2, "u", frame_crop_top_offset);
    dv(2, "u", frame_crop_bottom_offset);
    dv(1, "u", vui_parameters_present_flag);
    dump_video_usability_information(fp, &spsr->spsd.vui);

    dv(1, "u", SubWidthC);
    dv(1, "u", SubHeightC);
    dv(1, "u", MbWidthC);
    dv(1, "u", MbHeightC);
    dv(1, "u", ChromaArrayType);
    dv(1, "u", BitDepthY);
    dv(1, "u", QpBdOffsetY);
    dv(1, "u", BitDepthC);
    dv(1, "u", QpBdOffsetC);
    dv(1, "u", RawMbBits);
    da(1, "u", Flat_4x4_16, 16);
    da(1, "u", Flat_8x8_16, 64);
    dv(1, "lu", MaxFrameNum);
    dv(1, "lu", MaxPicOrderCntLsb);
    dv(1, "lu", ExpectedDeltaPerPicOrderCntCycle);
    dv(1, "u", PicWidthInMbs);
    dv(1, "u", PicWidthInSamplesL);
    dv(1, "u", PicWidthInSamplesC);
    dv(1, "u", PicHeightInMapUnits);
    dv(1, "u", PicSizeInMapUnits);
    dv(1, "u", FrameHeightInMbs);
    dv(1, "u", CropUnitX);
    dv(1, "u", CropUnitY);
#undef da
#undef dv

    fprintf(fp, "\n");
}