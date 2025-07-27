### 【0386】ask whats new 请问有哪些新事物？

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
    static int ask_whats_new(tag_t *old, int n_old, tag_t *now, int n_now,  
        tag_t **whats_new)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_now; ii++)  
        {  
            for (jj = 0; jj < n_old; jj++)  
                if (old[jj] == now[ii]) break;  
            if (jj == n_old) UF_CALL(UF_MODL_put_list_item(list, now[ii]));  
        }  
        return (make_an_array(&list, whats_new));  
    }

```

#### 代码解析

> 根据代码内容，这是一段NX二次开发代码，主要用于在NX环境中进行内存分配、列表操作和比较新旧列表的差异。
>
> 代码主要包括以下几个函数：
>
> 1. allocate_memory：用于分配指定大小的内存空间，并将内存地址存储在传入的指针中。
> 2. make_an_array：从NX的列表中获取元素，并创建一个数组来存储它们。这个函数首先获取列表的长度，然后分配足够的内存来创建一个数组，将列表元素复制到数组中，并删除列表。
> 3. ask_whats_new：用于比较两个列表（旧列表和新列表）的差异，找出新列表中新增的元素，并将这些新增元素存储在一个新列表中返回。
> 4. UF_MODL_ask_list_count：获取NX列表中元素的数量。
> 5. UF_MODL_put_list_item：向NX列表中添加一个元素。
> 6. UF_MODL_create_list：创建一个新的NX列表。
> 7. UF_MODL_delete_list：删除NX列表。
>
> 综合来看，这段代码通过NX提供的API实现了内存分配、列表操作等功能，以支持NX二次开发。
>
