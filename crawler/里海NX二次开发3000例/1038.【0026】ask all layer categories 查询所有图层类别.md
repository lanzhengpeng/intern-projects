### 【0026】ask all layer categories 查询所有图层类别

#### 代码

```cpp
    static tag_t ask_next_layer_category(tag_t part, tag_t cat)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_layer_category_type, &cat));  
        return cat;  
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
    static int ask_all_layer_categories(tag_t part, tag_t **categories)  
    {  
        tag_t  
            category = NULL_TAG;  
        uf_list_p_t  
            cat_list;  
        UF_CALL(UF_MODL_create_list(&cat_list));  
        while ((category = ask_next_layer_category(part, category)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(cat_list, category));  
        return (make_an_array(&cat_list, categories));  
    }

```

#### 代码解析

> 根据代码内容，这是NX的二次开发代码，主要用于获取部件(part)的所有层类别(layer category)。
>
> 主要函数介绍如下：
>
> 1. ask_next_layer_category(part, cat)：用于获取指定部件的下一个层类别，并返回。如果cat为NULL_TAG，则获取第一个层类别；否则获取cat之后的层类别。
> 2. allocate_memory(nbytes, where)：用于分配指定大小的内存，并将内存地址存入where指向的指针。
> 3. make_an_array(object_list, objects)：将object_list中的所有元素转换为tag_t数组，并存入objects指向的指针，同时返回数组长度。
> 4. ask_all_layer_categories(part, categories)：用于获取指定部件的所有层类别，并将它们存入categories指向的tag_t数组，同时返回数组长度。
>
> 代码流程为：
>
> 1. 调用ask_all_layer_categories获取部件的所有层类别。
> 2. 在ask_all_layer_categories中，通过循环调用ask_next_layer_category获取每个层类别，并存入一个链表。
> 3. 调用make_an_array将链表转换为数组。
> 4. make_an_array通过调用allocate_memory分配内存，然后遍历链表，将元素存入数组。
> 5. 最后，ask_all_layer_categories返回数组长度，categories指向这个数组。
>
> 该代码实现了查询并获取NX部件的所有层类别的功能，为后续处理提供了基础数据。
>
