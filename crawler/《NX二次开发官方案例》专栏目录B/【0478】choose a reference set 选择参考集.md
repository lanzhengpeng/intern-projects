### 【0478】choose a reference set 选择参考集

#### 代码

```cpp
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
    #ifdef UF_reference_design_model_subtype  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
    #else  
            if (subtype == UF_reference_design_subtype) return refset;  
    #endif  
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
    static int ask_all_reference_sets(tag_t part, tag_t **refsets)  
    {  
        tag_t  
            refset = NULL_TAG;  
        uf_list_p_t  
            refset_list;  
        UF_CALL(UF_MODL_create_list(&refset_list));  
        while ((refset = ask_next_ref_set(part, refset)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(refset_list, refset));  
        return (make_an_array(&refset_list, refsets));  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:翻译为：选择了其中一个。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_a_reference_set(char *prompt)  
    {  
        int  
            ii,  
            n_refsets,  
            which;  
        tag_t  
            part = UF_ASSEM_ask_work_part(),  
            the_refset,  
            *refsets;  
        char  
            **refset_names;  
        n_refsets = ask_all_reference_sets(part, &refsets);  
        UF_CALL(allocate_string_array(n_refsets, MAX_ENTITY_NAME_SIZE+1,  
            &refset_names));  
        for (ii = 0; ii < n_refsets; ii++)  
            UF_CALL(UF_OBJ_ask_name(refsets[ii], refset_names[ii]));  
        if ((which = choose_one_item(prompt, refset_names, n_refsets)) == -1)  
            the_refset = NULL_TAG;  
        else  
            the_refset = refsets[which];  
        UF_free(refsets);  
        UF_free_string_array(n_refsets, refset_names);  
        return the_refset;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能是遍历指定部件的参考集，并允许用户选择其中一个参考集。
>
> 代码的主要流程如下：
>
> 1. ask_next_ref_set函数：该函数用于遍历指定部件的参考集，并返回下一个参考集的tag。它会跳过非设计参考集。
> 2. allocate_memory函数：用于分配内存。
> 3. make_an_array函数：将列表转换为数组。
> 4. ask_all_reference_sets函数：遍历指定部件的所有设计参考集，并返回一个包含所有参考集tag的数组。
> 5. allocate_string_array函数：分配字符串数组内存。
> 6. choose_one_item函数：提供选项让用户选择一个，支持分页。
> 7. choose_a_reference_set函数：遍历指定部件的所有设计参考集，获取参考集名称，让用户选择一个参考集，并返回用户选择的参考集tag。
>
> 总的来说，这段代码实现了遍历和选择参考集的功能，以便进行后续操作。
>
