### 【1572】make bounding block 创建边界块

#### 代码

```cpp
    static tag_t make_bounding_block(tag_t object)  
    {  
        char  
            *len[3],  
            len_t[3][20];  
        double  
            box[6];  
        tag_t  
            block;  
        len[0] = &len_t[0][0];  
        len[1] = &len_t[1][0];  
        len[2] = &len_t[2][0];  
        sprintf(len_t[0], "%f", box[3] - box[0]);  
        sprintf(len_t[1], "%f", box[4] - box[1]);  
        sprintf(len_t[2], "%f", box[5] - box[2]);  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, box, len, &block));  
        UF_CALL(UF_MODL_ask_feat_body(block, &block));  
        UF_CALL(UF_OBJ_set_color(block, UF_OBJ_YELLOW));  
        return block;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是创建一个包围块，其步骤如下：
>
> 1. 声明变量：包括字符数组、double数组、tag_t等，用于存储长度、边界框坐标等信息。
> 2. 获取边界框尺寸：使用sprintf将边界框的长度转换成字符串，并存储在字符数组中。
> 3. 创建块：调用UF_MODL_create_block1函数，使用边界框坐标和长度字符串，创建一个块实体，并将其tag存储在block变量中。
> 4. 获取块体：调用UF_MODL_ask_feat_body，获取block的体实体，并存储在block变量中。
> 5. 设置颜色：调用UF_OBJ_set_color，将block的颜色设置为黄色。
> 6. 返回块实体tag：最后返回block变量，即新创建的包围块实体的tag。
>
> 综上所述，这段代码实现了在NX中根据边界框创建一个黄色的块实体，并将其返回的功能。
>
