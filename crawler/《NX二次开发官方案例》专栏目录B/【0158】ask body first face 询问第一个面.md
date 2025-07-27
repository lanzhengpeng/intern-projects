### 【0158】ask body first face 询问第一个面

#### 代码

```cpp
    static tag_t ask_body_first_face(tag_t body)  
    {  
        tag_t  
            face;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于获取NX部件中的第一个面。
>
> 主要功能包括：
>
> 1. 获取指定部件的所有面，并存储在面列表中。
> 2. 从面列表中获取第一个面，并返回其tag。
> 3. 删除面列表，释放内存。
>
> 具体步骤包括：
>
> 1. 使用UF_MODL_ask_body_faces()函数获取指定部件的所有面，并将面存储在面列表中。
> 2. 使用UF_MODL_ask_list_item()函数从面列表中获取第一个面，并返回其tag。
> 3. 使用UF_MODL_delete_list()函数删除面列表，释放内存。
> 4. 最后返回获取到的第一个面的tag。
>
> 该函数的参数为指定部件的body tag，返回值为第一个面的tag。该函数通过NX Open C++ API实现，可以方便地在NX Open C++应用程序中使用。
>
