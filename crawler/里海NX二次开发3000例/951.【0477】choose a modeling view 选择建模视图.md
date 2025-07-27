### 【0477】choose a modeling view 选择建模视图

#### 代码

```cpp
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
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
    static int ask_all_modeling_views(tag_t part, tag_t **views)  
    {  
        tag_t  
            view = NULL_TAG;  
        uf_list_p_t  
            view_list;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_CALL(UF_MODL_create_list(&view_list));  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
        {  
            UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
            if (type == UF_VIEW_MODEL_TYPE)  
                UF_CALL(UF_MODL_put_list_item(view_list, view));  
        }  
        return (make_an_array(&view_list, views));  
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
                case 2:                     /* Cancel 译:Cancel */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:好的，我来翻译一下"picked one"这个短语。它的意思是“挑一个”。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_a_modeling_view(char *prompt)  
    {  
        int  
            ii,  
            n_views,  
            which;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            the_view,  
            *views;  
        char  
            **view_names;  
        n_views = ask_all_modeling_views(part, &views);  
        UF_CALL(allocate_string_array(n_views, MAX_ENTITY_NAME_SIZE+1, &view_names));  
        for (ii = 0; ii < n_views; ii++)  
            UF_CALL(UF_OBJ_ask_name(views[ii], view_names[ii]));  
        if ((which = choose_one_item(prompt, view_names, n_views)) == -1)  
            the_view = NULL_TAG;  
        else  
            the_view = views[which];  
        UF_free(views);  
        UF_free_string_array(n_views, view_names);  
        return the_view;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 获取当前显示部件的所有建模视图：通过遍历部件的视图，筛选出类型为UF_VIEW_MODEL_TYPE的建模视图。
> 2. 将视图列表转换为数组：调用make_an_array函数，将视图列表转换为视图数组。
> 3. 为视图数组分配字符串数组：调用allocate_string_array函数，为数组中的每个视图分配一个字符串，用于存储视图名称。
> 4. 提示用户选择一个建模视图：调用choose_one_item函数，提示用户从视图名称列表中选择一个视图，支持分页显示和翻页。
> 5. 获取用户选择的建模视图：调用choose_a_modeling_view函数，获取用户选择的视图Tag。
> 6. 释放分配的内存：释放视图数组、视图名称数组和视图列表所占用的内存。
> 7. 主要函数：ask_next_view：获取下一个视图Tagallocate_memory：分配内存make_an_array：将列表转换为数组ask_all_modeling_views：获取所有建模视图allocate_string_array：分配字符串数组choose_one_item：提示用户选择一个项目choose_a_modeling_view：获取用户选择的视图
> 8. ask_next_view：获取下一个视图Tag
> 9. allocate_memory：分配内存
> 10. make_an_array：将列表转换为数组
> 11. ask_all_modeling_views：获取所有建模视图
> 12. allocate_string_array：分配字符串数组
> 13. choose_one_item：提示用户选择一个项目
> 14. choose_a_modeling_view：获取用户选择的视图
>
> 该代码实现了在NX中获取当前显示部件的所有建模视图，并提示用户选择一个视图的功能。
>
