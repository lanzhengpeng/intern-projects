### 【0015】ask all curves 询问所有曲线

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
    static int ask_all_curves(tag_t part, tag_t **curves)  
    {  
        int  
            ii, jj,  
            n,  
            type[4] = { UF_line_type,  
                        UF_circle_type,  
                        UF_conic_type,  
                        UF_spline_type };  
        tag_t  
            *objects;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < 4; ii++)  
        {  
            n = ask_all_of_type(part, type[ii], &objects);  
            for (jj = 0; jj < n; jj++)  
                UF_CALL(UF_MODL_put_list_item(object_list, objects[jj]));  
            if (n > 0) UF_free(objects);  
        }  
        return make_an_array(&object_list, curves);  
    }

```

#### 代码解析

> 这段代码是用于NX(UG) CAD软件的二次开发，其主要功能是查询一个零件中的所有曲线并返回它们的标签数组。
>
> 代码中的主要函数包括：
>
> 1. allocate_memory()：用于分配指定大小的内存，并返回指向这块内存的指针。
> 2. make_an_array()：遍历一个链表，将链表中的元素复制到一个数组中，然后释放链表并返回数组长度。
> 3. ask_all_of_type()：查询零件中指定类型的所有对象，将它们添加到链表中，然后调用make_an_array()生成对象标签数组。
> 4. ask_all_curves()：分别查询零件中的线、圆、二次曲线和样条线，将它们添加到同一个链表中，然后调用make_an_array()生成曲线标签数组。
> 5. UF_OBJ_cycle_objs_in_part()：遍历零件中的对象。
> 6. UF_MODL_put_list_item()：将一个对象添加到链表中。
> 7. UF_MODL_create_list()：创建一个空链表。
> 8. UF_MODL_ask_list_count()：查询链表中的对象数量。
> 9. UF_MODL_delete_list()：删除一个链表。
> 10. UF_free()：释放内存。
>
> 总的来说，这段代码实现了查询零件中所有曲线标签数组的功能。
>
