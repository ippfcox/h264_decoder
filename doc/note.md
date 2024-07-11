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

**forbidden_zero_bit** 必须等于 0。

**nal_ref_idc** 不等于 0 表示 NAL 单元的内容包含序列参数集、序列参数集扩展、子集序列参数集、图像参数集、参考图像的一片、参考图像的一片数据分区或在参考图像的一片之前的前缀 NAL 单元。

对于符合附录 A 中规定的一个或多个配置文件的编码视频序列，这些序列使用第 2 至第 9 条中规定的解码过程进行解码，当 NAL 单元包含一片或一片数据分区且 **nal_ref_idc** 等于 0 时，表示该片或片数据分区是非参考图像的一部分。

**nal_ref_idc** 对于序列参数集、序列参数集扩展、子集序列参数集或图像参数集 NAL 单元，不应等于 0。当一个 NAL 单元的 **nal_unit_type** 在 1 到 4（含）范围内时，其 **nal_ref_idc** 等于 0，那么对于该图像的所有 **nal_unit_type** 在 1 到 4（含）范围内的 NAL 单元，其 **nal_ref_idc** 都应等于 0。

**nal_ref_idc** 对于 **nal_unit_type** 等于 5 的 NAL 单元，不应等于 0。

**nal_ref_idc** 对于 **nal_unit_type** 等于 6、9、10、11 或 12 的所有 NAL 单元，应等于 0。

**nal_unit_type** 指定 NAL 单元中包含的 RBSP 数据结构的类型，如表 7-1 所示。

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
$$
\text{IdrPicFlag} = ((\text{nal\_unit\_type} == 5) ? 1 : 0) \tag{7-1}
$$
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
$$
\text{DepthFlag} = (\text{nal\_unit\_type} != 21) ? 0 : (\text{avc\_3d\_extension\_flag} ? \text{depth\_flag} : 1) \tag{7-2}
$$
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

### 7.4.2 原始字节序列有效负载和 RBSP 尾随位语义

#### 7.4.2.1 序列参数集 RBSP 语义

##### 7.4.2.1.1 序列参数集数据语义

`profile_idc` 和 `level_idc` 表示编码视频序列符合的配置文件和级别。
`constraint_set0_flag` 等于 1 表示编码视频序列遵守第 A.2.1 条规定的所有约束。
`constraint_set0_flag` 等于 0 表示编码视频序列可能遵守或不遵守第 A.2.1 条规定的所有约束。
`constraint_set1_flag` 等于 1 表示编码视频序列遵守第 A.2.2 条规定的所有约束。
`constraint_set1_flag` 等于 0 表示编码视频序列可能遵守或不遵守第 A.2.2 条规定的所有约束。
`constraint_set2_flag` 等于 1 表示编码视频序列遵守第 A.2.3 条规定的所有约束。
`constraint_set2_flag` 等于 0 表示编码视频序列可能遵守或不遵守第 A.2.3 条规定的所有约束。
  > **注 1** – 当一个或多个 `constraint_set0_flag`、`constraint_set1_flag` 或 `constraint_set2_flag` 等于 1 时，编码视频序列必须遵守第 A.2 条的所有指示的子条款的约束。当 `profile_idc` 等于 44、100、110、122 或 244 时，`constraint_set0_flag`、`constraint_set1_flag` 和 `constraint_set2_flag` 的值必须都等于 0。

`constraint_set3_flag` 的规定如下：

- 如果 `profile_idc` 等于 66、77 或 88 且 `level_idc` 等于 11，`constraint_set3_flag` 等于 1 表示编码视频序列遵守附录 A 中第 1b 级的所有约束，`constraint_set3_flag` 等于 0 表示编码视频序列遵守附录 A 中第 1.1 级的所有约束。
- 否则，如果 `profile_idc` 等于 100 或 110，`constraint_set3_flag` 等于 1 表示编码视频序列遵守附录 A 中 High 10 Intra 配置文件的所有约束，`constraint_set3_flag` 等于 0 表示编码视频序列可能遵守或不遵守这些相应的约束。
- 否则，如果 `profile_idc` 等于 122，`constraint_set3_flag` 等于 1 表示编码视频序列遵守附录 A 中 High 4:2:2 Intra 配置文件的所有约束，`constraint_set3_flag` 等于 0 表示编码视频序列可能遵守或不遵守这些相应的约束。
- 否则，如果 `profile_idc` 等于 44，`constraint_set3_flag` 应等于 1。当 `profile_idc` 等于 44 时，`constraint_set3_flag` 的值为 0 是禁止的。
- 否则，如果 `profile_idc` 等于 244，`constraint_set3_flag` 等于 1 表示编码视频序列遵守附录 A 中 High 4:4:4 Intra 配置文件的所有约束，`constraint_set3_flag` 等于 0 表示编码视频序列可能遵守或不遵守这些相应的约束。
- 否则（`profile_idc` 等于 66、77 或 88 且 `level_idc` 不等于 11，或 `profile_idc` 不等于 66、77、88、100、110、122、244 或 44），`constraint_set3_flag` 的值为 1 保留给 ITU-T | ISO/IEC 将来使用。在符合本建议书 | 国际标准的比特流中，`profile_idc` 等于 66、77 或 88 且 `level_idc` 不等于 11 以及 `profile_idc` 不等于 66、77、88、100、110、122、244 或 44 的编码视频序列的 `constraint_set3_flag` 应等于 0。当 `profile_idc` 等于 66、77 或 88 且 `level_idc` 不等于 11，或 `profile_idc` 不等于 66、77、88、100、110、122、244 或 44 时，解码器应忽略 `constraint_set3_flag` 的值。

`constraint_set4_flag` 的规定如下：

- 如果 `profile_idc` 等于 77、88、100 或 110，`constraint_set4_flag` 等于 1 表示 `frame_mbs_only_flag` 的值等于 1。`constraint_set4_flag` 等于 0 表示 `frame_mbs_only_flag` 的值可能等于或不等于 1。
- 否则，如果 `profile_idc` 等于 118、128 或 134，`constraint_set4_flag` 等于 1 表示编码视频序列遵守第 H.10.1.1 条规定的所有约束。`constraint_set4_flag` 等于 0 表示编码视频序列可能遵守或不遵守第 H.10.1.1 条规定的约束。
- 否则（`profile_idc` 不等于 77、88、100、110、118、128 或 134），`constraint_set4_flag` 的值为 1 保留给 ITU-T | ISO/IEC 将来使用。在符合本建议书 | 国际标准的比特流中，`profile_idc` 不等于 77、88、100、110、118、128 或 134 的编码视频序列的 `constraint_set4_flag` 应等于 0。当 `profile_idc` 不等于 77、88、100、110、118、128 或 134 时，解码器应忽略 `constraint_set4_flag` 的值。

`constraint_set5_flag` 的规定如下：

- 如果 `profile_idc` 等于 77、88 或 100，`constraint_set5_flag` 等于 1 表示编码视频序列中不存在 B 片类型。`constraint_set5_flag` 等于 0 表示编码视频序列中可能存在或不存在 B 片类型。
- 否则，如果 `profile_idc` 等于 118，`constraint_set5_flag` 等于 1 表示编码视频序列遵守第 H.10.1.2 条规定的所有约束。`constraint_set5_flag` 等于 0 表示编码视频序列可能遵守或不遵守第 H.10.1.2 条规定的约束。
- 否则（`profile_idc` 不等于 77、88、100 或 118），`constraint_set5_flag` 的值为 1 保留给 ITU-T | ISO/IEC 将来使用。在符合本建议书 | 国际标准的比特流中，`profile_idc` 不等于 77、88、100 或 118 的编码视频序列的 `constraint_set5_flag` 应等于 0。当 `profile_idc` 不等于 77、88、100 或 118 时，解码器应忽略 `constraint_set5_flag` 的值。
  > **注 2** – 对于符合多视图高和立体高配置文件的编码视频序列，使用 `profile_idc` 等于 118 和 `constraint_set5_flag` 等于 1 的组合。

`reserved_zero_2bits` 应等于 0。其他值的 `reserved_zero_2bits` 可能在将来由 ITU-T | ISO/IEC 规定。解码器应忽略 `reserved_zero_2bits` 的值。

`seq_parameter_set_id` 标识由图像参数集引用的序列参数集。`seq_parameter_set_id` 的值应在 0 到 31 范围内。
  > **注 3** – 当可行时，当其他序列参数集语法元素的值不同时，编码器应使用不同的 `seq_parameter_set_id` 值，而不是更改与特定 `seq_parameter_set_id` 值关联的语法元素的值。

`chroma_format_idc` 指定相对于亮度采样的色度采样，如第 6.2 条所述。`chroma_format_idc` 的值应在 0 到 3 范围内。当 `chroma_format_idc` 不存在时，推断其等于 1（4:2:0 色度格式）。

`separate_colour_plane_flag` 等于 1 表示 4:4:4 色度格式的三个颜色分量是分别编码的。`separate_colour_plane_flag` 等于 0 表示颜色分量不是分别编码的。当 `separate_colour_plane_flag` 不存在时，推断其等于 0。当 `separate_colour_plane_flag` 等于 1 时，主要编码图像由三个独立的分量组成，每个分量由一个颜色平面的编码样本组成（Y, Cb 或 Cr），每个分量使用单色编码语法。在这种情况下，每个颜色平面与特定的 `colour_plane_id` 值相关联。
  > **注 4** – 不同 `colour_plane_id` 值的颜色平面之间的解码过程中没有依赖关系。例如，具有一个 `colour_plane_id` 值的单色图像的解码过程不使用具有不同 `colour_plane_id` 值的单色图像中的任何数据进行帧间预测。

根据 `separate_colour_plane_flag` 的值，变量 `ChromaArrayType` 的值如下分配：

- 如果 `separate_colour_plane_flag` 等于 0，`ChromaArrayType` 设置等于 `chroma_format_idc`。
- 否则（`separate_colour_plane_flag` 等于 1），`ChromaArrayType` 设置等于 0。

`bit_depth_luma_minus8` 指定亮度阵列样本的位深度和亮度量化参数范围偏移量 `QpBdOffsetY` 的值，如下所示：
$$
\text{BitDepthY} = 8 + \text{bit\_depth\_luma\_minus8}\tag{7-3}
$$
$$
\text{QpBdOffsetY} = 6 * \text{bit\_depth\_luma\_minus8} \tag{7-4}
$$

当 `bit_depth_luma_minus8` 不存在时，应推断其等于 0。`bit_depth_luma_minus8` 的值应在 0 到 6 的范围内。

`bit_depth_chroma_minus8` 指定色度阵列样本的位深度和色度量化参数范围偏移量 `QpBdOffsetC` 的值，如下所示：
$$
\text{BitDepthC} = 8 + \text{bit\_depth\_chroma\_minus8} \tag{7-5}
$$
$$
\text{QpBdOffsetC} = 6 * \text{bit\_depth\_chroma\_minus8} \tag{7-6}
$$

当 `bit_depth_chroma_minus8` 不存在时，应推断其等于 0。`bit_depth_chroma_minus8` 的值应在 0 到 6 的范围内。
  > **注 5** – 当 `ChromaArrayType` 等于 0 时，解码过程中不使用 `bit_depth_chroma_minus8` 的值。特别是当 `separate_colour_plane_flag` 等于 1 时，每个颜色平面作为单独的单色图像进行解码，使用亮度分量解码过程（除了选择缩放矩阵外），亮度位深度用于所有三个颜色分量。

变量 `RawMbBits` 按如下派生：
$$
\text{RawMbBits} = 256 * \text{BitDepthY} + 2 * \text{MbWidthC} * \text{MbHeightC} * \text{BitDepthC} \tag{7-7}
$$

`qpprime_y_zero_transform_bypass_flag` 等于 1 表示当 `QP'Y` 等于 0 时，按照第 8.5 条的规定，在去块滤波过程之前对变换系数解码过程和图像构建过程应用变换旁路操作。`qpprime_y_zero_transform_bypass_flag` 等于 0 表示变换系数解码过程和图像构建过程在去块滤波过程之前不使用变换旁路操作。当 `qpprime_y_zero_transform_bypass_flag` 不存在时，应推断其等于 0。

`seq_scaling_matrix_present_flag` 等于 1 表示标志 `seq_scaling_list_present_flag[i]`（对于 `i = 0..7` 或 `i = 0..11`）存在。`seq_scaling_matrix_present_flag` 等于 0 表示这些标志不存在，并且对于 `i = 0..5` 推断为 `Flat_4x4_16` 指定的序列级缩放列表，对于 `i = 6..11` 推断为 `Flat_8x8_16` 指定的序列级缩放列表。当 `seq_scaling_matrix_present_flag` 不存在时，应推断其等于 0。

缩放列表 `Flat_4x4_16` 和 `Flat_8x8_16` 规定如下：
$$
\text{Flat\_4x4\_16[k]} = 16，其中k = 0..15\tag{7-8}
$$
$$
\text{Flat\_8x8\_16[k]} = 16，其中k = 0..63\tag{7-9}
$$

`seq_scaling_list_present_flag[i]` 等于 1 表示缩放列表 `i` 的语法结构存在于序列参数集中。`seq_scaling_list_present_flag[i]` 等于 0 表示缩放列表 `i` 的语法结构不存在于序列参数集中，并且应使用表 7-2 中指定的缩放列表回退规则集 A 推断索引 `i` 的序列级缩放列表。

Table 7-2
表 7-3 指定了默认的缩放列表`Default_4x4_Intra`和`Default_4x4_Inter`, 表 7-4 指定了默认的缩放列表`Default_8x8_Intra`和`Default_8x8_Inter`
Table 7-3
Table 7-4
Table 7-4 (continued)
Table 7-4 (continued)
Table 7-4 (continued)

`log2_max_frame_num_minus4` 指定变量 `MaxFrameNum` 的值，用于与 `frame_num` 相关的派生，如下所示：
$$
\text{MaxFrameNum} = 2^{(\text{log2\_max\_frame\_num\_minus4} + 4)}\tag{7-10}
$$
`log2_max_frame_num_minus4` 的值应在 0 到 12 的范围内。

`pic_order_cnt_type` 指定解码图片顺序计数的方法（如第 8.2.1 条所述）。`pic_order_cnt_type` 的值应在 0 到 2 的范围内。

在包含以下任一情况的编码视频序列中，`pic_order_cnt_type` 不应等于 2：

- 包含非参考帧的接入单元紧随包含非参考图片的接入单元之后，
- 两个接入单元每个包含一个场，这两个场一起形成一个互补的非参考场对，紧随其后是包含非参考图片的接入单元，
- 包含非参考场的接入单元紧随另一个非参考图片的接入单元之后，该图片不与这两个接入单元中的第一个形成互补的非参考场对。

`log2_max_pic_order_cnt_lsb_minus4` 指定变量 `MaxPicOrderCntLsb` 的值，用于解码过程中的图片顺序计数，如下所示：
$$
\text{MaxPicOrderCntLsb} = 2^{(\text{log2\_max\_pic\_order\_cnt\_lsb\_minus4} + 4)} \tag{7-11}
$$

`log2_max_pic_order_cnt_lsb_minus4` 的值应在 0 到 12 的范围内。

`delta_pic_order_always_zero_flag` 等于 1 表示在序列的片头中 `delta_pic_order_cnt[0]` 和 `delta_pic_order_cnt[1]` 不存在，应推断为 0。`delta_pic_order_always_zero_flag` 等于 0 表示在序列的片头中 `delta_pic_order_cnt[0]` 存在，`delta_pic_order_cnt[1]` 可能存在。

`offset_for_non_ref_pic` 用于计算非参考图片的图片顺序计数，如第 8.2.1 条所述。`offset_for_non_ref_pic` 的值应在 -2^31 + 1 到 2^31 - 1 的范围内。

`offset_for_top_to_bottom_field` 用于计算底场的图片顺序计数，如第 8.2.1 条所述。`offset_for_top_to_bottom_field` 的值应在 -2^31 + 1 到 2^31 - 1 的范围内。

`num_ref_frames_in_pic_order_cnt_cycle` 用于解码过程中的图片顺序计数，如第 8.2.1 条所述。`num_ref_frames_in_pic_order_cnt_cycle` 的值应在 0 到 255 的范围内。

`offset_for_ref_frame[i]` 是 `num_ref_frames_in_pic_order_cnt_cycle` 个值的列表中的一个元素，用于解码过程中的图片顺序计数，如第 8.2.1 条所述。`offset_for_ref_frame[i]` 的值应在 -2^31 + 1 到 2^31 - 1 的范围内。

当 `pic_order_cnt_type` 等于 1 时，变量 `ExpectedDeltaPerPicOrderCntCycle` 按如下派生：
    
```c
    ExpectedDeltaPerPicOrderCntCycle = 0
    for (i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)
      ExpectedDeltaPerPicOrderCntCycle += offset_for_ref_frame[i](7-12)
```


`max_num_ref_frames` 指定解码过程在编码视频序列中的任意图片的帧间预测中可能使用的短期和长期参考帧、互补参考场对以及非配对参考场的最大数量。`max_num_ref_frames` 还决定了滑动窗口操作的大小，如第 8.2.5.3 条所述。`max_num_ref_frames` 的值应在 0 到 `MaxDpbFrames`（如第 A.3.1 或 A.3.2 条所述）的范围内。

`gaps_in_frame_num_value_allowed_flag` 指定第 7.4.3 条规定的 `frame_num` 的允许值以及在推断的 `frame_num` 值之间存在间隙时的解码过程，如第 8.2.5.2 条所述。

`pic_width_in_mbs_minus1` 加 1 指定每个解码图片的宽度（以宏块为单位）。

变量 `PicWidthInMbs` 按如下派生：
$$
\text{PicWidthInMbs}=\text{pic\_width\_in\_mbs\_minus1}+1\tag{7-13}
$$
亮度分量的图片宽度变量按如下派生：
$$
\text{PicWidthInSamplesL}=\text{PicWidthInMbs} * 16\tag{7-14}
$$
色度分量的图片宽度变量按如下派生：
$$
\text{PicWidthInSamplesC}=\text{PicWidthInMbs} * \text{MbWidthC}\tag{7-15}
$$

`pic_height_in_map_units_minus1` 加 1 指定解码帧或场的切片组映射单元的高度。

变量 `PicHeightInMapUnits` 和 `PicSizeInMapUnits` 按如下派生：
$$
\text{PicHeightInMapUnits}=\text{pic\_height\_in\_map\_units\_minus1}+1\tag{7-16}
$$
$$
\text{PicSizeInMapUnits}=\text{PicWidthInMbs}*\text{PicHeightInMapUnits}\tag{7-17}
$$

`frame_mbs_only_flag` 等于 0 表示编码视频序列的编码图片可能是编码场或编码帧。`frame_mbs_only_flag` 等于 1 表示编码视频序列中的每个编码图片都是仅包含帧宏块的编码帧。

`pic_width_in_mbs_minus1`、`pic_height_in_map_units_minus1` 和 `frame_mbs_only_flag` 的允许值范围由附录 A 中的约束规定。

根据 `frame_mbs_only_flag`，为 `pic_height_in_map_units_minus1` 分配语义如下：

- 如果 `frame_mbs_only_flag` 等于 0，`pic_height_in_map_units_minus1` 加 1 是一个场的高度（以宏块为单位）。
- 否则（`frame_mbs_only_flag` 等于 1），`pic_height_in_map_units_minus1` 加 1 是一个帧的高度（以宏块为单位）。

变量 `FrameHeightInMbs` 按如下派生：
$$
\text{FrameHeightInMbs}=(2-\text{frame\_mbs\_only\_flag})*\text{PicHeightInMapUnits}\tag{7-18}
$$

`mb_adaptive_frame_field_flag` 等于 0 表示在一个图像中不进行帧和场宏块之间的切换。`mb_adaptive_frame_field_flag` 等于 1 表示在帧内可能进行帧和场宏块之间的切换。当 `mb_adaptive_frame_field_flag` 不存在时，应推断其等于 0。

`direct_8x8_inference_flag` 指定在第 8.4.1.2 条所述的推导过程中用于 B_Skip、B_Direct_16x16 和 B_Direct_8x8 的亮度运动矢量的方法。当 `frame_mbs_only_flag` 等于 0 时，`direct_8x8_inference_flag` 应等于 1。

`frame_cropping_flag` 等于 1 表示帧裁剪偏移参数紧接在序列参数集之后。`frame_cropping_flag` 等于 0 表示帧裁剪偏移参数不存在。

`frame_crop_left_offset`、`frame_crop_right_offset`、`frame_crop_top_offset`、`frame_crop_bottom_offset` 指定在解码过程中输出的编码视频序列中图片的样本，按输出的帧坐标指定的矩形区域。

变量 `CropUnitX` 和 `CropUnitY` 的派生如下：

- 如果 `ChromaArrayType` 等于 0，`CropUnitX` 和 `CropUnitY` 按如下派生：
$$
\text{CropUnitX}=1\tag{7-19}
$$
$$
\text{CropUnitY}=2-\text{frame\_mbs\_only\_flag}\tag{7-20}
$$

- 否则（`ChromaArrayType` 等于 1、2 或 3），`CropUnitX` 和 `CropUnitY` 按如下派生：
$$
\text{CropUnitX}=\text{SubWidthC}\tag{7-21}
$$
$$
\text{CropUnitY}=\text{SubHeightC}*(2-\text{frame\_mbs\_only\_flag})\tag{7-22}
$$

帧裁剪矩形包含的亮度样本的水平帧坐标从 `CropUnitX * frame_crop_left_offset` 到 `PicWidthInSamplesL − (CropUnitX * frame_crop_right_offset + 1)`，垂直帧坐标从 `CropUnitY * frame_crop_top_offset` 到 `(16 * FrameHeightInMbs) − (CropUnitY * frame_crop_bottom_offset + 1)`，两者均包括在内。`frame_crop_left_offset` 的值应在 0 到 `(PicWidthInSamplesL / CropUnitX) − (frame_crop_right_offset + 1)` 的范围内，包括该范围；`frame_crop_top_offset` 的值应在 0 到 `(16 * FrameHeightInMbs / CropUnitY) − (frame_crop_bottom_offset + 1)` 的范围内，包括该范围。

当 `frame_cropping_flag` 等于 0 时，`frame_crop_left_offset`、`frame_crop_right_offset`、`frame_crop_top_offset` 和 `frame_crop_bottom_offset` 的值应推断为 0。

当 `ChromaArrayType` 不等于 0 时，两色度阵列的相应样本是具有帧坐标 `(x / SubWidthC, y / SubHeightC)` 的样本，其中 `(x, y)` 是指定的亮度样本的帧坐标。
对于解码场，解码场的指定样本是在帧坐标中指定的矩形内的样本。
`vui_parameters_present_flag` 等于 1 表示存在附录 E 中指定的 `vui_parameters()` 语法结构。`vui_parameters_present_flag` 等于 0 表示不存在附录 E 中指定的 `vui_parameters()` 语法结构。

###### 7.4.2.1.1.1 缩放列表语义

`delta_scale` 用于派生缩放列表中第 j 个元素，其中 j 的范围是 0 到 `sizeOfScalingList − 1`（含）。`delta_scale` 的值应在 -128 到 +127 之间（含）。
当 `useDefaultScalingMatrixFlag` 派生为 1 时，缩放列表应推断为表 7-2 中指定的默认缩放列表。

##### 7.4.2.1.2 序列参数集扩展 RBSP 语义

`seq_parameter_set_id` 标识与序列参数集扩展相关联的序列参数集。`seq_parameter_set_id` 的值应在 0 到 31 之间（含）。

`aux_format_idc` 等于 0 表示编码视频序列中没有辅助编码图片。`aux_format_idc` 等于 1 表示每个接入单元中存在一个辅助编码图片，并且在解码过程后的显示过程中，出于 alpha 混合目的，每个接入单元中与其关联的主编码图片的解码样本应乘以该接入单元中辅助编码图片的解释样本值。`aux_format_idc` 等于 2 表示每个接入单元中存在一个辅助编码图片，并且出于 alpha 混合目的，在解码过程后的显示过程中，每个接入单元中与其关联的主编码图片的解码样本不应乘以该接入单元中辅助编码图片的解释样本值。`aux_format_idc` 等于 3 表示每个接入单元中存在一个辅助编码图片，但未指定辅助编码图片的用途。`aux_format_idc` 的值应在 0 到 3 之间（含）。`aux_format_idc` 大于 3 的值保留用于将来由 ITU-T | ISO/IEC 指定的其他目的。当 `aux_format_idc` 不存在时，应推断为 0。
  **注 1**：解码器不需要解码辅助编码图片。

`bit_depth_aux_minus8` 指定辅助编码图片样本数组的样本位深度。`bit_depth_aux_minus8` 的值应在 0 到 4 之间（含）。

`alpha_incr_flag` 等于 0 表示每个解码辅助编码图片样本值的解释样本值等于解码辅助编码图片样本值，用于 alpha 混合目的。`alpha_incr_flag` 等于 1 表示，对于 alpha 混合目的，在解码辅助编码图片样本后，任何大于 `Min(alpha_opaque_value, alpha_transparent_value)` 的辅助编码图片样本值应增加 1 以获得辅助编码图片样本的解释样本值，任何小于或等于 `Min(alpha_opaque_value, alpha_transparent_value)` 的辅助编码图片样本值应未经更改地用作解码辅助编码图片样本值的解释样本值。

`alpha_opaque_value` 指定辅助编码图片样本的解释样本值，对于同一接入单元中与其关联的亮度和色度样本，出于 alpha 混合目的，视为不透明。`alpha_opaque_value` 语法元素的表示所用的位数为 `bit_depth_aux_minus8 + 9` 位。

`alpha_transparent_value` 指定辅助编码图片样本的解释样本值，对于同一接入单元中与其关联的亮度和色度样本，出于 alpha 混合目的，视为透明。`alpha_transparent_value` 语法元素的表示所用的位数为 `bit_depth_aux_minus8 + 9` 位。

当 `alpha_incr_flag` 等于 1 时，`alpha_transparent_value` 不应等于 `alpha_opaque_value`，并且 `Log2(Abs(alpha_opaque_value − alpha_transparent_value))` 应具有整数值。`alpha_transparent_value` 等于 `alpha_opaque_value` 表示辅助编码图片不打算用于 alpha 混合目的。

  **注 2**：出于 alpha 混合目的，`alpha_opaque_value` 可能大于 `alpha_transparent_value`，也可能小于 `alpha_transparent_value`。解释样本值应截取在 `alpha_opaque_value` 到 `alpha_transparent_value`（含）范围内。

  序列参数集扩展的解码和辅助编码图片的解码不是符合此推荐标准的要求。

每个辅助编码图片的编码切片的语法应遵守与冗余图片编码切片相同的约束，以下约束差异除外：
  - 关于主编码图片是否为 IDR 图片，以下适用：
    - 如果主编码图片是 IDR 图片，辅助编码切片语法应对应于具有 `nal_unit_type` 等于 5（IDR 图片的切片）的切片。
    - 否则（主编码图片不是 IDR 图片），辅助编码切片语法应对应于具有 `nal_unit_type` 等于 1（非 IDR 图片的切片）的切片。
  - 辅助编码图片的切片（如果存在）应包含与主编码图片对应的所有宏块。
  - 在所有辅助编码切片中，`redundant_pic_cnt` 应等于 0。

辅助编码图片的（可选）解码过程与当前编码视频流中主编码图片不同，辅助编码图片作为单独编码视频流中的主编码图片的解码过程如下：
  - 辅助编码图片的 IDR 或非 IDR 状态应推断为与同一接入单元中的主图片的 IDR 或非 IDR 状态相同，而不是从 `nal_unit_type` 值中推断。
  - 用于辅助编码图片解码的 `chroma_format_idc` 和 `ChromaArrayType` 值应推断为 0。
  - 用于辅助编码图片解码的 `bit_depth_luma_minus8` 值应推断为 `bit_depth_aux_minus8`。
  **注 3**：Alpha 混合合成通常使用背景图片 B、前景图片 F 和解码的辅助编码图片 A，三者大小相同。假设为了示例说明，B 和 F 的色度分辨率已上采样到与亮度相同的分辨率。用 b、f 和 a 分别表示 B、F 和 A 的对应样本。用下标 Y、Cb 和 Cr 表示亮度和色度样本。

定义变量 `alphaRange`、`alphaFwt` 和 `alphaBwt` 如下：

```c
    alphaRange = Abs(alpha_opaque_value − alpha_transparent_value)
    alphaFwt = Abs(a − alpha_transparent_value)
    alphaBwt = Abs(a − alpha_opaque_value)
```

然后，在 alpha 混合合成中，显示图片 D 的样本 d 可按如下计算：
```c
    dY = (alphaFwt * fY + alphaBwt * bY + alphaRange / 2) / alphaRange
    dCb = (alphaFwt * fCb + alphaBwt * bCb + alphaRange / 2) / alphaRange
    dCr = (alphaFwt * fCr + alphaBwt * bCr + alphaRange / 2) / alphaRange
```

图片 D、F 和 B 的样本也可以表示红、绿和蓝分量值（参见第 E.2.1 条）。这里我们假设 Y、Cb 和 Cr 分量值。为示例说明假设每个分量（例如 Y）在图片 D、F 和 B 中具有相同的位深度。然而，在这个例子中，不同的分量（例如 Y 和 Cb）不需要具有相同的位深度。

当 `aux_format_idc` 等于 1 时，F 将是从解码的亮度和色度获得的解码图片，A 将是从解码的辅助编码图片获得的解码图片。在这种情况下，所示的示例 alpha 混合合成涉及将 F 的样本乘以从 A 的样本获得的因子。

一种用于编辑或直接观看的常用图片格式称为预乘黑视频。如果前景图片是 F，那么预乘黑视频 S 由以下公式给出：
```c
    sY = (alphaFwt * fY) / alphaRange
    sCb = (alphaFwt * fCb) / alphaRange
    sCr = (alphaFwt * fCr) / alphaRange
```
预乘黑视频的特点是如果在黑色背景上显示，图片 S 将显示正确。对于非黑色背景 B，显示图片 D 的合成可按如下计算：
```c
    dY = sY + (alphaBwt * bY + alphaRange / 2) / alphaRange
    dCb = sCb + (alphaBwt * bCb + alphaRange / 2) / alphaRange
    dCr = sCr + (alphaBwt * bCr + alphaRange / 2) / alphaRange
```

当 `aux_format_idc` 等于 2 时，S 将是从解码的亮度和色度获得的解码图片，A 仍然是从解码的辅助编码图片获得的解码图片。在这种情况下，alpha 混合合成不涉及将 S 的样本乘以从 A 的样本获得的因子。

`additional_extension_flag` 等于 0 表示在序列参数集扩展语法结构内，在 RBSP 尾随位之前没有附加数据。`additional_extension_flag` 的值应等于 0。`additional_extension_flag` 的值为 1 保留用于将来由 ITU-T | ISO/IEC 指定的用途。解码器应忽略序列参数集扩展 NAL 单元中 `additional_extension_flag` 值为 1 后的所有数据。

##### 7.4.2.1.3 子集序列参数集 RBSP 语义

`svc_vui_parameters_present_flag` 等于 0 表示语法结构 `svc_vui_parameters_extension( )` 不存在。`svc_vui_parameters_present_flag` 等于 1 表示语法结构 `svc_vui_parameters_extension( )` 存在。

`bit_equal_to_one` 应等于 1。

`mvc_vui_parameters_present_flag` 等于 0 表示语法结构 `mvc_vui_parameters_extension( )` 不存在。`mvc_vui_parameters_present_flag` 等于 1 表示语法结构 `mvc_vui_parameters_extension( )` 存在。

`additional_extension2_flag` 等于 0 表示在子集序列参数集 RBSP 语法结构中不存在 `additional_extension2_data_flag` 语法元素。符合本建议 | 国际标准的比特流中，`additional_extension2_flag` 应等于 0。`additional_extension2_flag` 的值为 1 保留供 ITU-T | ISO/IEC 将来使用。解码器应忽略子集序列参数集 NAL 单元中 `additional_extension2_flag` 值为 1 后的所有数据。

`additional_extension2_data_flag` 可以是任意值。它不应影响附录 A、G、H 或 I 中指定的配置文件的符合性。

#### 7.4.2.2 图像参数集 RBSP 语义

`pic_parameter_set_id` 用于标识在切片头中引用的图像参数集。`pic_parameter_set_id` 的值应在 0 到 255 的范围内（包括 0 和 255）。

`seq_parameter_set_id` 用于指示活动的序列参数集。`seq_parameter_set_id` 的值应在 0 到 31 的范围内（包括 0 和 31）。

`entropy_coding_mode_flag` 用于选择对语法元素应用的熵解码方法，语法表中会出现两个描述符，如下所示：
- 如果 `entropy_coding_mode_flag` 等于 0，则应用语法表中左侧描述符指定的方法（Exp-Golomb 编码，见第 9.1 节或 CAVLC，见第 9.2 节）。
- 否则（`entropy_coding_mode_flag` 等于 1），应用语法表中右侧描述符指定的方法（CABAC，见第 9.3 节）。

`bottom_field_pic_order_in_frame_present_flag` 等于 1 表示在切片头中存在与编码帧底场的图像顺序计数相关的语法元素 `delta_pic_order_cnt_bottom`（当 `pic_order_cnt_type` 等于 0 时）或 `delta_pic_order_cnt[ 1 ]`（当 `pic_order_cnt_type` 等于 1 时），这些元素在第 7.3.3 节中有所规定。`bottom_field_pic_order_in_frame_present_flag` 等于 0 表示在切片头中不存在 `delta_pic_order_cnt_bottom` 和 `delta_pic_order_cnt[ 1 ]` 语法元素。

`num_slice_groups_minus1` 加 1 用于指定图像的切片组数量。当 `num_slice_groups_minus1` 等于 0 时，图像的所有切片都属于同一个切片组。`num_slice_groups_minus1` 的允许范围在附录 A 中有规定。

`slice_group_map_type` 指定切片组映射单元到切片组的编码方式。`slice_group_map_type` 的值应在 0 到 6 的范围内（包括 0 和 6）。

- `slice_group_map_type` 等于 0 指定交错的切片组。
- `slice_group_map_type` 等于 1 指定分散的切片组映射。
- `slice_group_map_type` 等于 2 指定一个或多个“前景”切片组和一个“剩余”切片组。

`slice_group_map_type` 的值为 3、4 和 5 指定了变化的切片组。当 `num_slice_groups_minus1` 不等于 1 时，`slice_group_map_type` 不应等于 3、4 或 5。

- `slice_group_map_type` 等于 6 指定对每个切片组映射单元的显式切片组分配。

切片组映射单元的定义如下：
- 如果 `frame_mbs_only_flag` 等于 0 且 `mb_adaptive_frame_field_flag` 等于 1 且编码图像是帧，则切片组映射单元是宏块对单元。
- 否则，如果 `frame_mbs_only_flag` 等于 1 或编码图像是场，则切片组映射单元是宏块单元。
- 否则（`frame_mbs_only_flag` 等于 0 且 `mb_adaptive_frame_field_flag` 等于 0 且编码图像是帧），切片组映射单元是两个在帧中垂直相邻的宏块单元（如 MBAFF 帧的宏块对）。

`run_length_minus1[ i ]` 用于指定在切片组映射单元的栅格扫描顺序中，分配给第 i 个切片组的连续切片组映射单元数量。`run_length_minus1[ i ]` 的值应在 0 到 `PicSizeInMapUnits − 1` 的范围内（包括 0 和 `PicSizeInMapUnits − 1`）。

`top_left[ i ]` 和 `bottom_right[ i ]` 分别指定一个矩形的左上角和右下角。`top_left[ i ]` 和 `bottom_right[ i ]` 是切片组映射单元在图像中栅格扫描的位置信息。对于每个矩形 i，`top_left[ i ]` 和 `bottom_right[ i ]` 的语法元素值必须满足以下约束：
- `top_left[ i ]` 必须小于或等于 `bottom_right[ i ]`，并且 `bottom_right[ i ]` 必须小于 `PicSizeInMapUnits`。
- (`top_left[ i ] % PicWidthInMbs`) 必须小于或等于 (`bottom_right[ i ] % PicWidthInMbs`) 的值。

`slice_group_change_direction_flag` 与 `slice_group_map_type` 一起用于指定当 `slice_group_map_type` 为 3、4 或 5 时的精细化映射类型。

`slice_group_change_rate_minus1` 用于指定变化的 `SliceGroupChangeRate`。`SliceGroupChangeRate` 指定了从一帧到下一帧切片组的大小可以变化的切片组映射单元数量倍数。`slice_group_change_rate_minus1` 的值应在 0 到 `PicSizeInMapUnits − 1` 的范围内（包括 0 和 `PicSizeInMapUnits − 1`）。`SliceGroupChangeRate` 变量定义如下：

$$
\text{SliceGroupChangeRate} = \text{slice\_group\_change\_rate\_minus1} + 1  \tag{7-23}
$$

`pic_size_in_map_units_minus1` 用于指定图像中切片组映射单元的数量。`pic_size_in_map_units_minus1` 应等于 `PicSizeInMapUnits − 1`。

`slice_group_id[ i ]` 标识在栅格扫描顺序中第 i 个切片组映射单元所属的切片组。`slice_group_id[ i ]` 语法元素的长度为 `Ceil( Log2( num_slice_groups_minus1 + 1 ) )` 比特。`slice_group_id[ i ]` 的值应在 0 到 `num_slice_groups_minus1` 的范围内（包括 0 和 `num_slice_groups_minus1`）。

`num_ref_idx_l0_default_active_minus1` 指定 `num_ref_idx_l0_active_minus1` 的推断方式，当 `num_ref_idx_active_override_flag` 等于 0 时适用于 P、SP 和 B 切片。`num_ref_idx_l0_default_active_minus1` 的值应在 0 到 31 的范围内（包括 0 和 31）。

`num_ref_idx_l1_default_active_minus1` 指定 `num_ref_idx_l1_active_minus1` 的推断方式，当 `num_ref_idx_active_override_flag` 等于 0 时适用于 B 切片。`num_ref_idx_l1_default_active_minus1` 的值应在 0 到 31 的范围内（包括 0 和 31）。

`weighted_pred_flag` 等于 0 指定默认加权预测应应用于 P 和 SP 切片。`weighted_pred_flag` 等于 1 指定显式加权预测应应用于 P 和 SP 切片。

`weighted_bipred_idc` 等于 0 指定默认加权预测应应用于 B 切片。`weighted_bipred_idc` 等于 1 指定显式加权预测应应用于 B 切片。`weighted_bipred_idc` 等于 2 指定隐式加权预测应应用于 B 切片。`weighted_bipred_idc` 的值应在 0 到 2 的范围内（包括 0 和 2）。

`pic_init_qp_minus26` 指定每个切片的 `SliceQPY` 初始值减去 26。该初始值在切片层当解码到非零值的 `slice_qp_delta` 时会被修改，在宏块层当解码到非零值的 `mb_qp_delta` 时会进一步修改。`pic_init_qp_minus26` 的值应在 `−(26 + QpBdOffsetY )` 到 +25 的范围内（包括 `−(26 + QpBdOffsetY )` 和 +25）。

`pic_init_qs_minus26` 指定 SP 或 SI 切片中所有宏块的 `SliceQSY` 初始值减去 26。该初始值在切片层当解码到非零值的 `slice_qs_delta` 时会被修改。`pic_init_qs_minus26` 的值应在 `−26` 到 `+25` 的范围内（包括 `−26` 和 `+25`）。

`chroma_qp_index_offset` 指定应添加到 `QPY` 和 `QSY` 的偏移量，用于地址指定 Cb 色度分量的 `QPC` 值表。`chroma_qp_index_offset` 的值应在 `−12` 到 `+12` 的范围内（包括 `−12` 和 `+12`）。

`deblocking_filter_control_present_flag` 等于 1 指定在切片头中存在控制去块滤波器特性的语法元素集合。`deblocking_filter_control_present_flag` 等于 0 指定在切片头中不存在控制去块滤波器特性的语法元素集合，并且其推断值有效。

`constrained_intra_pred_flag` 等于 0 指定帧内预测允许使用残差数据和相邻宏块的解码样本，这些宏块使用帧间宏块预测模式编码，用于帧内宏块预测模式编码的宏块的预测。`constrained_intra_pred_flag` 等于 1 指定受限帧内预测，在这种情况下，帧内宏块预测模式编码的宏块的预测仅使用来自 I 或 SI 宏块类型的残差数据和解码样本。

`redundant_pic_cnt_present_flag` 等于 0 指定在引用（直接或通过与相应的编码切片数据分区 A NAL 单元关联）图像参数集的切片头、编码切片数据分区 B NAL 单元和编码切片数据分区 C NAL 单元中不存在 `redundant_pic_cnt` 语法元素。`redundant_pic_cnt_present_flag` 等于 1 指定在引用（直接或通过与相应的编码切片数据分区 A NAL 单元关联）图像参数集的所有切片头、编码切片数据分区 B NAL 单元和编码切片数据分区 C NAL 单元中存在 `redundant_pic_cnt` 语法元素。

`transform_8x8_mode_flag` 等于 1 指定 8x8 变换解码过程可能在使用中（见第 8.5 节）。`transform_8x8_mode_flag` 等于 0 指定 8x8 变换解码过程未在使用中。当 `transform_8x8_mode_flag` 不存在时，应推断其为 0。

`pic_scaling_matrix_present_flag` 等于 1 指定存在用于修改序列参数集中指定的缩放列表的参数。`pic_scaling_matrix_present_flag` 等于 0 指定用于图像的缩放列表应推断为等于序列参数集中指定的缩放列表。当 `pic_scaling_matrix_present_flag` 不存在时，应推断其为 0。

`pic_scaling_list_present_flag[ i ]` 等于 1 指定存在缩放列表语法结构，以指定索引 `i` 的缩放列表。`pic_scaling_list_present_flag[ i ]` 等于 0 指定图像参数集中不存在索引 `i` 的缩放列表语法结构，并且根据 `seq_scaling_matrix_present_flag` 的值，适用以下规则：
- 如果 `seq_scaling_matrix_present_flag` 等于 0，则使用表 7-2 中指定的缩放列表回退规则集 A 来推导图像级缩放列表。
- 否则（`seq_scaling_matrix_present_flag` 等于 1），使用表 7-2 中指定的缩放列表回退规则集 B 来推导图像级缩放列表。

`second_chroma_qp_index_offset` 指定应添加到 `QPY` 和 `QSY` 的偏移量，用于地址指定 Cr 色度分量的 `QPc` 值表。`second_chroma_qp_index_offset` 的值应在 −12 到 +12 的范围内（包括 −12 和 +12）。

当 `second_chroma_qp_index_offset` 不存在时，应推断其等于 `chroma_qp_index_offset`。

**注意** – 当 `ChromaArrayType` 等于 0 时，`bit_depth_chroma_minus8`、`chroma_qp_index_offset` 和 `second_chroma_qp_index_offset` 的值在解码过程中不使用。特别是当 `separate_colour_plane_flag` 等于 1 时，每个颜色平面都被解码为独立的单色图像，使用亮度分量解码过程（除了选择缩放矩阵外），包括应用亮度量化参数推导过程，但不应用偏移量来解码 `colour_plane_id` 不等于 0 的图像。

#### 7.4.2.3 补充增强信息 RBSP 语义

补充增强信息（SEI）包含不需要从 VCL NAL 单元解码编码图像样本的信息。

##### 7.4.2.3.1 补充增强信息消息语义

一个 SEI RBSP 包含一个或多个 SEI 消息。每个 SEI 消息由指定 SEI 负载类型 `payloadType` 和 SEI 负载大小 `payloadSize` 的变量组成。SEI 负载在附录 D 中指定。派生的 SEI 负载大小 `payloadSize` 以字节为单位，应等于 SEI 负载中 RBSP 字节的数量。

**注意** – 包含 SEI 消息的 NAL 单元字节序列可能包括一个或多个仿真防止字节（由 `emulation_prevention_three_byte` 语法元素表示）。由于 SEI 消息的负载大小以 RBSP 字节表示，因此仿真防止字节的数量不包括在 SEI 负载的 `payloadSize` 大小中。

`ff_byte` 是一个字节，等于 0xFF，标识需要更长表示的语法结构。

`last_payload_type_byte` 是 SEI 消息负载类型的最后一个字节。

`last_payload_size_byte` 是 SEI 消息负载大小的最后一个字节。

#### 7.4.2.4 访问单元分隔符 RBSP 语义

访问单元分隔符可用于指示主要编码图像中存在的切片类型，并简化访问单元边界的检测。与访问单元分隔符相关的解码过程没有规范要求。

`primary_pic_type` 指示主要编码图像所有切片的 `slice_type` 值属于表 7-5 中给定 `primary_pic_type` 值的集合。

**注意** – `primary_pic_type` 的值适用于主要编码图像所有切片头中的 `slice_type` 值，包括所有 NAL 单元中 `nal_unit_type` 等于 1、2 或 5 的 `slice_type` 语法元素。

Table 7-5 – Meaning of primary_pic_type

### 7.4.2.5 序列结束 RBSP 语义

序列结束 RBSP 指定比特流中解码顺序上的下一个访问单元（如果有的话）应为 IDR 访问单元。序列结束 RBSP 的 SODB 和 RBSP 的语法内容为空。序列结束 RBSP 没有规定规范的解码过程。

### 7.4.2.6 流结束 RBSP 语义

流结束 RBSP 表示在解码顺序上没有附加的 NAL 单元会出现在流结束 RBSP 之后。流结束 RBSP 的 SODB 和 RBSP 的语法内容为空。流结束 RBSP 没有规定规范的解码过程。
  
**注意** – 当存在流结束 NAL 单元时，比特流被认为结束（就本推荐 | 国际标准的范围而言）。在某些系统环境中，另一个比特流可能在结束的比特流之后跟随，可能是立即或者稍后，可能在同一个通信通道内。在这种情况下，本推荐 | 国际标准的范围仅适用于每个独立比特流的处理。对于这种比特流之间的转换（例如在时间、缓冲操作等方面），本文未规定任何要求。

### 7.4.2.7 填充数据 RBSP 语义

填充数据 RBSP 包含零个或多个字节。填充数据 RBSP 没有规定规范的解码过程。

`ff_byte` 是一个字节。比特流符合性的要求是 `ff_byte` 的值应等于 0xFF。

### 7.4.2.8 无分区切片层 RBSP 语义

无分区切片层 RBSP 由切片头和切片数据组成。

### 7.4.2.9 切片数据分区 RBSP 语义

#### 7.4.2.9.1 切片数据分区 A RBSP 语义

当使用切片数据分区时，一个切片的编码数据被分为三个独立的分区。切片数据分区 A 包含所有类别 2 的语法元素。类别 2 语法元素包括切片头和切片数据语法结构中的所有语法元素，除了 `residual( )` 语法结构中的语法元素。

`slice_id` 标识与切片数据分区相关的切片。`slice_id` 的值受以下约束：

- 如果 `separate_colour_plane_flag` 等于 0，以下规则适用：
  - 如果不允许任意切片顺序（如附录 A 中规定），则编码图像的第一个切片（按解码顺序）应具有 `slice_id` 等于 0，并且每个后续切片的 `slice_id` 按解码顺序递增 1。
  - 否则（允许任意切片顺序），每个切片在编码图像的切片集中应具有唯一的 `slice_id` 值。

- 否则（`separate_colour_plane_flag` 等于 1），以下规则适用：
  - 如果不允许任意切片顺序（如附录 A 中规定），则每个颜色平面的编码图像的第一个切片（按解码顺序）应具有 `slice_id` 等于 0，并且每个后续切片（具有相同 `colour_plane_id` 值）的 `slice_id` 按解码顺序递增 1。
  - 否则（允许任意切片顺序），每个切片在具有相同 `colour_plane_id` 值的编码图像的切片集中应具有唯一的 `slice_id` 值。

`slice_id` 的范围如下：
- 如果 `MbaffFrameFlag` 等于 0，`slice_id` 应在 0 到 `PicSizeInMbs − 1` 的范围内（包括 0 和 `PicSizeInMbs − 1`）。
- 否则（`MbaffFrameFlag` 等于 1），`slice_id` 应在 0 到 `PicSizeInMbs / 2 − 1` 的范围内（包括 0 和 `PicSizeInMbs / 2 − 1`）。

### 7.4.2.9.2 切片数据分区 B RBSP 语义

当使用切片数据分区时，一个切片的编码数据被分为一到三个独立的分区。切片数据分区 B 包含所有类别 3 的语法元素。

类别 3 语法元素包括 `residual()` 语法结构和在该语法结构中使用的所有语法元素，适用于表 7-10 中指定的集体宏块类型 I 和 SI。

`slice_id` 具有与第 7.4.2.9.1 节中规定相同的语义。

`colour_plane_id` 指定当 `separate_colour_plane_flag` 等于 1 时，与当前切片 RBSP 相关的颜色平面。`colour_plane_id` 的值应在 0 到 2 的范围内，包括 0 和 2。`colour_plane_id` 等于 0、1 和 2 分别对应于 Y、Cb 和 Cr 平面。
  
**注意** – 在具有不同 `colour_plane_id` 值的图像的解码过程中没有依赖关系。

`redundant_pic_cnt` 对于属于主编码图像的编码切片和编码切片数据分区，应等于 0。对于冗余编码图像中的编码切片和编码切片数据分区，`redundant_pic_cnt` 应大于 0。当 `redundant_pic_cnt` 不存在时，其值应推定为 0。`redundant_pic_cnt` 的值应在 0 到 127 的范围内，包括 0 和 127。

切片数据分区 B RBSP 的存在如下规定：
- 如果切片数据分区 A RBSP 的语法元素指示在切片数据中存在任何类别 3 的语法元素，则应存在一个具有与切片数据分区 A RBSP 相同的 `slice_id` 和 `redundant_pic_cnt` 的切片数据分区 B RBSP。
- 否则（切片数据分区 A RBSP 的语法元素未指示在切片数据中存在任何类别 3 的语法元素），不应存在具有与切片数据分区 A RBSP 相同的 `slice_id` 和 `redundant_pic_cnt` 的切片数据分区 B RBSP。

### 7.4.2.9.3 切片数据分区 C RBSP 语义

当使用切片数据分区时，一个切片的编码数据被分为三个独立的分区。切片数据分区 C 包含所有类别 4 的语法元素。

类别 4 语法元素包括 `residual()` 语法结构和在该语法结构中使用的所有语法元素，适用于表 7-10 中指定的集体宏块类型 P 和 B。

`slice_id` 具有与第 7.4.2.9.1 节中规定相同的语义。

`colour_plane_id` 具有与第 7.4.2.9.2 节中规定相同的语义。

`redundant_pic_cnt` 具有与第 7.4.2.9.2 节中规定相同的语义。

切片数据分区 C RBSP 的存在如下规定：
- 如果切片数据分区 A RBSP 的语法元素指示在切片数据中存在任何类别 4 的语法元素，则应存在一个具有与切片数据分区 A RBSP 相同的 `slice_id` 和 `redundant_pic_cnt` 的切片数据分区 C RBSP。
- 否则（切片数据分区 A RBSP 的语法元素未指示在切片数据中存在任何类别 4 的语法元素），不应存在具有与切片数据分区 A RBSP 相同的 `slice_id` 和 `redundant_pic_cnt` 的切片数据分区 C RBSP。

### 7.4.2.10 RBSP 切片尾随比特语义

`cabac_zero_word` 是一个字节对齐的两个字节序列，等于 0x0000。

令 `NumBytesInVclNALunits` 为所有编码图像的 VCL NAL 单元的 `NumBytesInNALunit` 值之和。

令 `BinCountsInNALunits` 为解码所有编码图像的 VCL NAL 单元的内容时调用解析过程函数 `DecodeBin()` 的次数，如第 9.3.3.2 节所规定。当 `entropy_coding_mode_flag` 等于 1 时，比特流符合性的要求是 `BinCountsInNALunits` 不应超过 `(32 ÷ 3) * NumBytesInVclNALunits + (RawMbBits * PicSizeInMbs) ÷ 32`。
  
**注意** – 可以通过插入若干 `cabac_zero_word` 语法元素来增加 `NumBytesInVclNALunits` 的值，从而满足从切片层 NAL 单元解码内容得到的最大 bin 数量约束。每个 `cabac_zero_word` 由 NAL 单元中的三字节序列 0x000003 表示（由于 NAL 单元内容的约束，需要包含每个 `cabac_zero_word` 的 `emulation_prevention_three_byte`）。

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

## 附录B 字节流格式

（本附录是本建议书|国际标准的组成部分。）

本附录规定了字节流格式的语法和语义，该格式用于将部分或全部NAL单元流作为字节或比特的有序流传输，其中需要从数据中的模式识别NAL单元边界，例如Rec. ITU-T H.222.0 | ISO/IEC 13818-1系统或Rec. ITU-T H.320系统。对于面向比特的传输，字节流格式的比特顺序规定为从第一个字节的MSB开始，依次到第一个字节的LSB，然后是第二个字节的MSB，依此类推。

字节流格式由一系列字节流NAL单元语法结构组成。每个字节流NAL单元语法结构包含一个起始码前缀，后跟一个nal_unit（NumBytesInNALunit）语法结构。在某些情况下，它还可能（并且在某些情况下必须）包含一个额外的zero_byte语法元素。它也可能包含一个或多个额外的trailing_zero_8bits语法元素。当它是位流中的第一个字节流NAL单元时，它还可能包含一个或多个额外的leading_zero_8bits语法元素。

### B.1 字节流NAL单元语法和语义

#### B.1.1 字节流NAL单元语法

#### B.1.2 字节流NAL单元语义

字节流中的字节流NAL单元顺序应遵循字节流NAL单元中包含的NAL单元的解码顺序（见第7.4.1.2条）。每个字节流NAL单元的内容与字节流NAL单元中包含的NAL单元相关联（见第7.4.1.2.3条）。

leading_zero_8bits是一个等于0x00的字节。

注意 - leading_zero_8bits语法元素只能出现在位流中的第一个字节流NAL单元中，因为（如B.1.1条的语法图所示）在NAL单元语法结构之后且在四字节序列0x00000001之前的任何等于0x00的字节将被视为前一个字节流NAL单元的一部分trailing_zero_8bits语法元素。zero_byte是一个等于0x00的字节。

当以下任何条件为真时，zero_byte语法元素必须存在：
- nal_unit（）中的nal_unit_type等于7（序列参数集）或8（图像参数集），
- 字节流NAL单元语法结构包含解码顺序中的访问单元的第一个NAL单元，如第7.4.1.2.3条所述。

start_code_prefix_one_3bytes是一个固定值的3字节序列，等于0x000001。此语法元素称为起始码前缀。trailing_zero_8bits是一个等于0x00的字节。

B.2 字节流NAL单元解码过程

此过程的输入由一个有序的字节流组成，该字节流包含一系列字节流NAL单元语法结构。

此过程的输出由一系列NAL单元语法结构组成。

在解码过程开始时，解码器将当前在字节流中的位置初始化为字节流的起始位置。然后，它提取并丢弃每个leading_zero_8bits语法元素（如果存在），在字节流中每次向前移动一个字节，直到字节流中的当前位置使位流中的下一个四字节序列形成四字节序列0x00000001。

然后，解码器重复执行以下步骤过程以提取和解码字节流中的每个NAL单元语法结构，直到遇到字节流的末尾（通过未指定的方式确定）并解码字节流中的最后一个NAL单元：

1. 当位流中的下一个四字节序列形成四字节序列0x00000001时，提取并丢弃字节流中的下一个字节（即zero_byte语法元素），并将字节流中的当前位置设置为该丢弃字节之后的字节的位置。

2. 提取并丢弃字节流中的下一个三字节序列（即start_code_prefix_one_3bytes），并将字节流中的当前位置设置为该三字节序列之后的字节的位置。

3. NumBytesInNALunit设置为从字节流中的当前位置开始的字节数，直到并包括任何以下位置之前的最后一个字节：
- 随后的字节对齐的三字节序列等于0x000000，
- 随后的字节对齐的三字节序列等于0x000001，
- 字节流的末尾，通过未指定的方式确定。

4. 从位流中移除NumBytesInNALunit字节，字节流中的当前位置前进NumBytesInNALunit字节。此字节序列为nal_unit（NumBytesInNALunit），并使用NAL单元解码过程进行解码。

5. 当字节流中的当前位置不在字节流末尾（通过未指定的方式确定）并且字节流中的下一个字节不以三字节序列0x000001开头且字节流中的下一个字节不以四字节序列0x00000001开头时，解码器提取并丢弃每个trailing_zero_8bits语法元素，在字节流中每次向前移动一个字节，直到字节流中的当前位置使字节流中的下一个字节形成四字节序列0x00000001或遇到字节流的末尾（通过未指定的方式确定）。