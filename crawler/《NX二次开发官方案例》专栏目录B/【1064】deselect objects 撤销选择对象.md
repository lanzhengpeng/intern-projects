### 【1064】deselect objects 撤销选择对象

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
    static int ask_all_visible_objects(tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG,  
            view;  
        uf_list_p_t  
            list;  
        if (UF_CALL(UF_VIEW_ask_work_view(&view))) return 0;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        return make_an_array(&list, objects);  
    }  
    /* 里海 */  
    static int preselect_all_visible_objects(UF_UI_selection_p_t select,  
        void *user_data)  
    {  
        int  
            n;  
        tag_t  
            *objects;  
        n = ask_all_visible_objects(&objects);  
        if (n > 0)  
        {  
            UF_CALL(UF_UI_add_to_sel_list(select, n, objects, TRUE));  
            UF_free(objects);  
        }  
        return (UF_UI_SEL_SUCCESS);  
    }  
    static int deselect_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Deselect Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, preselect_all_visible_objects,  
            NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要功能是选择工作视图中所有可见对象，并将这些对象添加到选择列表中。具体步骤如下：
>
> 1. ask_all_visible_objects函数：遍历工作视图中的所有可见对象，并将这些对象添加到列表中。然后调用make_an_array函数，将列表转换为一个对象数组，最后删除列表。
> 2. make_an_array函数：计算对象数组的长度，并为其分配内存。然后遍历列表，将每个对象添加到数组中，最后删除列表。
> 3. preselect_all_visible_objects函数：调用ask_all_visible_objects函数获取所有可见对象，然后调用UF_UI_add_to_sel_list函数将这些对象添加到选择列表中。最后释放对象数组内存。
> 4. deselect_objects函数：弹出一个对话框，让用户选择要取消选择的类，然后调用preselect_all_visible_objects函数选择所有可见对象，最后调用UF_DISP_set_highlight函数将这些对象的高亮显示去掉。
>
> 总的来说，这段代码通过NX的UF API实现了选择工作视图中所有可见对象的功能，并将其添加到选择列表中。
>
