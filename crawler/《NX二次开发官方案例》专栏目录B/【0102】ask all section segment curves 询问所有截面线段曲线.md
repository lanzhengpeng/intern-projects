### 【0102】ask all section segment curves 询问所有截面线段曲线

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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_section_segment_curves(tag_t part, tag_t **objects)  
    {  
        int  
            ii,  
            n_segs;  
        tag_t  
            object,  
            *sxsegs;  
        uf_list_p_t  
            object_list;  
        UF_DRF_object_t  
            *drf_object;  
        UF_DRAW_sxseg_info_t  
            sxseg_info;  
        n_segs = ask_all_of_type(part, UF_section_segment_type, &sxsegs);  
        if (n_segs == 0) return 0;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < n_segs; ii++)  
        {  
            UF_CALL(UF_DRAW_ask_sxline_sxseg(sxsegs[ii], &sxseg_info, &object,  
                &drf_object));  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
            UF_free(drf_object);  
        }  
        UF_free(sxsegs);  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段NX Open C++代码是用于查询NX模型中的截面线段信息，并获取每个截面线段对应的曲线。主要功能包括：
>
> 1. allocate_memory：用于分配指定大小的内存，并返回指向分配内存的指针。
> 2. make_an_array：接收一个链表，提取链表中的元素，并创建一个tag数组，最后释放链表。
> 3. ask_all_of_type：查询指定类型（如截面线段）的所有对象，并返回一个tag数组。
> 4. ask_all_section_segment_curves：查询模型中所有截面线段，并获取每个截面线段对应的曲线。它首先调用ask_all_of_type获取所有截面线段，然后循环遍历每个截面线段，使用UF_DRAW_ask_sxline_sxseg获取其对应的曲线，并最终返回一个曲线的tag数组。
>
> 整体而言，这段代码提供了查询模型中所有截面线段并获取其对应曲线的功能。它使用了NX Open提供的UF_MODL和UF_DRAW接口来实现对象查询和几何查询。
>
