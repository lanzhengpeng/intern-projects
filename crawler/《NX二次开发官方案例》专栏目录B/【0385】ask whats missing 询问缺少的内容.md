### 【0385】ask whats missing 询问缺少的内容

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
    static int ask_whats_missing(tag_t *old, int n_old, tag_t *now, int n_now,  
        tag_t **missing)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_old; ii++)  
        {  
            for (jj = 0; jj < n_now; jj++)  
                if (now[jj] == old[ii]) break;  
            if (jj == n_now) UF_CALL(UF_MODL_put_list_item(list, old[ii]));  
        }  
        return (make_an_array(&list, missing));  
    }

```

#### 代码解析

> 这段代码是NX Open API开发的示例，主要实现了以下功能：
>
> 1. 内存分配函数：使用UF_allocate_memory()函数来分配指定大小的内存，并将分配的内存地址存储在指定的指针中。
> 2. 创建数组函数：该函数首先获取UF_MODL对象列表的长度，然后分配相应大小的内存空间以存储对象标签。通过遍历列表，将每个对象的标签存储在数组中，并删除列表。
> 3. 查询缺失对象函数：该函数首先创建一个空列表，然后遍历旧对象数组。对于每个旧对象，遍历新对象数组，若新数组中没有该旧对象，则将该旧对象添加到列表中。最后，调用创建数组函数，将列表中的对象标签存储在missing数组中。
>
> 总体来说，这段代码主要用于处理NX模型中对象标签的数组，包括分配内存、创建标签数组以及查询旧数组中在新数组中缺失的对象标签数组。这些功能对于模型比较和对象管理非常有用。
>
