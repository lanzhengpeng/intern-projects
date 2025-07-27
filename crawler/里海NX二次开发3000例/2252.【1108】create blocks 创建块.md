### 【1108】create blocks 创建块

#### 代码

```cpp
    static void create_blocks(void)  
    {  
        char  
            dims[3][25] = { "1.0", "1.0", "1.0" },  
            *sides[3];  
        double  
            corner[3];  
        int  
            i,  
            irc,  
            sel[2] = { 0, 0 };  
        tag_t  
            block;  
        for (i=0; i<3; i++) sides[i] = &dims[i][0];  
        while (prompt_block_parms(sides) &&  
              ((irc = uc1616("Select corner point", sel, 0, corner)) == 5))  
            UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner, sides, &block));  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是创建多个块。
>
> 1. 首先，定义了一个三维数组dims，用于存储块的尺寸，初始值都是1.0。同时定义了一个字符指针数组sides，用于指向dims数组中的字符串。
> 2. 接着定义了double类型的数组corner，用于存储块的角点坐标，以及一个int类型的数组sel，用于存储选择项。
> 3. 在while循环中，首先调用prompt_block_parms函数提示用户输入块的尺寸参数，并存储到sides指向的字符串中。然后调用uc1616函数提示用户选择块的角点，并将选择的角点坐标存储到corner数组中。
> 4. 如果用户输入了块的尺寸并且选择了块的角点，那么调用UF_MODL_create_block1函数根据给定的尺寸和角点坐标创建一个块，并将块的tag存储到block变量中。
> 5. 循环继续，直到用户不再输入块的尺寸或不再选择块的角点。
> 6. 最终，这段代码会根据用户的输入创建多个块。
>
> 总的来说，这段代码通过循环获取用户输入的块尺寸和角点坐标，并调用UF_MODL_create_block1函数创建多个块。
>
