### 【1247】extract faces 提取面

#### 代码

```cpp
    static void extract_faces(tag_t body)  
    {  
        int  
            face_count,  
            i,j,k,  
            irc;  
        tag_t  
            face,  
            sheet;  
        uf_list_p_t  
            face_list;  
        double  
            atol = { 1.0 },  
            dtol = { 0.1 },  
            rtol = { 0.1 };  
        UF_CALL(UF_MODL_ask_body_faces( body, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &face_count));  
        printf("Body %d contains %d faces.\n", body, face_count);  
        for (i = 0; i < face_count; i++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(face_list, i, &face));  
            k = i + 1;  
            printf("  Face %d  = %d ...\n", k, face);  
            for (j = 0; j < 3; j++)  
            {  
                UF_CALL(UF_MODL_extract_face(face, j, &sheet));  
                printf("    extract (%d) = %d\n", j, sheet);  
                if (sheet) UF_CALL( UF_OBJ_delete_object( sheet ));  
            }  
            FTN(uf5444)(&face, &dtol, &atol, &sheet, &irc);  
            UF_CALL( irc );  
            printf("    UF5444 = %d\n", sheet);  
            if (sheet) UF_CALL( UF_OBJ_delete_object( sheet ));  
            FTN(uf5465)(&face, &rtol, &sheet);  
            printf("    UF5465 = %d\n", sheet);  
            if (sheet) UF_CALL( UF_OBJ_delete_object( sheet ));  
            FTN(uf6509)(&body, &k, &rtol, &sheet);  
            printf("    UF6509 = %d\n", sheet);  
            if (sheet) UF_CALL( UF_OBJ_delete_object( sheet ));  
        }  
        UF_CALL( UF_MODL_delete_list( &face_list));  
    }

```

#### 代码解析

> 这段代码实现了在NX中提取实体(body)包含的面(face)信息的功能。具体步骤包括：
>
> 1. 调用UF_MODL_ask_body_faces函数，获取实体中面的列表(face_list)。
> 2. 获取面的数量(face_count)。
> 3. 遍历面列表，对于每个面(face)，执行以下操作：提取面的3个子面(sheet)，并打印子面标识。删除子面，避免重复提取。调用UF5444函数，基于公差提取面(face)的另一个版本(sheet)，并打印结果。删除提取的子面。调用UF5465函数，基于公差提取面(face)的另一个版本(sheet)，并打印结果。删除提取的子面。调用UF6509函数，基于公差提取实体(body)的第k个面(sheet)，并打印结果。删除提取的子面。
> 4. 提取面的3个子面(sheet)，并打印子面标识。
> 5. 删除子面，避免重复提取。
> 6. 调用UF5444函数，基于公差提取面(face)的另一个版本(sheet)，并打印结果。
> 7. 删除提取的子面。
> 8. 调用UF5465函数，基于公差提取面(face)的另一个版本(sheet)，并打印结果。
> 9. 删除提取的子面。
> 10. 调用UF6509函数，基于公差提取实体(body)的第k个面(sheet)，并打印结果。
> 11. 删除提取的子面。
> 12. 删除面列表，避免内存泄漏。
>
> 综合来看，这段代码通过遍历实体中的每个面，并调用不同的函数提取面，实现了在NX中提取实体面信息的功能。
>
