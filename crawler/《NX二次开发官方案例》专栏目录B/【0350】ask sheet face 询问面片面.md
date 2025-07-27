### 【0350】ask sheet face 询问面片面

#### 代码

```cpp
    static tag_t ask_sheet_face(tag_t sheet)  
    {  
        int  
            n;  
        tag_t  
            face = NULL_TAG;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(sheet, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &n));  
        if (n == 1) UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API进行NX二次开发的示例。代码的主要功能是查询NX中的面片(sheet)的关联面(face)。具体步骤包括：
>
> 1. 定义一个tag_t类型的变量sheet，表示要查询的面片。
> 2. 定义一个tag_t类型的变量face，用于存储查询到的面。初始值为NULL_TAG。
> 3. 定义一个uf_list_p_t类型的变量face_list，用于存储查询到的面列表。
> 4. 调用UF_MODL_ask_body_faces函数，传入面片的tag，获取面片关联的面的列表，存储在face_list中。
> 5. 调用UF_MODL_ask_list_count函数，传入face_list，获取列表中的面数量，存储在变量n中。
> 6. 判断n的值，如果n为1，表示面片只有一个面，则调用UF_MODL_ask_list_item函数，传入face_list和0，获取第一个面，存储在变量face中。
> 7. 调用UF_MODL_delete_list函数，传入face_list，释放面列表的内存。
> 8. 返回变量face的值。
>
> 通过以上步骤，该函数实现了查询NX中面片关联面的功能。
>
