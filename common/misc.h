#ifndef __MISC_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdbool.h>
#include <stdint.h>

    // 7.2

    // - 如果比特流中的当前位置在字节边界上，即比特流中的下一个比特是一个字节的第
    // 一个比特，则 byte_aligned() 的返回值为 TRUE。
    // - 否则，byte_aligned() 的返回值为 FALSE。
    bool byte_aligned();

    // 该函数仅在附录 B 指定的字节流 NAL 单元语法结构中使用，具体定义如下：
    // - 如果字节流中还有更多数据，more_data_in_byte_stream() 的返回值为 TRUE。
    // - 否则，more_data_in_byte_stream() 的返回值为 FALSE。
    bool more_data_in_byte_stream();

    // - 如果 RBSP 中没有更多数据，more_rbsp_data() 的返回值为 FALSE。
    // - 否则，在 RBSP 中搜索最后一个（最低有效的、最右边的）比特等于 1 的位置，
    // 该比特是 rbsp_trailing_bits() 语法结构中的第一个比特（rbsp_stop_one_bit）。
    // 根据该比特的位置，以下情况适用：
    //   - 如果 RBSP 中在 rbsp_trailing_bits() 语法结构之前有更多数据，
    // more_rbsp_data() 的返回值为 TRUE。
    //   - 否则，more_rbsp_data() 的返回值为 FALSE。
    bool more_rbsp_data();

    // 用于确定 RBSP 中是否有更多数据的方法由应用程序指定（或在使用字节流格式的
    // 应用程序中由附录 B 指定）。

    // - 如果 RBSP 中有更多数据，more_rbsp_trailing_data() 的返回值为 TRUE。
    // - 否则，more_rbsp_trailing_data() 的返回值为 FALSE。
    bool more_rbsp_trailing_data();

    // 该函数在不推进比特流指针的情况下提供比特流中的下 n 个比特供比较使用。提供
    // 对比特流中下 n 个比特的查看，n 是其参数。在附录 B 指定的字节流中使用时，
    // 如果字节流中剩余的比特少于 n，比特流中返回值为 0。
    uint8_t next_bits(int n);

    // 读取比特流中的下 n 个比特，并将比特流指针前进 n 个比特位置。当 n 等于 0
    // 时，read_bits(n) 指定返回值等于 0，并且不前进比特流指针。
    uint8_t read_bits(uint8_t *buffer, int *bit_offset, int n);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __MISC_H__