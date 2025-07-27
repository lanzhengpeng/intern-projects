### 【1322】free linked exterior data 释放关联外部数据

#### 代码

```cpp
    static void free_linked_exterior_data(UF_MODL_linked_ext_p_t ext_data)  
    {  
        if (ext_data->num_bodies > 0)  
        {  
            UF_free(ext_data->bodies);  
            UF_free(ext_data->xforms);  
        }  
        if (ext_data->num_faces > 0)  
        {  
            UF_free(ext_data->faces);  
            UF_free(ext_data->xform_index);  
        }  
        ext_data->num_bodies = 0;  
        ext_data->bodies = NULL;  
        ext_data->xforms = NULL;  
        ext_data->num_faces = 0;  
        ext_data->faces = NULL;  
        ext_data->xform_index = NULL;  
    }

```

#### 代码解析

> 这段代码用于释放NX二次开发中链接到外部数据的内存。代码的主要功能包括：
>
> 1. 如果外部数据中包含多个体，则释放体和其变换矩阵的内存。
> 2. 如果外部数据中包含多个面，则释放面和面对应的变换矩阵索引的内存。
> 3. 将外部数据的体数量、面数量、体数组、面数组、变换矩阵数组和变换矩阵索引数组清零或设置为NULL。
> 4. 通过UF_free函数来释放内存，避免内存泄露。
>
> 这段代码的作用是清理外部数据结构，释放相关内存，防止内存泄露。在NX二次开发中，合理释放内存是非常重要的，可以避免程序运行时内存占用过大或程序异常终止。
>
