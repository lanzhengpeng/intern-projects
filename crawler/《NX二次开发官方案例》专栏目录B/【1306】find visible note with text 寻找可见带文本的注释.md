### 【1306】find visible note with text 寻找可见带文本的注释

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
    static int ask_all_visible_objects_of_type(int the_type, tag_t **objects)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG,  
            view;  
        uf_list_p_t  
            list;  
        if (UF_CALL(UF_VIEW_ask_work_view(&view))) return 0;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == the_type)  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }  
    static tag_t find_visible_note_with_text(char *text)  
    {  
        int  
            ii, jj, kk,  
            n,  
            n_text;  
        tag_t  
            a_note = NULL_TAG,  
            *notes;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        n = ask_all_visible_objects_of_type(UF_drafting_entity_type, &notes);  
        for (kk = 0; kk < n; kk++)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(notes[kk], &n_text, &text_info));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                for (jj = 0; jj < text_info[ii].num_lines; jj++)  
                    if (strstr(text_info[ii].text[jj].string, text) != NULL)  
                        break;  
                if (jj < text_info[ii].num_lines) break;  
            }  
            UF_CALL(UF_DRF_free_text(n_text, &text_info));  
            if (ii < n_text)  
            {  
                a_note = notes[kk];  
                break;  
            }  
        }  
        if (n > 0) UF_free(notes);  
        return a_note;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能是检索当前工作视图中所有可见的标注，并找出其中包含特定文本的标注。具体来说，代码执行以下步骤：
>
> 1. 获取当前工作视图：使用UF_VIEW_ask_work_view函数获取当前工作视图。
> 2. 创建列表：使用UF_MODL_create_list函数创建一个空列表，用于存放可见的标注。
> 3. 遍历可见标注：使用UF_VIEW_cycle_objects函数遍历当前工作视图中的所有可见标注，并将标注添加到列表中。
> 4. 提取标注文本：对列表中的每个标注，使用UF_DRF_ask_draft_aid_text_info函数提取标注的文本信息。
> 5. 查找特定文本：遍历标注的文本，使用strstr函数查找包含特定文本的标注。
> 6. 返回结果：找到包含特定文本的标注后，返回该标注的tag。如果未找到，则返回NULL_TAG。
> 7. 释放内存：最后，释放为标注分配的内存。
>
> 通过以上步骤，这段代码实现了在当前工作视图中快速检索特定文本的功能，提高了工作效率。
>
