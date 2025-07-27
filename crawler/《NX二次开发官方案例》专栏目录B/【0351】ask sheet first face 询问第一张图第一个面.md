### 【0351】ask sheet first face 询问第一张图第一个面

#### 代码

```cpp
    static tag_t ask_sheet_first_face(tag_t sheet)  
    {  
        tag_t  
            face;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(sheet, &face_list));  
        face = face_list->eid;  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，主要实现了以下功能：
>
> 1. 定义了一个名为ask_sheet_first_face的静态函数，用于获取NX模型中给定片体的第一个面。
> 2. 函数接受一个参数sheet，表示要查询的片体。
> 3. 使用UF_MODL_ask_body_faces函数获取片体的所有面，并将结果存储在face_list指针中。
> 4. 从face_list中获取第一个面的ID，并将其存储在变量face中。
> 5. 使用UF_MODL_delete_list函数删除face_list，以释放内存。
> 6. 函数返回获取到的第一个面的ID。
> 7. 整个函数使用UF_CALL宏来调用NX API，用于处理函数返回值，确保函数调用成功。
> 8. 该函数的目的是获取一个片体的第一个面，可以用于后续的几何处理。
>
> 总体来说，这段代码实现了在NX模型中获取指定片体的第一个面的功能，为后续的几何操作提供了基础。
>
