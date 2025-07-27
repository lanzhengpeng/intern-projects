### 【0108】ask all udos 查询所有用户定义对象

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
    static tag_t ask_next_udo(tag_t part, UF_UDOBJ_class_t class_id, tag_t udo)  
    {  
        UF_CALL(UF_UDOBJ_cycle_udos_by_class(part, class_id, &udo));  
        return udo;  
    }  
    static int ask_all_udos(tag_t part, UF_UDOBJ_class_t class_id, tag_t **udos)  
    {  
        tag_t  
            udo = NULL_TAG;  
        uf_list_p_t  
            udo_list;  
        UF_CALL(UF_MODL_create_list(&udo_list));  
        while ((udo = ask_next_udo(part, class_id, udo)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(udo_list, udo));  
        return (make_an_array(&udo_list, udos));  
    }

```

#### 代码解析

> 这段NX Open API代码实现了以下几个功能：
>
> 1. allocate_memory函数：用于申请指定大小的内存，并将内存地址赋值给where指向的指针，返回申请内存的结果代码。
> 2. make_an_array函数：用于从object_list列表中提取所有元素的ID，存入一个tag_t类型的数组中，并返回数组元素个数。首先调用UF_MODL_ask_list_count函数获取列表中元素个数n，然后调用allocate_memory函数申请n个tag_t大小的内存，接着遍历列表，将每个元素的ID存入数组，最后删除列表。
> 3. ask_next_udo函数：用于获取指定零件part下，指定类别class_id的下一个用户定义对象UDO，并返回其ID。如果没有下一个UDO，则返回NULL_TAG。
> 4. ask_all_udos函数：用于获取指定零件part下，指定类别class_id的所有用户定义对象UDO，并将它们的ID存入一个tag_t类型的数组中，返回数组元素个数。首先创建一个列表，然后循环调用ask_next_udo函数获取每个UDO的ID，并添加到列表中。最后调用make_an_array函数将列表中的UDO ID存入数组，并返回数组元素个数。
>
> 综上所述，这段代码实现了从NX模型中查询特定类别用户定义对象的功能，并将它们的ID存入数组中返回。
>
