翻译（ChatGPT-4o）

## 7 语法和语义

### 7.1 以表格形式指定语法的方法

语法表格指定了所有允许的比特流的语法的超集。在其他条款中，可能会直接或间接地规定对语法的附加约束。
  > **注** – 实际解码器应实现识别比特流入口点的方法，并能够识别和处理不符合规范的比特流。识别和处理错误以及其他类似情况的方法在此未作规定。

下表列出了用于描述语法的伪代码示例。当出现`syntax_element`时，它表明从比特流中解析出一个语法元素，并且在比特流解析过程中，比特流指针将前进到语法元素后的下一个位置。

|                                                                                                                                                                                       | Category | Descriptor |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------- | ---------- |
| /\*语句可以是具有相关语法类别和描述的语法元素，也可以是用于指定语法元素存在、类型和数量的条件表达式，如以下两个示例所示。\*/                                                          |          |            |
| **`syntax_element`**                                                                                                                                                                  | 3        | ue(v)      |
| `conditioning statement`                                                                                                                                                              |          |            |
|                                                                                                                                                                                       |          |            |
| /\*用花括号括起来的一组语句是复合语句，并且在功能上被视为单一语句。\*/                                                                                                                |          |            |
| `{`                                                                                                                                                                                   |          |            |
| `    statement`                                                                                                                                                                       |          |            |
| `    statement`                                                                                                                                                                       |          |            |
| `    ...`                                                                                                                                                                             |          |            |
| `}`                                                                                                                                                                                   |          |            |
| /\*“while”结构指定对`condition`是否为真进行测试，如果为真，则指定重复执行一个语句（或复合语句），直到`condition`不再为真。\*/                                                         |          |            |
| `while(condition)`                                                                                                                                                                    |          |            |
| `    statement`                                                                                                                                                                       |          |            |
|                                                                                                                                                                                       |          |            |
| /\*“do … while”结构指定先执行一次`statement`，然后测试`condition`是否为真，如果为真，则指定重复执行该语句，直到条件不再为真。\*/                                                      |          |            |
| `do`                                                                                                                                                                                  |          |            |
| `    statement`                                                                                                                                                                       |          |            |
| `while(condition)`                                                                                                                                                                    |          |            |
|                                                                                                                                                                                       |          |            |
| /\*“if … else”结构指定对条件是否为真进行测试，如果条件为真，则指定执行一个主要语句，否则，指定执行一个替代语句。如果不需要执行替代语句，则省略结构中的“else”部分和相关的替代语句。\*/ |          |            |
| `if(condition)`                                                                                                                                                                       |          |            |
| `    primary statement`                                                                                                                                                               |          |            |
| `else`                                                                                                                                                                                |          |            |
| `    alternative statement`                                                                                                                                                           |          |            |
|                                                                                                                                                                                       |          |            |
| /\*“for”结构指定首先执行一个初始语句，然后测试一个条件，如果条件为真，则指定重复执行一个主要语句，并在每次执行后执行一个后续语句，直到条件不再为真。\*/                               |          |            |
| `for(initial statement; condition; subsequent statement)`                                                                                                                             |          |            |
| `    primary statement`                                                                                                                                                               |          |            |

### 7.2 语法功能、类别和描述符的规定

本文介绍的功能用于语法描述。这些功能假设存在一个比特流指针，该指针指示解码过程中从比特流中读取下一个比特的位置。

**byte_aligned()** 的定义如下：
- 如果比特流中的当前位置在字节边界上，即比特流中的下一个比特是一个字节中的第一个比特，则 byte_aligned() 的返回值为 TRUE。
- 否则，byte_aligned() 的返回值为 FALSE。

**more_data_in_byte_stream()** 仅在附录B中规定的字节流 NAL 单元语法结构中使用，其定义如下：
- 如果字节流中有更多数据，则 more_data_in_byte_stream() 的返回值为 TRUE。
- 否则，more_data_in_byte_stream() 的返回值为 FALSE。

**more_rbsp_data()** 的定义如下：
- 如果 RBSP 中没有更多数据，则 more_rbsp_data() 的返回值为 FALSE。
- 否则，在 RBSP 中搜索最后一个等于 1 的比特（即 rbsp_trailing_bits() 语法结构的第一个比特 rbsp_stop_one_bit），根据这个比特的位置，适用以下情况：
  - 如果在 rbsp_trailing_bits() 语法结构之前的 RBSP 中有更多数据，则 more_rbsp_data() 的返回值为 TRUE。
  - 否则，more_rbsp_data() 的返回值为 FALSE。

确定 RBSP 中是否有更多数据的方法由应用程序规定（或在使用字节流格式的应用程序中在附录 B 中规定）。

**more_rbsp_trailing_data()** 的定义如下：
- 如果 RBSP 中有更多数据，则 more_rbsp_trailing_data() 的返回值为 TRUE。
- 否则，more_rbsp_trailing_data() 的返回值为 FALSE。

**next_bits(n)** 提供比特流中的下一个比特以供比较使用，而不前进比特流指针。它查看比特流中的下 n 个比特，n 为其参数。当在附录B中规定的字节流中使用时，如果字节流中剩余的比特少于 n 个，next_bits(n) 的返回值为 0。

**read_bits(n)** 从比特流中读取下 n 个比特，并将比特流指针前进 n 个比特位置。当 n 等于 0 时，read_bits(n) 被规定返回值为 0 并且不前进比特流指针。

**类别（在表中标记为 C）** 指定将片数据分区为最多三个片数据分区。片数据分区 A 包含所有类别 2 的语法元素。片数据分区 B 包含所有类别 3 的语法元素。片数据分区 C 包含所有类别 4 的语法元素。其他类别值的含义未作规定。对于某些语法元素，使用竖线分隔的两个类别值。在这些情况下，将在文本中进一步指定应用的类别值。对于在其他语法结构中使用的语法结构，列出了在包含的语法结构中找到的所有语法元素的类别，使用竖线分隔。标记为“所有”的类别的语法元素或语法结构出现在包含该语法元素或语法结构的所有语法结构中。对于在其他语法结构中使用的语法结构，在包含的语法结构中包含类别标记为“所有”的语法元素的位置处提供的数值类别被视为适用于类别“所有”的语法元素。

以下描述符指定了每个语法元素的解析过程。对于某些语法元素，使用了两个描述符，并以竖线分隔。在这些情况下，当 `entropy_coding_mode_flag` 等于 0 时，左边的描述符适用；当 `entropy_coding_mode_flag` 等于 1 时，右边的描述符适用。

- **ae(v)**: 上下文自适应算术熵编码语法元素。此描述符的解析过程在第 9.3 条中规定。
- **b(8)**: 具有任意位串模式的字节（8 位）。此描述符的解析过程由函数 `read_bits(8)` 的返回值指定。
- **ce(v)**: 上下文自适应变长熵编码语法元素，左位优先。此描述符的解析过程在第 9.2 条中规定。
- **f(n)**: 使用 n 位的固定模式位串，从左到右写入，左位优先。此描述符的解析过程由函数 `read_bits(n)` 的返回值指定。
- **i(n)**: 使用 n 位的有符号整数。当语法表中 n 为 "v" 时，位数根据其他语法元素的值而变化。此描述符的解析过程由函数 `read_bits(n)` 的返回值指定，并解释为最重要位优先的二进制补码整数表示。
- **me(v)**: 使用左位优先的映射 Exp-Golomb 编码语法元素。此描述符的解析过程在第 9.1 条中规定。
- **se(v)**: 使用左位优先的有符号整数 Exp-Golomb 编码语法元素。此描述符的解析过程在第 9.1 条中规定。
- **st(v)**: 使用 ISO/IEC 10646 规定的通用编码字符集 (UCS) 传输格式-8 (UTF-8) 字符编码的空终止字符串。解析过程如下规定：`st(v)` 从比特流中的字节对齐位置开始，读取并返回一系列字节，从当前位置开始，直到但不包括下一个等于 0x00 的字节对齐字节，并将比特流指针前进（`stringLength + 1`）* 8 个比特位置，其中 `stringLength` 等于返回的字节数。
  > **注** – `st(v)` 语法描述符仅在比特流中的当前位置是字节对齐位置时在本规范中使用。
- **te(v)**: 使用左位优先的截断 Exp-Golomb 编码语法元素。此描述符的解析过程在第 9.1 条中规定。
- **u(n)**: 使用 n 位的无符号整数。当语法表中 n 为 "v" 时，位数根据其他语法元素的值而变化。此描述符的解析过程由函数 `read_bits(n)` 的返回值指定，并解释为最重要位优先的无符号整数的二进制表示。
- **ue(v)**: 使用左位优先的无符号整数 Exp-Golomb 编码语法元素。此描述符的解析过程在第 9.1 条中规定。

### 7.3 略

### 7.4 语义

本条款规定了与语法结构及这些结构内的语法元素相关联的语义。当使用表格或一组表格来指定语法元素的语义时，除非本建议书 | 国际标准另有规定，表格中未指定的任何值不应出现在比特流中。

#### 7.4.1 NAL 单元语义

  > **注 1** – 设计视频编码层 (VCL) 的目的是高效表示视频数据的内容。网络提取层 (NAL) 的设计目的是对这些数据进行格式化，并以适合在各种通信通道或存储介质上传输的方式提供头信息。所有数据都包含在 NAL 单元中，每个 NAL 单元包含一个整数数量的字节。NAL 单元为分组导向和比特流系统指定了通用格式。分组导向传输和字节流的 NAL 单元格式是相同的，唯一的区别是在字节流格式中，每个 NAL 单元可以以起始码前缀和额外填充字节开头。

**NumBytesInNALunit** 指定 NAL 单元的字节大小。此值是解码 NAL 单元所必需的。需要某种形式的 NAL 单元边界标记方法，以便推断 **NumBytesInNALunit**。其中一种标记方法在附录 B 中针对字节流格式进行了规定。其他标记方法可能在本建议书 | 国际标准之外进行规定。

- **forbidden_zero_bit** 必须等于 0。

- **nal_ref_idc** 不等于 0 表示 NAL 单元的内容包含序列参数集、序列参数集扩展、子集序列参数集、图像参数集、参考图像的一片、参考图像的一片数据分区或在参考图像的一片之前的前缀 NAL 单元。

对于符合附录 A 中规定的一个或多个配置文件的编码视频序列，这些序列使用第 2 至第 9 条中规定的解码过程进行解码，当 NAL 单元包含一片或一片数据分区且 **nal_ref_idc** 等于 0 时，表示该片或片数据分区是非参考图像的一部分。

- **nal_ref_idc** 对于序列参数集、序列参数集扩展、子集序列参数集或图像参数集 NAL 单元，不应等于 0。当一个 NAL 单元的 **nal_unit_type** 在 1 到 4（含）范围内时，其 **nal_ref_idc** 等于 0，那么对于该图像的所有 **nal_unit_type** 在 1 到 4（含）范围内的 NAL 单元，其 **nal_ref_idc** 都应等于 0。

- **nal_ref_idc** 对于 **nal_unit_type** 等于 5 的 NAL 单元，不应等于 0。

- **nal_ref_idc** 对于 **nal_unit_type** 等于 6、9、10、11 或 12 的所有 NAL 单元，应等于 0。

- **nal_unit_type** 指定 NAL 单元中包含的 RBSP 数据结构的类型，如表 7-1 所示。

表 7-1 中标记为“C”的列列出了 NAL 单元中可能存在的语法元素的类别。此外，根据 RBSP 数据结构的语法和语义，可能存在语法类别为“所有”的语法元素。特定类别语法元素的存在与否由相关 RBSP 数据结构的语法和语义决定。**nal_unit_type** 不应等于 3 或 4，除非在 RBSP 数据结构中至少存在一个语法元素，其语法元素类别值等于 **nal_unit_type** 的值并且未分类为“所有”。

对于符合附录 A 中规定的一个或多个配置文件并使用第 2 至 9 条规定的解码过程进行解码的编码视频序列，VCL 和非 VCL NAL 单元在表 7-1 中的“附录 A NAL 单元类型类”列中指定。对于符合附录 G 中规定的一个或多个配置文件并使用附录 G 规定的解码过程进行解码的编码视频序列，以及符合附录 H 中规定的一个或多个配置文件并使用附录 H 规定的解码过程进行解码的编码视频序列，VCL 和非 VCL NAL 单元在表 7-1 中的“附录 G 和附录 H NAL 单元类型类”列中指定。对于 **nal_unit_type** 等于 14 的条目“后缀依赖”规定如下：

- 如果在解码顺序中直接跟随 **nal_unit_type** 等于 14 的 NAL 单元是 **nal_unit_type** 等于 1 或 5 的 NAL 单元，则 **nal_unit_type** 等于 14 的 NAL 单元是 VCL NAL 单元。
- 否则（在解码顺序中直接跟随 **nal_unit_type** 等于 14 的 NAL 单元是 **nal_unit_type** 不等于 1 或 5 的 NAL 单元），**nal_unit_type** 等于 14 的 NAL 单元是非 VCL NAL 单元。解码器应忽略（从比特流中移除并丢弃）**nal_unit_type** 等于 14 的 NAL 单元及其后（按解码顺序）的 **nal_unit_type** 等于 14 的 NAL 单元。

Table 7-1略

当编码视频序列中存在 **nal_unit_type** 等于 13 或 19 的 NAL 单元时，解码器应执行针对这些 NAL 单元的（可选）解码过程，或者忽略（从比特流中移除并丢弃）这些 NAL 单元的内容。

符合附录 A 规定的一个或多个配置文件而非附录 G 或 H 规定的配置文件的解码器应忽略（从比特流中移除并丢弃）所有 **nal_unit_type** 等于 14、15 或 20 的 NAL 单元的内容。

使用 **nal_unit_type** 等于 0 或在 24 到 31 范围内（含）的 NAL 单元不应影响本建议书 | 国际标准中规定的解码过程。
  > **注 2** – **nal_unit_type** 为 0 和 24 到 31 的 NAL 单元可根据应用程序的需要使用。本建议书 | 国际标准未规定这些 **nal_unit_type** 值的解码过程。由于不同应用程序可能将 **nal_unit_type** 为 0 和 24 到 31 的 NAL 单元用于不同的目的，因此在设计生成 **nal_unit_type** 为 0 或在 24 到 31 范围内（含）的 NAL 单元的编码器以及设计解释这些 NAL 单元内容的解码器时，必须特别小心。

解码器应忽略（从比特流中移除并丢弃）使用保留 **nal_unit_type** 值的所有 NAL 单元的内容。
  > **注 3** – 这一要求允许将来对本建议书 | 国际标准进行兼容扩展的定义。
  > **注 4** – 在本建议书 | 国际标准的早期版本中，**nal_unit_type** 为 13 到 15 和 19 到 20（或这些 NAL 单元类型的子集）是保留的，并且未规定这些 **nal_unit_type** 值的 NAL 单元的解码过程。在本建议书 | 国际标准的后续版本中，目前保留的 **nal_unit_type** 值可能会变为非保留，并且可能会规定这些 **nal_unit_type** 值的解码过程。编码器应考虑到在本建议书 | 国际标准的早期版本中保留的 **nal_unit_type** 值可能会被解码器忽略。

在文本中，编码片 NAL 单元统称为非 IDR 图像的编码片 NAL 单元或 IDR 图像的编码片 NAL 单元。变量 **IdrPicFlag** 定义如下：
```c
IdrPicFlag = ((nal_unit_type == 5) ? 1 : 0)
```
当包含某个特定图像的一片的 NAL 单元的 **nal_unit_type** 等于 5 时，该图像不应包含 **nal_unit_type** 在 1 到 4 范围内（含）的 NAL 单元。对于符合附录 A 中规定的一个或多个配置文件并使用第 2 至 9 条规定的解码过程进行解码的编码视频序列，这样的图像称为 IDR 图像。
  > **注 5** – IDR 图像不能使用片数据分区。

**svc_extension_flag** 指示语法结构中接下来是否会跟随 **nal_unit_header_svc_extension()** 或 **nal_unit_header_mvc_extension()**。

当 **svc_extension_flag** 未出现时，推断 **svc_extension_flag** 的值等于 0。

对于符合附录 G 中规定的一个或多个配置文件的编码视频序列，**svc_extension_flag** 的值应等于 1。符合附录 G 中规定的一个或多个配置文件的解码器应忽略（从比特流中移除并丢弃）**nal_unit_type** 等于 14 或 20 且 **svc_extension_flag** 等于 0 的 NAL 单元。

对于符合附录 H 中规定的一个或多个配置文件的编码视频序列，**svc_extension_flag** 的值应等于 0。符合附录 H 中规定的一个或多个配置文件的解码器应忽略（从比特流中移除并丢弃）**nal_unit_type** 等于 14 或 20 且 **svc_extension_flag** 等于 1 的 NAL 单元。

对于符合附录 I 中规定的一个或多个配置文件的编码视频序列，**svc_extension_flag** 的值应等于 0。符合附录 I 中规定的一个或多个配置文件的解码器应忽略（从比特流中移除并丢弃）**nal_unit_type** 等于 14、20 或 21 且 **svc_extension_flag** 等于 1 的 NAL 单元。

对于符合附录 J 中规定的一个或多个配置文件的编码视频序列，**svc_extension_flag** 的值应等于 0。符合附录 J 中规定的一个或多个配置文件的解码器应忽略（从比特流中移除并丢弃）**nal_unit_type** 等于 14 或 20 且 **svc_extension_flag** 等于 1 的 NAL 单元。

**avc_3d_extension_flag** 指示对于 **nal_unit_type** 等于 21 的 NAL 单元，接下来在语法结构中是 **nal_unit_header_mvc_extension()** 还是 **nal_unit_header_3davc_extension()**。

当 **avc_3d_extension_flag** 未出现时，推断 **avc_3d_extension_flag** 的值等于 0。

**DepthFlag** 的值规定如下：
```c
DepthFlag = (nal_unit_type != 21) ? 0 : (avc_3d_extension_flag ? depth_flag : 1)
```
对于符合附录 I 中规定的一个或多个配置文件的编码视频序列，**avc_3d_extension_flag** 的值应等于 0。符合附录 I 中规定的一个或多个配置文件的解码器应忽略（从比特流中移除并丢弃）**nal_unit_type** 等于 21 且 **avc_3d_extension_flag** 等于 1 的 NAL 单元。

**rbsp_byte[i]** 是 RBSP 的第 i 个字节。RBSP 规定为如下顺序排列的字节序列。

RBSP 包含一个 SODB，如下所示：
- 如果 SODB 为空（即长度为零比特），则 RBSP 也为空。
- 否则，RBSP 包含如下 SODB：
  1. RBSP 的第一个字节包含 SODB 的前八个比特（最重要的，最左边的）；RBSP 的下一个字节包含 SODB 的下八个比特，依此类推，直到 SODB 中剩余的比特少于八个。
  2. SODB 之后是 **rbsp_trailing_bits()** 语法结构，如下所示：
     - 最后的 RBSP 字节的第一个（最重要的，最左边的）比特包含 SODB 的剩余比特（如果有）。
     - 下一个比特是单个比特，等于 1（即 **rbsp_stop_one_bit**）。
     - 当 **rbsp_stop_one_bit** 不是字节对齐的最后一个比特时，会有一个或多个零值比特（即 **rbsp_alignment_zero_bit** 的实例）以实现字节对齐。
  3. 在 RBSP 的结尾，一些 RBSP 可能在 **rbsp_trailing_bits()** 之后包含一个或多个等于 0x0000 的 16 位语法元素 **cabac_zero_word**。

具有这些 RBSP 属性的语法结构在语法表中以 "_rbsp" 后缀表示。这些结构应作为 **rbsp_byte[i]** 数据字节的内容包含在 NAL 单元中。RBSP 语法结构与 NAL 单元的关联应按照表 7-1 的规定。
  > **注 6** – 当 RBSP 的边界已知时，解码器可以通过将 RBSP 字节的比特连接起来并丢弃 **rbsp_stop_one_bit**（即最后一个等于 1 的比特）和其后的所有零值比特，从 RBSP 中提取 SODB。解码过程中所需的数据包含在 RBSP 的 SODB 部分。

**emulation_prevention_three_byte** 是一个等于 0x03 的字节。当 **emulation_prevention_three_byte** 出现在 NAL 单元中时，解码过程中应将其丢弃。

NAL 单元的最后一个字节不得等于 0x00。

在 NAL 单元中，以下三个字节序列不得出现在任何字节对齐位置：
- 0x000000
- 0x000001
- 0x000002

在 NAL 单元中，任何以 0x000003 开头的四字节序列，除了以下序列，不得出现在任何字节对齐位置：
- 0x00000300
- 0x00000301
- 0x00000302
- 0x00000303
  > **注 7** – 当 **nal_unit_type** 等于 0 时，在编码器设计中必须特别注意，避免上述列出的三字节和四字节模式出现在 NAL 单元语法结构的开头，因为语法元素 **emulation_prevention_three_byte** 不能是 NAL 单元的第三个字节。

##### 7.4.1.1 将 SODB 封装在 RBSP 中（说明性）

本条款不构成本建议书 | 国际标准的组成部分。

将 SODB 封装在 RBSP 中的形式及在 NAL 单元内使用 **emulation_prevention_three_byte** 封装 RBSP 的目的是：

- 防止在 NAL 单元中仿真起始码，同时允许在 NAL 单元中表示任意的 SODB，
- 通过从 RBSP 末尾开始搜索 **rbsp_stop_one_bit** 来识别 NAL 单元中 SODB 的结尾，
- 在某些情况下，使 NAL 单元的大小大于 SODB 的大小（使用一个或多个 **cabac_zero_word**）。

编解码器可以通过以下步骤从 RBSP 生成 NAL 单元：

1. **在 RBSP 中搜索以下二进制模式的字节对齐位**：
   `'00000000 00000000 000000xx'`（其中 xx 代表任何 2 位模式：00、01、10 或 11），
   并插入一个字节等于 0x03，将这些位模式替换为：
   `'00000000 00000000 00000011 000000xx'`，并且，当 RBSP 数据的最后一个字节等于 0x00（这只能在 RBSP 以 cabac_zero_word 结尾时发生）时，追加一个字节等于 0x03 到数据的末尾。RBSP 数据中字节对齐的三个字节序列 0x000000 的最后一个零字节（被替换为四字节序列 0x00000300）在搜索 RBSP 数据中的下一个字节对齐位时需要考虑。

2. **将得到的字节序列前缀化如下**：
   - 如果 `nal_unit_type` 不等于 14 或 20，字节序列前缀为包含语法元素 `forbidden_zero_bit`、`nal_ref_idc` 和 `nal_unit_type` 的 NAL 单元的第一字节，其中 `nal_unit_type` 指示 NAL 单元包含的 RBSP 数据结构的类型。
   - 否则（`nal_unit_type` 等于 14 或 20），字节序列前缀为 NAL 单元的前四个字节，其中第一字节包含语法元素 `forbidden_zero_bit`、`nal_ref_idc` 和 `nal_unit_type`，接下来的三字节包含语法结构 `nal_unit_header_svc_extension()`。第一字节中的语法元素 `nal_unit_type` 指示在接下来的三字节中存在语法结构 `nal_unit_header_svc_extension()` 和 NAL 单元包含的 RBSP 数据结构的类型。

上述过程生成了整个 NAL 单元。

此过程可以允许在 NAL 单元中表示任何 SODB，同时确保：
- 在 NAL 单元中不会仿真字节对齐的起始码前缀，
- 在 NAL 单元中不会仿真任何由 8 个零值位跟随起始码前缀的序列，无论是否字节对齐。

### 7.4.1.2 NAL 单元的顺序及其与编码图像、接入单元和视频序列的关联

本条款规定了比特流中 NAL 单元顺序的约束。

任何遵守这些约束的比特流中 NAL 单元的顺序在文本中称为 NAL 单元的解码顺序。在 NAL 单元内，第 7.3 条、第 D.1 条和第 E.1 条的语法规定了语法元素的解码顺序。解码器应能够按照解码顺序接收 NAL 单元及其语法元素。

### 7.4.1.2.1 序列和图像参数集 RBSP 的顺序及其激活

本条款规定了符合附录 A 中规定的一个或多个配置文件并使用第 2 至 9 条中规定的解码过程的编码视频序列的图像和序列参数集的激活过程。
  > **注 1** – 序列和图像参数集机制将不常变化的信息的传输与编码宏块数据的传输解耦。在某些应用中，序列和图像参数集可以使用可靠的传输机制“带外”传输。

一个图像参数集 RBSP 包含可以由一个或多个编码图像的编码片 NAL 单元或编码片数据分区 A NAL 单元引用的参数。在解码过程开始时，每个图像参数集 RBSP 初始时被认为是非激活状态。在解码过程中，最多只有一个图像参数集 RBSP 在任意时刻被认为是激活状态，并且激活任何特定的图像参数集 RBSP 会导致之前激活的图像参数集 RBSP（如果有的话）被停用。

当一个图像参数集 RBSP（具有特定值的 `pic_parameter_set_id`）未激活且被编码片 NAL 单元或编码片数据分区 A NAL 单元引用（使用该 `pic_parameter_set_id` 值）时，它会被激活。该图像参数集 RBSP 被称为活动图像参数集 RBSP，直到通过激活另一个图像参数集 RBSP 使其停用。具有该特定 `pic_parameter_set_id` 值的图像参数集 RBSP 应在其激活之前可供解码过程使用。

对于一个编码图像，任何包含活动图像参数集 RBSP 的 `pic_parameter_set_id` 值的图像参数集 NAL 单元应与该编码图像的活动图像参数集 RBSP 的内容相同，除非它出现在该编码图像的最后一个 VCL NAL 单元之后且在另一个编码图像的第一个 VCL NAL 单元之前。

当接收到具有特定 `pic_parameter_set_id` 值的图像参数集 NAL 单元时，其内容将替换解码顺序中具有相同 `pic_parameter_set_id` 值的前一个图像参数集 NAL 单元的内容（如果比特流中存在具有相同 `pic_parameter_set_id` 值的前一个图像参数集 NAL 单元）。
  > **注 2** – 解码器必须能够同时存储所有 `pic_parameter_set_id` 值的图像参数集的内容。当接收到具有相同 `pic_parameter_set_id` 值的新图像参数集 NAL 单元时，特定 `pic_parameter_set_id` 值的图像参数集内容将被覆盖。

一个序列参数集 RBSP 包含可以由一个或多个图像参数集 RBSP 或包含缓冲期 SEI 消息的一个或多个 SEI NAL 单元引用的参数。在解码过程开始时，每个序列参数集 RBSP 初始时被认为是非激活状态。在解码过程中，最多只有一个序列参数集 RBSP 在任意时刻被认为是激活状态，并且激活任何特定的序列参数集 RBSP 会导致之前激活的序列参数集 RBSP（如果有的话）被停用。

当一个序列参数集 RBSP（具有特定值的 `seq_parameter_set_id`）未激活且被图像参数集 RBSP（使用该 `seq_parameter_set_id` 值）的激活或包含缓冲期 SEI 消息的 SEI NAL 单元引用时，它会被激活。该序列参数集 RBSP 被称为活动序列参数集 RBSP，直到通过激活另一个序列参数集 RBSP 使其停用。具有该特定 `seq_parameter_set_id` 值的序列参数集 RBSP 应在其激活之前可供解码过程使用。激活的序列参数集 RBSP 应在整个编码视频序列中保持激活状态。
  > **注 3** – 因为 IDR 接入单元开始一个新的编码视频序列，并且激活的序列参数集 RBSP 必须在整个编码视频序列中保持激活状态，所以序列参数集 RBSP 只能通过 IDR 接入单元中的缓冲期 SEI 消息激活。

对于一个编码视频序列，任何包含活动序列参数集 RBSP 的 `seq_parameter_set_id` 值的序列参数集 NAL 单元应与该编码视频序列的活动序列参数集 RBSP 的内容相同，除非它出现在该编码视频序列的最后一个接入单元之后且在另一个编码视频序列的第一个 VCL NAL 单元和包含缓冲期 SEI 消息的第一个 SEI NAL 单元（如果存在）之前。
  > **注 4** – 如果图像参数集 RBSP 或序列参数集 RBSP 在比特流中传输，这些约束对包含图像参数集 RBSP 或序列参数集 RBSP 的 NAL 单元施加顺序约束。否则（图像参数集 RBSP 或序列参数集 RBSP 通过本建议书 | 国际标准未规定的其他方式传输），它们必须及时可供解码过程使用，以便遵守这些约束。

当接收到具有特定 `seq_parameter_set_id` 值的序列参数集 NAL 单元时，其内容将替换解码顺序中具有相同 `seq_parameter_set_id` 值的前一个序列参数集 NAL 单元的内容（如果比特流中存在具有相同 `seq_parameter_set_id` 值的前一个序列参数集 NAL 单元）。
  > **注 5** – 解码器必须能够同时存储所有 `seq_parameter_set_id` 值的序列参数集的内容。当接收到具有相同 `seq_parameter_set_id` 值的新序列参数集 NAL 单元时，特定 `seq_parameter_set_id` 值的序列参数集内容将被覆盖。

当序列参数集扩展 RBSP 存在时，它包含的参数与序列参数集 RBSP 的参数功能相似。为了建立序列参数集扩展 RBSP 的语法元素约束和确定序列参数集扩展 RBSP 的激活目的，序列参数集扩展 RBSP 应被视为具有相同 `seq_parameter_set_id` 的前一个序列参数集 RBSP 的一部分。当一个序列参数集 RBSP 存在且在激活序列参数集 RBSP 之前没有紧随其后具有相同 `seq_parameter_set_id` 的序列参数集扩展 RBSP 时，序列参数集扩展 RBSP 及其语法元素应被视为不适用于活动序列参数集 RBSP。

对语法元素（及从这些语法元素派生的变量）在序列参数集和图像参数集中及其他语法元素之间关系的所有约束是仅适用于活动序列参数集和活动图像参数集的约束表达式。如果存在未在比特流中激活的任何序列参数集 RBSP，其语法元素应具有符合规定约束的值，若其在符合标准的比特流中被引用激活。如果存在未在比特流中激活的任何图像参数集 RBSP，其语法元素应具有符合规定约束的值，若其在符合标准的比特流中被引用激活。

在解码过程中（见第 8 条），活动图像参数集和活动序列参数集的参数值应视为生效。对于 SEI 消息的解释，应视为生效的是在同一接入单元中用于主要编码图像的 VCL NAL 单元的解码过程操作中活动的图像参数集和序列参数集的参数值，除非在 SEI 消息语义中另有规定。

### 7.4.1.2.2 接入单元的顺序及其与编码视频序列的关联

符合本建议书 | 国际标准的比特流由一个或多个编码视频序列组成。

编码视频序列由一个或多个接入单元组成。对于符合附录 A 中规定的一个或多个配置文件并使用第 2 至 9 条中规定的解码过程的编码视频序列，NAL 单元和编码图像的顺序及其与接入单元的关联在第 7.4.1.2.3 条中描述。

每个编码视频序列的第一个接入单元是 IDR 接入单元。编码视频序列中的所有后续接入单元都是非 IDR 接入单元。

比特流符合性的要求是，当在解码顺序中编码视频序列中的两个连续接入单元都包含非参考图像时，第一个接入单元中每个编码场或编码帧的场的图像顺序计数值应小于或等于第二个接入单元中每个编码场或编码帧的场的图像顺序计数值。

比特流符合性的要求是，当存在一个包含序列结束 NAL 单元的接入单元后，随后的接入单元应是一个 IDR 接入单元。

比特流符合性的要求是，当 SEI NAL 单元包含与多个接入单元相关的数据（例如，当 SEI NAL 单元的作用范围是编码视频序列时），它应包含在第一个适用的接入单元中。

比特流符合性的要求是，当接入单元中存在流结束 NAL 单元时，该接入单元应是比特流中的最后一个接入单元，并且流结束 NAL 单元应是该接入单元中的最后一个 NAL 单元。

### 7.4.1.2.3 NAL 单元和编码图像的顺序及其与接入单元的关联

本条款规定了符合附录 A 中规定的一个或多个配置文件并使用第 2 至 9 条中规定的解码过程的编码视频序列中 NAL 单元和编码图像的顺序及其与接入单元的关联。
  > **注 1** – 符合附录 G 或 H 中规定的配置文件的某些比特流可能会违反本条款中规定的 NAL 单元顺序。发生此类 NAL 单元顺序违规的条件在第 G.7.4.1.2.3 条和 H.7.4.1.2.3 条中规定。

一个接入单元由一个主要编码图像、零个或多个对应的冗余编码图像和零个或多个非 VCL NAL 单元组成。VCL NAL 单元与主要或冗余编码图像的关联在第 7.4.1.2.5 条中描述。

比特流中的第一个接入单元从比特流的第一个 NAL 单元开始。

在主要编码图像的最后一个 VCL NAL 单元之后的任何以下 NAL 单元中的第一个指定了一个新接入单元的开始：
- 接入单元分隔符 NAL 单元（如果存在），
- 序列参数集 NAL 单元（如果存在），
- 图像参数集 NAL 单元（如果存在），
- SEI NAL 单元（如果存在），
- **nal_unit_type** 范围在 14 到 18 之间（含）的 NAL 单元（如果存在），
- 主要编码图像的第一个 VCL NAL 单元（始终存在）。

第 7.4.1.2.4 条规定了检测主要编码图像的第一个 VCL NAL 单元的约束。

在一个接入单元内，编码图像和非 VCL NAL 单元的顺序应遵守以下约束：
- 当存在接入单元分隔符 NAL 单元时，它应是第一个 NAL 单元。在任何接入单元中最多只能有一个接入单元分隔符 NAL 单元。
- 当存在任何 SEI NAL 单元时，它们应在主要编码图像之前。
- 当存在包含缓冲期 SEI 消息的 SEI NAL 单元时，缓冲期 SEI 消息应是接入单元中第一个 SEI NAL 单元的第一个 SEI 消息负载。
- 主要编码图像应在对应的冗余编码图像之前。
- 当存在冗余编码图像时，它们应按 **redundant_pic_cnt** 的值按升序排列。
- 当存在序列参数集扩展 NAL 单元时，它应是具有相同 **seq_parameter_set_id** 值的序列参数集 NAL 单元之后的下一个 NAL 单元。
- 当存在一个或多个不带分区的辅助编码图像片 NAL 单元时，它们应在主要编码图像和所有冗余编码图像（如果有）之后。
- 当存在序列结束 NAL 单元时，它应在主要编码图像、所有冗余编码图像（如果有）和所有不带分区的辅助编码图像片 NAL 单元（如果有）之后。
- 当存在流结束 NAL 单元时，它应是最后一个 NAL 单元。
- **nal_unit_type** 等于 0、12 或在 20 到 31 范围内（含）的 NAL 单元不得在主要编码图像的第一个 VCL NAL 单元之前。
  > **注 2** – 序列参数集 NAL 单元或图像参数集 NAL 单元可以存在于接入单元中，但不能在接入单元内主要编码图像的最后一个 VCL NAL 单元之后，否则这种情况将指定一个新接入单元的开始。
  > **注 3** – 当一个 **nal_unit_type** 等于 7 或 8 的 NAL 单元存在于接入单元中时，它可能会或可能不会被该接入单元的编码图像引用，并且可能会被后续接入单元的编码图像引用。

不包含任何 **nal_unit_type** 等于 0、7、8 或在 12 到 18 范围内（含）、或在 20 到 31 范围内（含）的 NAL 单元的接入单元结构如图 7-1 所示。

Figure 7-1 略

### 7.4.1.2.4 检测主要编码图像的第一个 VCL NAL 单元

本条款规定了 VCL NAL 单元语法的约束，以便能够检测符合附录 A 中规定的一个或多个配置文件并使用第 2 至 9 条中规定的解码过程的编码视频序列中每个主要编码图像的第一个 VCL NAL 单元。

当前接入单元的主要编码图像的任何编码片 NAL 单元或编码片数据分区 A NAL 单元应在以下一种或多种方式上不同于前一个接入单元的主要编码图像的任何编码片 NAL 单元或编码片数据分区 A NAL 单元：

- **frame_num** 的值不同。用于测试此条件的 **frame_num** 的值是出现在片头语法中的 **frame_num** 的值，无论由于存在等于 5 的 **memory_management_control_operation**，该值是否被推断为在解码过程中使用时等于 0。
  > **注 1** – 上述声明的一个结果是，**frame_num** 等于 1 的主要编码图像不能包含等于 5 的 **memory_management_control_operation**，除非以下的某个条件在其之后的下一个主要编码图像（如果有）中得到满足。

- **pic_parameter_set_id** 的值不同。
- **field_pic_flag** 的值不同。
- **bottom_field_flag** 两者都存在且值不同。
- **nal_ref_idc** 的值不同，且其中一个 **nal_ref_idc** 的值等于 0。
- **pic_order_cnt_type** 对于两者都等于 0，并且 **pic_order_cnt_lsb** 的值不同，或 **delta_pic_order_cnt_bottom** 的值不同。
- **pic_order_cnt_type** 对于两者都等于 1，并且 **delta_pic_order_cnt[0]** 的值不同，或 **delta_pic_order_cnt[1]** 的值不同。
- **IdrPicFlag** 的值不同。
- **IdrPicFlag** 对于两者都等于 1，并且 **idr_pic_id** 的值不同。
  > **注 2** – 冗余编码图像中的一些 VCL NAL 单元或一些非 VCL NAL 单元（例如，接入单元分隔符 NAL 单元）也可以用于检测接入单元之间的边界，因此可以帮助检测新主要编码图像的开始。

### 7.4.1.2.5 VCL NAL 单元的顺序及其与编码图像的关联

本条款规定了符合附录 A 中规定的一个或多个配置文件并使用第 2 至 9 条中规定的解码过程的编码视频序列中 VCL NAL 单元的顺序及其与编码图像的关联。

每个 VCL NAL 单元都是编码图像的一部分。

编码 IDR 图像内的 VCL NAL 单元的顺序受到以下约束：

- 如果允许任意片顺序，如附录 A 所述，编码 IDR 图像的片 NAL 单元可以相互之间具有任意顺序。
- 否则（不允许任意片顺序），适用以下情况：
  - 如果 **separate_colour_plane_flag** 等于 0，则片组的编码 IDR 图像的片 NAL 单元不得与另一片组的编码 IDR 图像的片 NAL 单元交错，片组内编码 IDR 图像的片 NAL 单元的顺序应按照特定片组的每个编码 IDR 图像的片 NAL 单元的第一个宏块的宏块地址递增的顺序排列。
  - 否则（**separate_colour_plane_flag** 等于 1），特定 **colour_plane_id** 值的片组的编码 IDR 图像的片 NAL 单元不得与具有相同 **colour_plane_id** 值的另一片组的编码 IDR 图像的片 NAL 单元交错，特定 **colour_plane_id** 值的片组内编码 IDR 图像的片 NAL 单元的顺序应按照特定 **colour_plane_id** 值的片组的每个编码 IDR 图像的片 NAL 单元的第一个宏块的宏块地址递增的顺序排列。
    > **注 1** – 当 **separate_colour_plane_flag** 等于 1 时，不同 **colour_plane_id** 值的编码片的相对顺序不受约束。

编码非 IDR 图像内的 VCL NAL 单元的顺序受到以下约束：

- 如果允许任意片顺序，如附录 A 所述，编码非 IDR 图像的片 NAL 单元或编码片数据分区 A NAL 单元可以相互之间具有任意顺序。具有特定 **slice_id** 值的编码片数据分区 A NAL 单元应在任何存在的具有相同 **slice_id** 值的编码片数据分区 B NAL 单元之前。具有特定 **slice_id** 值的编码片数据分区 A NAL 单元应在任何存在的具有相同 **slice_id** 值的编码片数据分区 C NAL 单元之前。当存在具有特定 **slice_id** 值的编码片数据分区 B NAL 单元时，它应在任何存在的具有相同 **slice_id** 值的编码片数据分区 C NAL 单元之前。
- 否则（不允许任意片顺序），适用以下情况：
  - 如果 **separate_colour_plane_flag** 等于 0，则片组的编码非 IDR 图像的片 NAL 单元或编码片数据分区 NAL 单元不得与另一片组的编码非 IDR 图像的片 NAL 单元或编码片数据分区 NAL 单元交错，片组内编码非 IDR 图像的片 NAL 单元或编码片数据分区 A NAL 单元的顺序应按照特定片组的每个编码非 IDR 图像的片 NAL 单元或编码片数据分区 A NAL 单元的第一个宏块的宏块地址递增的顺序排列。具有特定 **slice_id** 值的编码片数据分区 A NAL 单元应紧接在任何存在的具有相同 **slice_id** 值的编码片数据分区 B NAL 单元之前。具有特定 **slice_id** 值的编码片数据分区 A NAL 单元应紧接在任何存在的具有相同 **slice_id** 值的编码片数据分区 C NAL 单元之前，当不存在具有相同 **slice_id** 值的编码片数据分区 B NAL 单元时。当存在具有特定 **slice_id** 值的编码片数据分区 B NAL 单元时，它应紧接在任何存在的具有相同 **slice_id** 值的编码片数据分区 C NAL 单元之前。
  - 否则（**separate_colour_plane_flag** 等于 1），特定 **colour_plane_id** 值的片组的编码非 IDR 图像的片 NAL 单元或编码片数据分区 NAL 单元不得与具有相同 **colour_plane_id** 值的另一片组的编码非 IDR 图像的片 NAL 单元或编码片数据分区 NAL 单元交错，特定 **colour_plane_id** 值的片组内编码非 IDR 图像的片 NAL 单元或编码片数据分区 A NAL 单元的顺序应按照特定 **colour_plane_id** 值的片组的每个编码非 IDR 图像的片 NAL 单元或编码片数据分区 A NAL 单元的第一个宏块的宏块地址递增的顺序排列。与特定 **slice_id** 和 **colour_plane_id** 值相关的编码片数据分区 A NAL 单元应紧接在任何存在的具有相同 **slice_id** 和 **colour_plane_id** 值的编码片数据分区 B NAL 单元之前。与特定 **slice_id** 和 **colour_plane_id** 值相关的编码片数据分区 A NAL 单元应紧接在任何存在的具有相同 **slice_id** 和 **colour_plane_id** 值的编码片数据分区 C NAL 单元之前，当不存在具有相同 **slice_id** 和 **colour_plane_id** 值的编码片数据分区 B NAL 单元时。当存在具有特定 **slice_id** 和 **colour_plane_id** 值的编码片数据分区 B NAL 单元时，它应紧接在任何存在的具有相同 **slice_id** 和 **colour_plane_id** 值的编码片数据分区 C NAL 单元之前。
    > **注 2** – 当 **separate_colour_plane_flag** 等于 1 时，不同 **colour_plane_id** 值的编码片的相对顺序不受约束。

具有 **nal_unit_type** 等于 12 的 NAL 单元可以存在于接入单元中，但不得在接入单元中主要编码图像的第一个 VCL NAL 单元之前。

具有 **nal_unit_type** 等于 0 或在 24 到 31 范围内（含）的未指定的 NAL 单元可以存在于接入单元中，但不得在接入单元中主要编码图像的第一个 VCL NAL 单元之前。

具有 **nal_unit_type** 在 20 到 23 范围内（含）的 NAL 单元不得在接入单元中主要编码图像的第一个 VCL NAL 单元之前。

[TODO] 7.4.2
[TODO] 7.4.3
[TODO] 7.4.4
[TODO] 7.4.5

## 8 解码过程

该过程的输出是当前图像的解码样本（有时通过变量 CurrPic 引用）。

根据 `chroma_format_idc` 的值，当前图像的样本数组数量如下：

- 如果 `chroma_format_idc` 等于 0，当前图像由一个样本数组 SL 组成。
- 否则（`chroma_format_idc` 不等于 0），当前图像由三个样本数组 SL、SCb 和 SCr 组成。

本条款描述了解码过程，给出了第 7 条中的语法元素和大写变量。

解码过程的规定使得所有解码器应产生数值上相同的结果。任何产生与此处描述的过程相同结果的解码过程都符合本建议书 | 国际标准的解码过程要求。

本条款中提到的每个图像都是一个完整的主要编码图像或主要编码图像的一部分。本条款中提到的每个片都是主要编码图像的片。本条款中提到的每个片数据分区都是主要编码图像的片数据分区。

根据 `separate_colour_plane_flag` 的值，解码过程的结构如下：

- 如果 `separate_colour_plane_flag` 等于 0，解码过程仅调用一次，当前图像为输出。
- 否则（`separate_colour_plane_flag` 等于 1），解码过程调用三次。解码过程的输入是具有相同 `colour_plane_id` 值的主要编码图像的所有 NAL 单元。具有特定 `colour_plane_id` 值的 NAL 单元的解码过程被规定为仅在比特流中存在具有该特定 `colour_plane_id` 值的单色格式的编码视频序列。每个解码过程的输出分配给当前图像的三个样本数组，其中 `colour_plane_id` 等于 0 的 NAL 单元分配给 SL，`colour_plane_id` 等于 1 的 NAL 单元分配给 SCb，`colour_plane_id` 等于 2 的 NAL 单元分配给 SCr。
  > **注** – 当 `separate_colour_plane_flag` 等于 1 且 `chroma_format_idc` 等于 3 时，变量 `ChromaArrayType` 派生为 0。在解码过程中，该变量的值的评估结果与 `chroma_format_idc` 等于 0 的单色图像相同。

解码过程概述如下：

1. NAL 单元的解码在第 8.1 条中规定。

2. 第 8.2 条中的过程规定了使用片层及以上的语法元素的解码过程：
   - 第 8.2.1 条中派生了与图像顺序计数相关的变量和函数（仅需对图像的一个片调用）。
   - 第 8.2.2 条中派生了与宏块到片组映射相关的变量和函数（仅需对图像的一个片调用）。
   - 第 8.2.3 条描述了在使用片数据分区时组合各种片数据分区的方法。
   - 当当前图像的 `frame_num` 不等于 `PrevRefFrameNum` 并且不等于 `(PrevRefFrameNum + 1) % MaxFrameNum` 时，在解码当前图像的任何片之前，根据第 8.2.5.2 条执行 `frame_num` 间隙的解码过程。
   - 在每个 P、SP 或 B 片的解码过程开始时，调用第 8.2.4 条中规定的参考图像列表构建过程，派生参考图像列表 0（RefPicList0），并在解码 B 片时派生参考图像列表 1（RefPicList1）。
   - 当当前图像是参考图像时，在解码当前图像的所有片之后，第 8.2.5 条中的解码参考图像标记过程规定了在稍后解码图像的帧间预测中如何使用当前图像。

3. 第 8.3、8.4、8.5、8.6 和 8.7 条中的过程规定了使用宏块层及以上的语法元素的解码过程。
   - 第 8.3 条规定了 I 和 SI 宏块的帧内预测过程（除 I_PCM 宏块外），其输出为帧内预测样本。对于 I_PCM 宏块，第 8.3 条直接规定了图像构建过程。输出是在去块滤波过程之前构建的样本。
   - 第 8.4 条规定了 P 和 B 宏块的帧间预测过程，其输出为帧间预测样本。
   - 第 8.5 条规定了变换系数解码过程和去块滤波过程之前的图像构建过程。该过程派生 I 和 B 宏块以及 P 片中 P 宏块的样本。输出是在去块滤波过程之前构建的样本。
   - 第 8.6 条规定了 SP 片中 P 宏块或 SI 宏块的解码过程。该过程派生 SP 片中的 P 宏块和 SI 宏块的样本。输出是在去块滤波过程之前构建的样本。
   - 第 8.7 条规定了去块滤波过程，其中紧邻块和宏块边缘的去块滤波过程将去块滤波过程之前构建的样本处理为解码样本。

### 8.1 NAL 单元解码过程

该过程的输入是 NAL 单元。

该过程的输出是封装在 NAL 单元中的 RBSP 语法结构。

每个 NAL 单元的解码过程从 NAL 单元中提取 RBSP 语法结构，然后按照以下步骤执行针对 NAL 单元中 RBSP 语法结构的解码过程。

第 8.2 条描述了 **nal_unit_type** 等于 1 到 5 的 NAL 单元的解码过程。

第 8.3 条描述了 **nal_unit_type** 等于 1、2 和 5 的 NAL 单元中编码的宏块或部分宏块的解码过程。

第 8.4 条描述了 **nal_unit_type** 等于 1 和 2 的 NAL 单元中编码的宏块或部分宏块的解码过程。

第 8.5 条描述了 **nal_unit_type** 等于 1 和 3 到 5 的 NAL 单元中编码的宏块或部分宏块的解码过程。

第 8.6 条描述了 **nal_unit_type** 等于 1 和 3 到 5 的 NAL 单元中编码的宏块或部分宏块的解码过程。

第 8.7 条描述了 **nal_unit_type** 等于 1 到 5 的 NAL 单元中编码的宏块或部分宏块的解码过程。

**nal_unit_type** 等于 7 和 8 的 NAL 单元分别包含序列参数集和图像参数集。图像参数集在每个图像的片头中通过引用图像参数集在其他 NAL 单元的解码过程中使用。序列参数集在每个序列的图像参数集中通过引用序列参数集在其他 NAL 单元的解码过程中使用。

对于 **nal_unit_type** 等于 6、9、10、11 和 12 的 NAL 单元，本标准未规定规范的解码过程。

### 8.2 片解码过程

#### 8.2.1 图像顺序计数解码过程

该过程的输出为 TopFieldOrderCnt（如适用）和 BottomFieldOrderCnt（如适用）。

图像顺序计数用于在解码 B 片时确定参考图像的初始顺序（见第 8.2.4.2.3 条和第 8.2.4.2.4 条），确定共定位图像（见第 8.4.1.2.1 条）以在时间或空间直接模式下派生运动参数，表示帧间或场间的图像顺序差异以在时间直接模式下派生运动矢量（见第 8.4.1.2.3 条），用于 B 片中的隐式模式加权预测（见第 8.4.2.3.2 条），以及用于解码器一致性检查（见第 C.4 条）。

图像顺序计数信息按如下方式为每帧、每场（无论是从编码场解码还是作为解码帧的一部分解码）或互补场对派生：

- 每个编码帧与两个图像顺序计数相关，分别称为其顶场和底场的 TopFieldOrderCnt 和 BottomFieldOrderCnt。
- 每个编码场与一个图像顺序计数相关，称为编码顶场的 TopFieldOrderCnt 和底场的 BottomFieldOrderCnt。
- 每个互补场对与两个图像顺序计数相关，分别为其编码顶场的 TopFieldOrderCnt 和编码底场的 BottomFieldOrderCnt。

TopFieldOrderCnt 和 BottomFieldOrderCnt 表示相应的顶场或底场相对于前一个 IDR 图像或包含 memory_management_control_operation 等于 5 的前一个参考图像的第一个输出场的图像顺序。

TopFieldOrderCnt 和 BottomFieldOrderCnt 通过分别调用第 8.2.1.1、8.2.1.2 和 8.2.1.3 条中的图像顺序计数类型 0、1 和 2 的解码过程派生。当当前图像包含 memory_management_control_operation 等于 5 时，在解码当前图像之后，将 tempPicOrderCnt 设置为 PicOrderCnt(CurrPic)，将当前图像的 TopFieldOrderCnt（如适用）设置为 TopFieldOrderCnt - tempPicOrderCnt，将当前图像的 BottomFieldOrderCnt（如适用）设置为 BottomFieldOrderCnt - tempPicOrderCnt。
  > **注 1** – 当包括 memory_management_control_operation 等于 5 的图像 currPic 的解码过程引用图像 currPic 的 TopFieldOrderCnt（如适用）或 BottomFieldOrderCnt（如适用）值时（包括以 currPic 作为参数引用 PicOrderCnt() 函数和以 currPic 作为一个参数引用 DiffPicOrderCnt() 函数的引用），使用按第 8.2.1.1、8.2.1.2 和 8.2.1.3 条中规定的方式派生的图像 currPic 的 TopFieldOrderCnt（如适用）和 BottomFieldOrderCnt（如适用）值。当图像的解码过程引用包括 memory_management_control_operation 等于 5 的前一个图像 prevMmco5Pic 的 TopFieldOrderCnt（如适用）或 BottomFieldOrderCnt（如适用）值时（包括通过函数 PicOrderCnt() 或 DiffPicOrderCnt() 引用），用于图像 prevMmco5Pic 的 TopFieldOrderCnt（如适用）和 BottomFieldOrderCnt（如适用）值是上述段落中规定的修改后的值（导致 TopFieldOrderCnt 和/或 BottomFieldOrderCnt 等于 0）。

比特流不得包含导致编码 IDR 帧的 Min(TopFieldOrderCnt, BottomFieldOrderCnt) 不等于 0 的数据，编码 IDR 顶场的 TopFieldOrderCnt 不等于 0 的数据，或编码 IDR 底场的 BottomFieldOrderCnt 不等于 0 的数据。因此，编码 IDR 帧的场的 TopFieldOrderCnt 和 BottomFieldOrderCnt 中至少有一个应等于 0。

当当前图像不是 IDR 图像时，适用以下规定：

1) 考虑列表变量 listD，其中包含的元素为与以下图像列表相关联的 TopFieldOrderCnt 和 BottomFieldOrderCnt 值：

  a. 列表中的第一个图像是以下任意类型的前一个图像：
    - IDR 图像，
    - 包含 memory_management_control_operation 等于 5 的图像。

  b. 以下附加图像：
    - 如果 pic_order_cnt_type 等于 0，则所有在解码顺序中紧随列表中第一个图像之后且不是通过第 8.2.5.2 条中规定的 frame_num 间隙解码过程推断的“非存在”帧的其他图像，这些图像在解码顺序中在当前图像之前或是当前图像。当 pic_order_cnt_type 等于 0 且当前图像不是通过第 8.2.5.2 条中规定的 frame_num 间隙解码过程推断的“非存在”帧时，当前图像在调用解码参考图像标记过程之前包含在 listD 中。
    - 否则（pic_order_cnt_type 不等于 0），在解码顺序中紧随列表中第一个图像之后的所有其他图像，这些图像在解码顺序中在当前图像之前或是当前图像。当 pic_order_cnt_type 不等于 0 时，当前图像在调用解码参考图像标记过程之前包含在 listD 中。

2) 考虑列表变量 listO，其中包含按升序排列的 listD 的元素。listO 不得包含以下任何内容：
  - 列表中不在连续位置的帧或互补场对的 TopFieldOrderCnt 和 BottomFieldOrderCnt，
  - 具有相同值的两个 TopFieldOrderCnt，
  - 具有相同值的两个 BottomFieldOrderCnt，
  - 具有等于 TopFieldOrderCnt 的 BottomFieldOrderCnt，除非 BottomFieldOrderCnt 和 TopFieldOrderCnt 属于同一编码帧或互补场对。

比特流不得包含导致在第 8.2.1.1 至第 8.2.1.3 条中规定的解码过程中使用的 TopFieldOrderCnt、BottomFieldOrderCnt、PicOrderCntMsb 或 FrameNumOffset 的值超出 -2^31 至 2^31 - 1 范围的数据。

函数 PicOrderCnt(picX) 规定如下：
```c
if (picX is a frame or a complementary field pair)
  PicOrderCnt(picX) = Min(TopFieldOrderCnt, BottomFieldOrderCnt) of the frame or complementary field pair picX
else if (picX is a top field)
  PicOrderCnt(picX) = TopFieldOrderCnt of field picX
else if (picX is a bottom field)
  PicOrderCnt(picX) = BottomFieldOrderCnt of field picX
```

然后，函数 DiffPicOrderCnt(picA, picB) 规定如下：
```c
DiffPicOrderCnt(picA, picB) = PicOrderCnt(picA) − PicOrderCnt(picB)
```

比特流不得包含导致在解码过程中使用的 DiffPicOrderCnt(picA, picB) 值超出 -2^15 至 2^15 - 1 范围的数据。
  > **注 2** – 设 X 为当前图像，Y 和 Z 为同一序列中的两个其他图像，当 DiffPicOrderCnt(X, Y) 和 DiffPicOrderCnt(X, Z) 都为正或都为负时，认为 Y 和 Z 与 X 的输出顺序方向相同。
  > **注 3** – 许多编码器分配与前一个 IDR 图像或包含 memory_management_control_operation 等于 5 的前一个参考图像的第一个输出场的采样时间成比例的 TopFieldOrderCnt 和 BottomFieldOrderCnt。

当当前图像包含 memory_management_control_operation 等于 5 时，PicOrderCnt(CurrPic) 应大于 listD 中任何其他图像的 PicOrderCnt。

### 8.2.1.1 图像顺序计数类型 0 的解码过程

当 `pic_order_cnt_type` 等于 0 时调用此过程。

该过程的输入是解码顺序中的前一个参考图像的 `PicOrderCntMsb`，如本条款所述。

该过程的输出是 `TopFieldOrderCnt` 或 `BottomFieldOrderCnt`（或两者）。

变量 `prevPicOrderCntMsb` 和 `prevPicOrderCntLsb` 的派生如下：

- 如果当前图像是 IDR 图像，则 `prevPicOrderCntMsb` 设置为 0，`prevPicOrderCntLsb` 设置为 0。
- 否则（当前图像不是 IDR 图像），适用以下情况：
  - 如果解码顺序中的前一个参考图像包含 `memory_management_control_operation` 等于 5，则适用以下情况：
    - 如果解码顺序中的前一个参考图像不是底场，则 `prevPicOrderCntMsb` 设置为 0，`prevPicOrderCntLsb` 设置为解码顺序中的前一个参考图像的 `TopFieldOrderCnt` 的值。
    - 否则（解码顺序中的前一个参考图像是底场），`prevPicOrderCntMsb` 设置为 0，`prevPicOrderCntLsb` 设置为 0。
  - 否则（解码顺序中的前一个参考图像不包含 `memory_management_control_operation` 等于 5），`prevPicOrderCntMsb` 设置为解码顺序中的前一个参考图像的 `PicOrderCntMsb`，`prevPicOrderCntLsb` 设置为解码顺序中的前一个参考图像的 `pic_order_cnt_lsb` 的值。

当前图像的 `PicOrderCntMsb` 按以下伪代码派生：

```c
if ((pic_order_cnt_lsb < prevPicOrderCntLsb) && ((prevPicOrderCntLsb - pic_order_cnt_lsb) >= (MaxPicOrderCntLsb / 2)))
  PicOrderCntMsb = prevPicOrderCntMsb + MaxPicOrderCntLsb
else if ((pic_order_cnt_lsb > prevPicOrderCntLsb) && ((pic_order_cnt_lsb - prevPicOrderCntLsb) > (MaxPicOrderCntLsb / 2)))
  PicOrderCntMsb = prevPicOrderCntMsb - MaxPicOrderCntLsb
else
  PicOrderCntMsb = prevPicOrderCntMsb
```

当当前图像不是底场时，`TopFieldOrderCnt` 按如下派生：

```c
TopFieldOrderCnt = PicOrderCntMsb + pic_order_cnt_lsb
```

当当前图像不是顶场时，`BottomFieldOrderCnt` 按以下伪代码派生：

```c
if (!field_pic_flag)
  BottomFieldOrderCnt = TopFieldOrderCnt + delta_pic_order_cnt_bottom
else
  BottomFieldOrderCnt = PicOrderCntMsb + pic_order_cnt_lsb
```

### 8.2.1.2 图像顺序计数类型 1 的解码过程

当 `pic_order_cnt_type` 等于 1 时调用此过程。

该过程的输入是解码顺序中的前一个图像的 `FrameNumOffset`，如本条款所述。

该过程的输出是 `TopFieldOrderCnt` 或 `BottomFieldOrderCnt`（或两者）。

`TopFieldOrderCnt` 和 `BottomFieldOrderCnt` 的值按本条款规定派生。令 `prevFrameNum` 等于解码顺序中的前一个图像的 `frame_num`。

当当前图像不是 IDR 图像时，变量 `prevFrameNumOffset` 按如下派生：

- 如果解码顺序中的前一个图像包含 `memory_management_control_operation` 等于 5，`prevFrameNumOffset` 设置为 0。
- 否则（解码顺序中的前一个图像不包含 `memory_management_control_operation` 等于 5），`prevFrameNumOffset` 设置为解码顺序中的前一个图像的 `FrameNumOffset` 的值。
  > **注** – 当 `gaps_in_frame_num_value_allowed_flag` 等于 1 时，解码顺序中的前一个图像可能是通过第 8.2.5.2 条规定的 `frame_num` 间隙解码过程推断的“非存在”帧。

变量 `FrameNumOffset` 按以下伪代码派生：
```c
if (IdrPicFlag == 1)
  FrameNumOffset = 0
else if (prevFrameNum > frame_num)
  FrameNumOffset = prevFrameNumOffset + MaxFrameNum
else
  FrameNumOffset = prevFrameNumOffset
```

变量 `absFrameNum` 按以下伪代码派生：
```c
if (num_ref_frames_in_pic_order_cnt_cycle != 0)
  absFrameNum = FrameNumOffset + frame_num
else
  absFrameNum = 0
if (nal_ref_idc == 0 && absFrameNum > 0)
  absFrameNum = absFrameNum − 1
```

当 `absFrameNum > 0` 时，变量 `picOrderCntCycleCnt` 和 `frameNumInPicOrderCntCycle` 按如下派生：
```c
picOrderCntCycleCnt = (absFrameNum − 1) / num_ref_frames_in_pic_order_cnt_cycle
frameNumInPicOrderCntCycle = (absFrameNum − 1) % num_ref_frames_in_pic_order_cnt_cycle
```

变量 `expectedPicOrderCnt` 按以下伪代码派生：
```c
if (absFrameNum > 0) {
  expectedPicOrderCnt = picOrderCntCycleCnt * ExpectedDeltaPerPicOrderCntCycle
  for (i = 0; i <= frameNumInPicOrderCntCycle; i++)
    expectedPicOrderCnt = expectedPicOrderCnt + offset_for_ref_frame[i]
} else
  expectedPicOrderCnt = 0
if (nal_ref_idc == 0)
  expectedPicOrderCnt = expectedPicOrderCnt + offset_for_non_ref_pic
```

`TopFieldOrderCnt` 或 `BottomFieldOrderCnt` 变量按以下伪代码派生：
```c
if (!field_pic_flag) {
  TopFieldOrderCnt = expectedPicOrderCnt + delta_pic_order_cnt[0]
  BottomFieldOrderCnt = TopFieldOrderCnt + offset_for_top_to_bottom_field + delta_pic_order_cnt[1]
} else if (!bottom_field_flag)
  TopFieldOrderCnt = expectedPicOrderCnt + delta_pic_order_cnt[0]
else
  BottomFieldOrderCnt = expectedPicOrderCnt + offset_for_top_to_bottom_field + delta_pic_order_cnt[0]
```

### 8.2.1.3 图像顺序计数类型 2 的解码过程

当 `pic_order_cnt_type` 等于 2 时调用此过程。

该过程的输出是 `TopFieldOrderCnt` 或 `BottomFieldOrderCnt`（或两者）。

令 `prevFrameNum` 等于解码顺序中的前一个图像的 `frame_num`。

当当前图像不是 IDR 图像时，变量 `prevFrameNumOffset` 按如下派生：

- 如果解码顺序中的前一个图像包含 `memory_management_control_operation` 等于 5，则 `prevFrameNumOffset` 设置为 0。
- 否则（解码顺序中的前一个图像不包含 `memory_management_control_operation` 等于 5），`prevFrameNumOffset` 设置为解码顺序中的前一个图像的 `FrameNumOffset` 值。
  > **注 1** – 当 `gaps_in_frame_num_value_allowed_flag` 等于 1 时，解码顺序中的前一个图像可能是通过第 8.2.5.2 条规定的 `frame_num` 间隙解码过程推断的“非存在”帧。

变量 `FrameNumOffset` 按以下伪代码派生：

```c
if (IdrPicFlag == 1)
  FrameNumOffset = 0
else if (prevFrameNum > frame_num)
  FrameNumOffset = prevFrameNumOffset + MaxFrameNum
else
  FrameNumOffset = prevFrameNumOffset
```

变量 `tempPicOrderCnt` 按以下伪代码派生：

```c
if (IdrPicFlag == 1)
  tempPicOrderCnt = 0
else if (nal_ref_idc == 0)
  tempPicOrderCnt = 2 * (FrameNumOffset + frame_num) - 1
else
  tempPicOrderCnt = 2 * (FrameNumOffset + frame_num)
```

`TopFieldOrderCnt` 或 `BottomFieldOrderCnt` 变量按以下伪代码派生：

```c
if (!field_pic_flag) {
  TopFieldOrderCnt = tempPicOrderCnt
  BottomFieldOrderCnt = tempPicOrderCnt
} else if (bottom_field_flag)
  BottomFieldOrderCnt = tempPicOrderCnt
else
  TopFieldOrderCnt = tempPicOrderCnt
```
  > **注 2** – 图像顺序计数类型 2 不能用于包含连续非参考图像的编码视频序列，因为这将导致这些图像中的多个图像具有相同的 `TopFieldOrderCnt` 值或多个图像具有相同的 `BottomFieldOrderCnt` 值。
  > **注 3** – 图像顺序计数类型 2 的输出顺序与解码顺序相同。

### 8.2.2 宏块到片组映射的解码过程

该过程的输入是活动图像参数集和要解码的片的片头。

该过程的输出是宏块到片组映射 `MbToSliceGroupMap`。

每个片开始时调用此过程。
  > **注** – 对于图像的所有片，该过程的输出是相同的。

当 `num_slice_groups_minus1` 等于 1 且 `slice_group_map_type` 等于 3、4 或 5 时，片组 0 和 1 的大小和形状由 `slice_group_change_direction_flag` 确定，如表 8-1 所示，并在第 8.2.2.4 至 8.2.2.6 条中规定。

表 8-1 – 精细片组映射类型

在这种情况下，按照指定的增长顺序为片组 0 分配 `MapUnitsInSliceGroup0` 片组映射单元，剩余的 `PicSizeInMapUnits - MapUnitsInSliceGroup0` 图像的片组映射单元分配给片组 1。

当 `num_slice_groups_minus1` 等于 1 且 `slice_group_map_type` 等于 4 或 5 时，变量 `sizeOfUpperLeftGroup` 定义如下：

```c
sizeOfUpperLeftGroup = (slice_group_change_direction_flag ? (PicSizeInMapUnits - MapUnitsInSliceGroup0) : MapUnitsInSliceGroup0)
```

`mapUnitToSliceGroupMap` 数组按如下方式派生：

- 如果 `num_slice_groups_minus1` 等于 0，片组映射单元映射为所有范围从 0 到 `PicSizeInMapUnits - 1`（含）：

```c
mapUnitToSliceGroupMap[i] = 0
```

- 否则（`num_slice_groups_minus1` 不等于 0），`mapUnitToSliceGroupMap` 派生如下：
  - 如果 `slice_group_map_type` 等于 0，第 8.2.2.1 条中规定的 `mapUnitToSliceGroupMap` 的派生适用。
  - 否则，如果 `slice_group_map_type` 等于 1，第 8.2.2.2 条中规定的 `mapUnitToSliceGroupMap` 的派生适用。
  - 否则，如果 `slice_group_map_type` 等于 2，第 8.2.2.3 条中规定的 `mapUnitToSliceGroupMap` 的派生适用。
  - 否则，如果 `slice_group_map_type` 等于 3，第 8.2.2.4 条中规定的 `mapUnitToSliceGroupMap` 的派生适用。
  - 否则，如果 `slice_group_map_type` 等于 4，第 8.2.2.5 条中规定的 `mapUnitToSliceGroupMap` 的派生适用。
  - 否则，如果 `slice_group_map_type` 等于 5，第 8.2.2.6 条中规定的 `mapUnitToSliceGroupMap` 的派生适用。
  - 否则（`slice_group_map_type` 等于 6），第 8.2.2.7 条中规定的 `mapUnitToSliceGroupMap` 的派生适用。

在派生 `mapUnitToSliceGroupMap` 之后，调用第 8.2.2.8 条中规定的过程将片组映射单元映射 `mapUnitToSliceGroupMap` 转换为宏块到片组映射 `MbToSliceGroupMap`。在按第 8.2.2.8 条派生宏块到片组映射之后，函数 `NextMbAddress( n )` 定义为变量 `nextMbAddress` 的值，按以下伪代码派生：

```c
i = n + 1
while (i < PicSizeInMbs && MbToSliceGroupMap[i] != MbToSliceGroupMap[n])
   i++;
nextMbAddress = i
```

### 8.2.2.1 交错片组映射类型的规范

当 `slice_group_map_type` 等于 0 时，适用本条款中的规范。

片组映射单元映射按以下伪代码生成：

```c
i = 0
do {
  for (iGroup = 0; iGroup <= num_slice_groups_minus1 && i < PicSizeInMapUnits; i += run_length_minus1[iGroup++] + 1)
    for (j = 0; j <= run_length_minus1[iGroup] && i + j < PicSizeInMapUnits; j++)
      mapUnitToSliceGroupMap[i + j] = iGroup
} while (i < PicSizeInMapUnits)
```

### 8.2.2.2 分散片组映射类型的规范

当 `slice_group_map_type` 等于 1 时，适用本条款中的规范。

片组映射单元映射按以下伪代码生成：

```c
for (i = 0; i < PicSizeInMapUnits; i++)
  mapUnitToSliceGroupMap[i] = ((i % PicWidthInMbs) + (((i / PicWidthInMbs) * (num_slice_groups_minus1 + 1)) / 2)) % (num_slice_groups_minus1 + 1)
```

### 8.2.2.3 前景与剩余片组映射类型的规范

当 `slice_group_map_type` 等于 2 时，适用本条款中的规范。

片组映射单元映射按以下伪代码生成：

```c
for (i = 0; i < PicSizeInMapUnits; i++)
  mapUnitToSliceGroupMap[i] = num_slice_groups_minus1
for (iGroup = num_slice_groups_minus1 - 1; iGroup >= 0; iGroup--) {
  yTopLeft = top_left[iGroup] / PicWidthInMbs
  xTopLeft = top_left[iGroup] % PicWidthInMbs
  yBottomRight = bottom_right[iGroup] / PicWidthInMbs
  xBottomRight = bottom_right[iGroup] % PicWidthInMbs
  for (y = yTopLeft; y <= yBottomRight; y++)
    for (x = xTopLeft; x <= xBottomRight; x++)
      mapUnitToSliceGroupMap[y * PicWidthInMbs + x] = iGroup
}
```

  >**注** – 矩形可能会重叠。片组 0 包含在 `top_left[0]` 和 `bottom_right[0]` 指定的矩形内的宏块。片组 ID 大于 0 且小于 `num_slice_groups_minus1` 的片组包含在该片组指定的矩形内但不在任何片组 ID 较小的片组指定的矩形内的宏块。片组 ID 等于 `num_slice_groups_minus1` 的片组包含不在其他片组内的宏块。

### 8.2.2.4 箱式片组映射类型的规范

当 `slice_group_map_type` 等于 3 时，适用本条款中的规范。

片组映射单元映射按以下伪代码生成：

```c
for (i = 0; i < PicSizeInMapUnits; i++)
  mapUnitToSliceGroupMap[i] = 1

x = (PicWidthInMbs - slice_group_change_direction_flag) / 2
y = (PicHeightInMapUnits - slice_group_change_direction_flag) / 2
(leftBound, topBound) = (x, y)
(rightBound, bottomBound) = (x, y)
(xDir, yDir) = (slice_group_change_direction_flag - 1, slice_group_change_direction_flag)

for (k = 0; k < MapUnitsInSliceGroup0; k += mapUnitVacant) {
  mapUnitVacant = (mapUnitToSliceGroupMap[y * PicWidthInMbs + x] == 1 )
  if (mapUnitVacant)
    mapUnitToSliceGroupMap[y * PicWidthInMbs + x] = 0

  if (xDir == -1 && x == leftBound) {
    leftBound = Max(leftBound - 1, 0)
    x = leftBound
    (xDir, yDir) = (0, 2 * slice_group_change_direction_flag - 1)
  } else if (xDir == 1 && x == rightBound) {
    rightBound = Min(rightBound + 1, PicWidthInMbs - 1)
    x = rightBound
    (xDir, yDir) = (0, 1 - 2 * slice_group_change_direction_flag)
  } else if (yDir == -1 && y == topBound) {
    topBound = Max(topBound - 1, 0)
    y = topBound
    (xDir, yDir) = (1 - 2 * slice_group_change_direction_flag, 0)
  } else if (yDir == 1 && y == bottomBound) {
    bottomBound = Min(bottomBound + 1, PicHeightInMapUnits - 1)
    y = bottomBound
    (xDir, yDir) = (2 * slice_group_change_direction_flag - 1, 0)
  } else {
    (x, y) = (x + xDir, y + yDir)
  }
}
```

[TODO] 8.2.2.5