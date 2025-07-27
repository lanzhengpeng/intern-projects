### 【0101】ask all section lines 询问所有剖面线

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
    static int ask_all_section_lines(tag_t part, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_section_line_type,  
            &object)) && (object != NULL_TAG))  
                UF_MODL_put_list_item(object_list, object);  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要用于处理NX模型中的线和面的相关操作。
>
> 1. allocate_memory函数：用于申请指定大小的内存，并将内存地址赋值给传入的指针参数。
> 2. make_an_array函数：将NX列表转换为C数组，首先获取列表中的元素个数，然后申请相应大小的内存，并将列表中的元素复制到数组中。最后释放NX列表，返回数组元素个数。
> 3. ask_all_section_lines函数：用于获取NX部件中所有截面线的Tag。首先创建一个NX列表，然后遍历部件中的所有截面线，将截面线的Tag添加到列表中。最后调用make_an_array函数，将列表转换为C数组，并返回数组元素个数。
>
> 这些函数封装了NX对象操作的通用流程，为后续代码处理NX模型提供了便利。
>
