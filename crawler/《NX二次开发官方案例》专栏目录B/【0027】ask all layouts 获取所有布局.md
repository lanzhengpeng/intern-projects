### 【0027】ask all layouts 获取所有布局

#### 代码

```cpp
    static tag_t ask_next_layout(tag_t part, tag_t layout)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_layout_type, &layout));  
        return (layout);  
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
    static void sort_layouts_by_name(int n, tag_t *layouts)  
    {  
        int  
            ii;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            layout;  
        char  
            *layout_names;  
        UF_CALL(allocate_memory(n * sizeof(char) * (MAX_ENTITY_NAME_SIZE+1),  
            (void **)&layout_names));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_OBJ_ask_name(layouts[ii],  
                layout_names+(ii*(MAX_ENTITY_NAME_SIZE+1))));  
        qsort(layout_names, n, MAX_ENTITY_NAME_SIZE+1, strcmp);  
        for (ii = 0; ii < n; ii++)  
        {  
            layout = NULL_TAG;  
            UF_CALL(UF_OBJ_cycle_by_name_and_type(part,  
                layout_names+(ii*(MAX_ENTITY_NAME_SIZE+1)), UF_layout_type, FALSE, &layout));  
            layouts[ii] = layout;  
        }  
        UF_free(layout_names);  
    }  
    /*  This function returns the array of layouts in the same order they appear in the  
        interactive View-> Layout-> Open... Open Layout dialog  
    */  
    static int ask_all_layouts(tag_t part, tag_t **all_layouts)  
    {  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            *layouts,  
            layout = NULL_TAG;  
        char  
            layout_name[MAX_ENTITY_NAME_SIZE+1];  
        uf_list_p_t  
            canned,  
            normal,  
            layout_list;  
        UF_CALL(UF_MODL_create_list(&layout_list));  
        UF_CALL(UF_MODL_create_list(&canned));  
        UF_CALL(UF_MODL_create_list(&normal));  
        while ((layout = ask_next_layout(part, layout)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(layout, layout_name));  
            if (strchr(layout_name, '!') != layout_name)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(layout, &type, &subtype));  
                if (subtype == UF_normal_layout_subtype)  
                    UF_CALL(UF_MODL_put_list_item(normal, layout));  
                else  
                    UF_CALL(UF_MODL_put_list_item(canned, layout));  
            }  
        }  
        n = make_an_array(&canned, &layouts);  
        sort_layouts_by_name(n, layouts);  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_put_list_item(layout_list, layouts[ii]));  
        UF_free(layouts);  
        n = make_an_array(&normal, &layouts);  
        sort_layouts_by_name(n, layouts);  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_put_list_item(layout_list, layouts[ii]));  
        UF_free(layouts);  
        return (make_an_array(&layout_list, all_layouts));  
    }

```

#### 代码解析

> 这段代码主要用于获取NX中所有布局的列表，并按照其在“视图”->“布局”->“打开布局”对话框中的顺序返回。
>
> 具体步骤如下：
>
> 1. 定义了一个ask_next_layout函数，用于遍历指定部件part中的所有布局，并按顺序返回下一个布局。
> 2. 定义了一个allocate_memory函数，用于申请指定大小的内存空间。
> 3. 定义了一个make_an_array函数，用于将对象列表转换为对象数组，并释放列表内存。
> 4. 定义了一个sort_layouts_by_name函数，用于按布局名称排序布局数组。
> 5. 定义了ask_all_layouts函数，用于获取部件中的所有布局。首先创建布局列表，然后遍历所有布局，根据是否为模板布局，将布局添加到模板布局列表或普通布局列表中。接着对两个列表分别排序，并合并到布局列表中。最后将布局列表转换为布局数组并返回。
> 6. 整个流程通过UF_MODL_create_list、UF_MODL_put_list_item、UF_MODL_ask_list_count、UF_MODL_delete_list等函数来操作布局列表。
> 7. 通过UF_OBJ_cycle_objs_in_part、UF_OBJ_cycle_by_name_and_type等函数来遍历和获取布局。
> 8. 通过UF_OBJ_ask_name、UF_OBJ_ask_type_and_subtype等函数来获取布局的名称和类型信息。
> 9. 通过strcmp和qsort等函数进行布局名称的比较和排序。
> 10. 通过UF_free释放申请的内存。
>
> 总的来说，这段代码通过遍历部件中的所有布局，并按名称排序，最终返回一个与交互式布局对话框中顺序一致的布局数组。
>
