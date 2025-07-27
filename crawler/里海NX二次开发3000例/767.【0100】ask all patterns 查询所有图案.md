### 【0100】ask all patterns 查询所有图案

#### 代码

```cpp
    static tag_t ask_next_pattern(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_pattern_type, &object));  
        return (object);  
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
    static int ask_all_patterns(tag_t part, tag_t **patterns)  
    {  
        tag_t  
            pattern = NULL_TAG;  
        uf_list_p_t  
            patt_list;  
        UF_CALL(UF_MODL_create_list(&patt_list));  
        while ((pattern = ask_next_pattern(part, pattern)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(patt_list, pattern));  
        return (make_an_array(&patt_list, patterns));  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能是获取NX模型中的所有pattern，并将其存储在一个tag_t类型的数组中。代码的主要步骤包括：
>
> 1. 创建对象列表：首先使用UF_MODL_create_list函数创建一个对象列表patt_list，用于存储后续获取的pattern对象。
> 2. 循环获取pattern对象：使用ask_next_pattern函数循环获取当前part中的所有pattern对象，直到获取完毕。这个函数内部调用了UF_OBJ_cycle_objs_in_part函数，用于遍历当前part中的所有pattern对象，并返回下一个pattern对象。
> 3. 将pattern对象添加到列表中：每获取一个pattern对象，就使用UF_MODL_put_list_item函数将其添加到patt_list列表中。
> 4. 将列表转换为数组：调用make_an_array函数，将patt_list列表转换为tag_t类型的数组。这个函数内部首先调用UF_MODL_ask_list_count获取列表的长度，然后分配相应大小的内存，遍历列表将每个pattern的tag赋值给数组对应位置的元素，最后释放列表。
> 5. 返回数组：make_an_array函数返回数组长度，并将数组首地址赋给传入的patterns指针参数。这样调用者就可以获取到包含所有pattern的数组。
>
> 总体来说，这段代码通过遍历part获取pattern对象，并将它们存储在数组中，为后续处理pattern提供了方便。
>
