翻译（ChatGPT-4o）

## 7

### 7.1 以表格形式指定的语法

语法表指定了所有允许的码流的语法的超集。在其他条款中，可以直接或间接地指定对语法的附加约束。

> 注 – 实际的解码器应该实现识别码流入口点的方法，并能够识别和处理不符合规范的码流。识别和处理错误以及其他此类情况的方法在此不作规定。

下表列出了用于描述语法的伪代码示例。当出现syntax_element时，它指定从码流中解析出一个语法元素，并在码流解析过程中将码流指针推进到该语法元素之后的下一个位置。

||Category|Descriptor|
|---|---|---|
|/\*语句可以是具有相关语法类别和描述的语法元素，也可以是用于指定语法元素存在、类型和数量的条件表达式，如以下两个示例所示。\*/|||
|**`syntax_element`**|3|ue(v)|
|`conditioning statement`|||
||||
|/\*用花括号括起来的一组语句是复合语句，并且在功能上被视为单一语句。\*/|||
|`{`|||
|`    statement`|||
|`    statement`|||
|`    ...`|||
|`}`|||
|/\*“while”结构指定对`condition`是否为真进行测试，如果为真，则指定重复执行一个语句（或复合语句），直到`condition`不再为真。\*/|||
|`while(condition)`|||
|`    statement`|||
||||
|/\*“do … while”结构指定先执行一次`statement`，然后测试`condition`是否为真，如果为真，则指定重复执行该语句，直到条件不再为真。\*/|||
|`do`|||
|`    statement`|||
|`while(condition)`|||
||||
|/\*“if … else”结构指定对条件是否为真进行测试，如果条件为真，则指定执行一个主要语句，否则，指定执行一个替代语句。如果不需要执行替代语句，则省略结构中的“else”部分和相关的替代语句。\*/|||
|`if(condition)`|||
|`    primary statement`|||
|`else`|||
|`    alternative statement`|||
||||
|/\*“for”结构指定首先执行一个初始语句，然后测试一个条件，如果条件为真，则指定重复执行一个主要语句，并在每次执行后执行一个后续语句，直到条件不再为真。\*/|||
|`for(initial statement; condition; subsequent statement)`|||
|`    primary statement`|||

### 7.2 语法函数、类别和描述符的规定

这里介绍的函数用于语法描述。这些函数假设存在一个比特流指针，该指针指示解码过程将从比特流中读取的下一个比特的位置。

#### byte_aligned() 函数
- 如果比特流中的当前位置在字节边界上，即比特流中的下一个比特是一个字节的第一个比特，则 byte_aligned() 的返回值为 TRUE。
- 否则，byte_aligned() 的返回值为 FALSE。

#### more_data_in_byte_stream() 函数
该函数仅在附录 B 指定的字节流 NAL 单元语法结构中使用，具体定义如下：
- 如果字节流中还有更多数据，more_data_in_byte_stream() 的返回值为 TRUE。
- 否则，more_data_in_byte_stream() 的返回值为 FALSE。

#### more_rbsp_data() 函数
- 如果 RBSP 中没有更多数据，more_rbsp_data() 的返回值为 FALSE。
- 否则，在 RBSP 中搜索最后一个（最低有效的、最右边的）比特等于 1 的位置，该比特是 rbsp_trailing_bits() 语法结构中的第一个比特（rbsp_stop_one_bit）。根据该比特的位置，以下情况适用：
  - 如果 RBSP 中在 rbsp_trailing_bits() 语法结构之前有更多数据，more_rbsp_data() 的返回值为 TRUE。
  - 否则，more_rbsp_data() 的返回值为 FALSE。

用于确定 RBSP 中是否有更多数据的方法由应用程序指定（或在使用字节流格式的应用程序中由附录 B 指定）。

#### more_rbsp_trailing_data() 函数
- 如果 RBSP 中有更多数据，more_rbsp_trailing_data() 的返回值为 TRUE。
- 否则，more_rbsp_trailing_data() 的返回值为 FALSE。

#### next_bits(n) 函数
该函数在不推进比特流指针的情况下提供比特流中的下 n 个比特供比较使用。提供对比特流中下 n 个比特的查看，n 是其参数。在附录 B 指定的字节流中使用时，如果字节流中剩余的比特少于 n，比特流中返回值为 0。

#### read_bits(n) 函数
读取比特流中的下 n 个比特，并将比特流指针前进 n 个比特位置。当 n 等于 0 时，read_bits(n) 指定返回值等于 0，并且不前进比特流指针。

### 类别（C）
类别（在表中标记为 C）指定切片数据最多划分为三个切片数据分区。切片数据分区 A 包含所有类别 2 的语法元素。切片数据分区 B 包含所有类别 3 的语法元素。切片数据分区 C 包含所有类别 4 的语法元素。其他类别值的含义未作规定。对于某些语法元素，使用两个类别值（用竖线分隔）。在这些情况下，进一步在文本中指定应用的类别值。对于在其他语法结构中使用的语法结构，列出了包含的语法结构中的所有语法元素的类别，用竖线分隔。类别标记为“All”的语法元素或语法结构存在于包含该语法元素或语法结构的所有语法结构中。对于在其他语法结构中使用的语法结构，在包含类别标记为“All”的语法元素的语法结构的位置提供的数值类别被认为适用于类别“All”的语法元素。

## 8 解码过程

此过程的输出为当前图像的解码样本（有时称为变量CurrPic）。根据chroma_format_idc的值，当前图像的样本数组数量如下：

- 如果chroma_format_idc等于0，则当前图像由一个样本数组SL组成。
- 否则（chroma_format_idc不等于0），当前图像由三个样本数组SL、SCb、SCr组成。

本条款描述了解码过程，前提是已知第7条款中的语法元素和大写变量。该解码过程的规定是所有解码器应产生数值上相同的结果。任何产生与此处描述的过程相同结果的解码过程都符合本建议|国际标准的解码过程要求。

本条款中提到的每个图像都是一个完整的主要编码图像或部分主要编码图像。本条款中提到的每个片段都是主要编码图像的一个片段。本条款中提到的每个片段数据分区都是主要编码图像的一个片段数据分区。

根据separate_colour_plane_flag的值，解码过程的结构如下：

- 如果separate_colour_plane_flag等于0，则解码过程仅调用一次，并输出当前图像。
- 否则（separate_colour_plane_flag等于1），解码过程调用三次。解码过程的输入为具有相同colour_plane_id值的主要编码图像的所有NAL单元。具有特定colour_plane_id值的NAL单元的解码过程被指定为，如果比特流中仅存在具有该特定colour_plane_id值的单色彩格式的编码视频序列。三个解码过程的输出分别分配给当前图像的3个样本数组：colour_plane_id等于0的NAL单元分配给SL，colour_plane_id等于1的NAL单元分配给SCb，colour_plane_id等于2的NAL单元分配给SCr。

> **注**：当separate_colour_plane_flag等于1且chroma_format_idc等于3时，变量ChromaArrayType派生为0。在解码过程中，该变量的值被评估，结果与chroma_format_idc等于0的单色图像的操作相同。

解码过程概述如下：

1. NAL单元的解码在第8.1条款中规定。
2. 第8.2条款中的过程规定了在片层及以上使用语法元素的解码过程：
- 与图像顺序计数相关的变量和函数在第8.2.1条款中派生（仅需要为图像的一个片段调用）。
- 与宏块到片组映射相关的变量和函数在第8.2.2条款中派生（仅需要为图像的一个片段调用）。
- 使用片数据分区时，各种片数据分区的组合方法在第8.2.3条款中描述。
- 当当前图像的frame_num不等于PrevRefFrameNum且不等于(PrevRefFrameNum + 1) % MaxFrameNum时，在解码当前图像的任何片段之前，根据第8.2.5.2条款执行frame_num间隔的解码过程。
- 在每个P、SP或B片段的解码过程开始时，根据第8.2.4条款规定的参考图像列表构建过程进行调用，以派生参考图像列表0（RefPicList0），在解码B片段时，也派生参考图像列表1（RefPicList1）。
- 当当前图像是参考图像时，在解码完当前图像的所有片段之后，第8.2.5条款中规定的已解码参考图像标记过程将规定当前图像在后续解码图像的帧间预测解码过程中的使用方式。

3. 第8.3、8.4、8.5、8.6和8.7条款中的过程规定了在宏块层及以上使用语法元素的解码过程：

- 第8.3条款规定了I和SI宏块的帧内预测过程，除了I_PCM宏块，输出为帧内预测样本。对于I_PCM宏块，第8.3条款直接规定了图像构建过程。输出为去块效应滤波过程之前的构建样本。
- 第8.4条款规定了P和B宏块的帧间预测过程，输出为帧间预测样本。
- 第8.5条款规定了变换系数解码过程和去块效应滤波过程之前的图像构建过程。该过程派生了I宏块和B宏块，以及P片段中的P宏块的样本。输出为去块效应滤波过程之前的构建样本。
- 第8.6条款规定了SP片段中P宏块或SI宏块的解码过程。该过程派生了SP片段中的P宏块和SI宏块的样本。输出为去块效应滤波过程之前的构建样本。
- 第8.7条款规定了去块效应滤波过程，该过程处理去块效应滤波过程之前的构建样本，这些样本位于块和宏块的边缘。输出为解码样本。

### 8.1 NAL单元解码过程

此过程的输入为NAL单元。  
此过程的输出为封装在NAL单元内的RBSP语法结构。  
每个NAL单元的解码过程从NAL单元中提取RBSP语法结构，然后根据NAL单元中的RBSP语法结构执行指定的解码过程，如下所述：

- 第8.2条款描述了nal_unit_type等于1到5的NAL单元的解码过程。
- 第8.3条款描述了在nal_unit_type等于1、2和5的NAL单元中编码的宏块或部分宏块的解码过程。
- 第8.4条款描述了在nal_unit_type等于1和2的NAL单元中编码的宏块或部分宏块的解码过程。
- 第8.5条款描述了在nal_unit_type等于1和3到5的NAL单元中编码的宏块或部分宏块的解码过程。
- 第8.6条款描述了在nal_unit_type等于1和3到5的NAL单元中编码的宏块或部分宏块的解码过程。
- 第8.7条款描述了在nal_unit_type等于1到5的NAL单元中编码的宏块或部分宏块的解码过程。

nal_unit_type等于7和8的NAL单元分别包含序列参数集和图像参数集。图像参数集在其他NAL单元的解码过程中使用，通过参考每个图像的片头中的图像参数集来确定。序列参数集在其他NAL单元的解码过程中使用，通过参考每个序列的图像参数集中的序列参数集来确定。

对于nal_unit_type等于6、9、10、11和12的NAL单元，没有规定规范的解码过程。

### 8.2 片段解码过程

#### 8.2.1 图像顺序计数的解码过程

此过程的输出为TopFieldOrderCnt（如果适用）和BottomFieldOrderCnt（如果适用）。

图像顺序计数用于确定B片段解码中的参考图像初始排序（见8.2.4.2.3和8.2.4.2.4条款）、确定共位图像以在时间或空间直接模式中派生运动参数（见8.4.1.2.1条款）、表示帧或场之间的图像顺序差异以在时间直接模式中派生运动矢量（见8.4.1.2.3条款）、在B片段中进行隐式模式加权预测（见8.4.2.3.2条款），以及进行解码器一致性检查（见C.4条款）。

图像顺序计数信息对于每个帧、场（无论是从编码场解码还是作为解码帧的一部分解码）或互补场对派生如下：

- 每个编码帧关联两个图像顺序计数，分别称为其顶场和底场的TopFieldOrderCnt和BottomFieldOrderCnt。
- 每个编码场关联一个图像顺序计数，编码顶场为TopFieldOrderCnt，编码底场为BottomFieldOrderCnt。
- 每个互补场对关联两个图像顺序计数，分别为其编码顶场的TopFieldOrderCnt和编码底场的BottomFieldOrderCnt。

TopFieldOrderCnt和BottomFieldOrderCnt表示相应顶场或底场相对于前一个IDR图像或前一个参考图像的第一个输出场的图像顺序。在解码顺序中包括memory_management_control_operation等于5的图像。

TopFieldOrderCnt和BottomFieldOrderCnt通过调用8.2.1.1、8.2.1.2和8.2.1.3条款中分别针对图像顺序计数类型0、1和2的解码过程派生。当当前图像包括memory_management_control_operation等于5时，在解码当前图像之后，tempPicOrderCnt设置为PicOrderCnt(CurrPic)，当前图像的TopFieldOrderCnt（如果有）设置为TopFieldOrderCnt - tempPicOrderCnt，当前图像的BottomFieldOrderCnt（如果有）设置为BottomFieldOrderCnt - tempPicOrderCnt。

> 注释 1

> 当对包含memory_management_control_operation等于5的图像currPic的解码过程涉及到图像currPic的TopFieldOrderCnt（如果适用）或BottomFieldOrderCnt（如果适用）的值（包括引用函数PicOrderCnt()并以图像currPic作为参数，或者引用函数DiffPicOrderCnt()并以currPic作为参数之一），使用的是根据第8.2.1.1、8.2.1.2和8.2.1.3条款派生的图像currPic的TopFieldOrderCnt（如果适用）和BottomFieldOrderCnt（如果适用）的值。

> 当对图像的解码过程涉及到前一个包含memory_management_control_operation等于5的图像prevMmco5Pic的TopFieldOrderCnt（如果适用）或BottomFieldOrderCnt（如果适用）的值（包括通过函数PicOrderCnt()或DiffPicOrderCnt()引用），用于图像prevMmco5Pic的TopFieldOrderCnt（如果适用）和BottomFieldOrderCnt（如果适用）的值是经过上述段落中指定的修改后的值（即TopFieldOrderCnt和/或BottomFieldOrderCnt等于0）。

比特流不应包含导致以下情况的数据：对于编码的IDR帧，TopFieldOrderCnt和BottomFieldOrderCnt中的较小者不等于0；对于编码的IDR顶场，TopFieldOrderCnt不等于0；或者对于编码的IDR底场，BottomFieldOrderCnt不等于0。因此，对于编码的IDR帧的场，TopFieldOrderCnt和BottomFieldOrderCnt中至少有一个应等于0。

当当前图像不是IDR图像时，适用以下规定：

1. 考虑包含以下元素的列表变量listD：与图像列表关联的TopFieldOrderCnt和BottomFieldOrderCnt值，该图像列表包括以下所有内容：
    a. 列表中的第一个图像是以下任意类型的前一个图像：
        - IDR图像
        - 包含memory_management_control_operation等于5的图像
    b. 其他附加图像：
        - 如果pic_order_cnt_type等于0，则在解码顺序中在列表中的第一个图像之后的所有其他图像，这些图像不是根据第8.2.5.2条款中规定的frame_num间隙解码过程中推断出的“非存在”帧，并且在解码顺序中在当前图像之前或为当前图像。当pic_order_cnt_type等于0且当前图像不是根据第8.2.5.2条款中规定的frame_num间隙解码过程中推断出的“非存在”帧时，当前图像在调用解码的参考图像标记过程之前被包含在listD中。
        - 否则（pic_order_cnt_type不等于0），在解码顺序中在列表中的第一个图像之后的所有其他图像，并且在解码顺序中在当前图像之前或为当前图像。当pic_order_cnt_type不等于0时，当前图像在调用解码的参考图像标记过程之前被包含在listD中。

2. 考虑包含listD元素的列表变量listO，按升序排序。listO不应包含以下任何内容：
    - 在listO中不连续位置的帧或互补场对的TopFieldOrderCnt和BottomFieldOrderCnt对
    - 具有相同值的TopFieldOrderCnt
    - 具有相同值的BottomFieldOrderCnt
    - 具有与TopFieldOrderCnt相同值的BottomFieldOrderCnt，除非BottomFieldOrderCnt和TopFieldOrderCnt属于同一编码帧或互补场对