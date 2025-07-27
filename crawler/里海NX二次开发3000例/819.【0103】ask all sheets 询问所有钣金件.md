### 【0103】ask all sheets 询问所有钣金件

#### 代码

```cpp
    static tag_t ask_next_sheet_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SHEET_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_sheets(tag_t part, tag_t **sheets)  
    {  
        tag_t  
            sheet = NULL_TAG;  
        uf_list_p_t  
            sheet_list;  
        UF_CALL(UF_MODL_create_list(&sheet_list));  
        while ((sheet = ask_next_sheet_body(part, sheet)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(sheet_list, sheet));  
        return (make_an_array(&sheet_list, sheets));  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于查询和遍历装配体中所有钣金体的函数。
>
> 主要功能包括：
>
> 1. ask_next_sheet_body函数：用于遍历装配体中的所有实体，并判断是否为钣金体。如果是，则返回该钣金体的tag。
> 2. allocate_memory函数：用于在NX中分配内存。
> 3. make_an_array函数：用于将NX的列表转换为C语言的数组。
> 4. ask_all_sheets函数：用于查询装配体中所有的钣金体，并将它们的tag放入一个数组返回。
>
> 这些函数通过NX的API进行交互，实现了对装配体中钣金体的查询和遍历功能，为后续的钣金体操作提供了基础。
>
