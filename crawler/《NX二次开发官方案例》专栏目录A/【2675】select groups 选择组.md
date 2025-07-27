### 【2675】select groups 选择组

#### 代码

```cpp
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
    /* 里海 */  
    static int mask_for_groups(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_group_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_groups(char *prompt, tag_t **groups)  
    {  
        int  
            cnt,  
            err,  
            ii,  
            n = 0,  
            resp,  
            subtype,  
            type;  
        tag_t  
            *objs;  
        uf_list_p_t  
            grp_list;  
        UF_CALL(UF_UI_select_with_class_dialog("Select groups", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_groups, NULL, &resp, &cnt, &objs));  
        if (cnt > 0)  
        {  
            UF_CALL(UF_MODL_create_list(&grp_list));  
            for (ii = 0; ii < cnt; ii++)  
            {  
                UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
                UF_CALL(UF_OBJ_ask_type_and_subtype(objs[ii], &type, &subtype));  
                if (type == UF_group_type)  
                    UF_CALL(UF_MODL_put_list_item(grp_list, objs[ii]));  
            }  
            UF_free(objs);  
            return make_an_array(&grp_list, groups);  
        }  
        else return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包含以下几个部分：
>
> 1. 内存分配函数allocate_memory：该函数用于为对象数组分配内存空间。它接受要分配的内存字节数和指向分配内存地址的指针，并调用UF_allocate_memory函数进行内存分配，返回分配结果。
> 2. 创建对象数组函数make_an_array：此函数用于创建对象数组。它接受对象链表和指向对象数组的指针，通过链表长度计算所需内存，然后调用allocate_memory分配内存，并将链表中的对象ID复制到数组中。最后删除链表并返回数组长度。
> 3. 选择过滤函数mask_for_groups：该函数用于设置选择过滤条件，仅选择组类型的对象。
> 4. 选择组对象函数select_groups：此函数用于通过对话框选择组对象。它设置选择过滤条件，然后调用UF_UI_select_with_class_dialog选择对象。如果选择了对象，则创建链表，将选择结果复制到链表中，并调用make_an_array创建对象数组。最后返回数组长度。
>
> 总体而言，这段代码实现了内存分配、对象数组创建、选择过滤和选择组对象的功能，主要用于NX的二次开发。
>
