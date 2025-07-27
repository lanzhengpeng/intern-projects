### 【1246】extract face 提取面

#### 代码

```cpp
    static void extract_face(tag_t face)  
    {  
        int  
            irc,  
            j;  
        tag_t  
            sheet;  
        double  
            atol = { 1.0 },  
            dtol = { 0.1 },  
            rtol = { 0.1 };  
        printf("Face = %d ...\n", face);  
        for (j = 0; j < 3; j++)  
        {  
            if (!UF_CALL(UF_MODL_extract_face(face, j, &sheet)))  
            {  
                printf("    extract (%d) = %d\n", j, sheet);  
                UF_CALL(UF_OBJ_delete_object( sheet ));  
            }  
        }  
        FTN(uf5444)(&face, &dtol, &atol, &sheet, &irc);  
        UF_CALL(irc);  
        printf("    UF5444 = %d\n", sheet);  
        if (sheet) UF_CALL( UF_OBJ_delete_object( sheet ));  
        FTN(uf5465)(&face, &rtol, &sheet);  
        printf("    UF5465 = %d\n", sheet);  
        if (sheet) UF_CALL( UF_OBJ_delete_object( sheet ));  
        FTN(uf6509)(&body, &k, &rtol, &sheet);  
        printf("    UF6509 = %d\n", sheet);  
        if (sheet) UF_CALL( UF_OBJ_delete_object( sheet ));  
    }

```

#### 代码解析

> 根据代码，该函数的主要功能是提取给定面(face)的边或顶点，并删除提取的边或顶点。具体步骤如下：
>
> 1. 遍历面的三个顶点，使用UF_MODL_extract_face接口提取每个顶点，并打印提取的顶点标签。之后，调用UF_OBJ_delete_object接口删除提取的顶点。
> 2. 调用UF5444接口提取面的边，并删除提取的边。打印提取的边标签。
> 3. 调用UF5465接口提取面的顶点，并打印提取的顶点标签。之后，删除提取的顶点。
> 4. 调用UF6509接口提取体的边，并打印提取的边标签。之后，删除提取的边。
> 5. 对于提取的边或顶点，都进行打印和删除操作。
>
> 总的来说，该函数通过不同的接口提取面的不同组成部分，并删除提取的部分，主要用于测试和验证NX提取边和顶点的接口功能。
>
