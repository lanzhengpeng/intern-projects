### 【2488】report split faces 报告拆分面

#### 代码

```cpp
    static void report_split_faces(tag_t face)  
    {  
        int  
            ii,  
            nfaces;  
        tag_t  
            *faces;  
        UF_CALL(UF_MODL_ask_split_faces(face, &nfaces, &faces));  
        if (nfaces > 1)  
        {  
            for (ii = 0; ii < nfaces; ii++ )  
                UF_CALL(UF_DISP_set_highlight(faces[ii], TRUE));  
            uc1601("Highlighted faces were once one face", TRUE);  
            for (ii = 0; ii < nfaces; ii++ )  
                UF_CALL(UF_DISP_set_highlight(faces[ii], FALSE));  
        }  
        else  
            uc1601("That face has not been split", 1);  
        if (nfaces > 0) UF_free(faces);  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的功能，主要实现了以下功能：
>
> 1. 查询面片是否被分割：通过调用UF_MODL_ask_split_faces函数，查询指定面片是否被分割，并获取分割后的面片数量和面片数组。
> 2. 高亮显示被分割的面片：如果面片被分割成多个，则通过UF_DISP_set_highlight函数高亮显示这些被分割的面片，并通过提示框显示“Highlighted faces were once one face”，表示这些面片曾经是一个面片。
> 3. 取消高亮显示：高亮显示几秒钟后，通过UF_DISP_set_highlight函数取消这些面片的高亮显示。
> 4. 释放内存：如果面片数组不为空，则通过UF_free函数释放分配的面片数组内存。
> 5. 提示信息：如果面片没有被分割，则通过提示框显示“That face has not been split”。
>
> 总体来说，这段代码通过NX的API实现了查询面片是否被分割，以及分割后的面片高亮显示和提示的功能，对于检查面片是否被分割非常有用。
>
