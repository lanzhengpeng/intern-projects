### 【3038】update model avoid udo callbacks 更新模型避免UDO回调

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_udo_links(tag_t udo, UF_UDOBJ_link_t **links)  
    {  
        int  
            ii,  
            n;  
        UF_UDOBJ_all_data_t  
            data;  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        n = data.num_links;  
        if (!UF_CALL(allocate_memory(n * sizeof(UF_UDOBJ_link_t), (void **)links)))  
            for (ii = 0; ii < n; ii++)  
                (*links)[ii] = data.link_defs[ii];  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        return n;  
    }  
    static update_model_avoid_udo_callbacks(tag_t udo)  
    {  
        int  
            ii,  
            n_links;  
        UF_UDOBJ_link_t  
            *links;  
        n_links = ask_udo_links(udo, &links);  
        for (ii = 0; ii < n_links; ii++)  
            UF_CALL(UF_UDOBJ_delete_link(udo, &links[ii]));  
        UF_CALL(UF_MODL_update());  
        if (n_links > 0)  
        {  
            UF_CALL(UF_UDOBJ_add_links(udo, n_links, links));  
            UF_free(links);  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API进行NX二次开发的一段示例代码。下面是代码的主要功能和逻辑：
>
> 1. allocate_memory函数：用于动态分配指定大小的内存，并返回指向该内存的指针。
> 2. ask_udo_links函数：获取用户定义对象(UDO)的所有链接信息。首先调用UF_UDOBJ_ask_udo_data获取UDO的数据，然后分配内存并复制链接信息，最后释放UDO数据内存。
> 3. update_model_avoid_udo_callbacks函数：用于在更新模型时避免调用UDO的回调函数。首先获取UDO的所有链接信息，然后删除这些链接，更新模型，最后重新添加链接。
> 4. 代码整体逻辑：通过动态内存分配、获取和释放链接信息，实现了一种在更新模型时避免调用UDO回调函数的方法。
> 5. 使用NX Open C++ API：代码使用了NX Open C++ API中的UF_UDOBJ相关的函数，实现了对NX UDO的查询、删除、添加等操作。
> 6. 函数封装：代码通过封装函数实现了代码的模块化和重用。
> 7. 错误处理：代码中使用了UF_CALL宏来处理函数调用可能出现的错误。
>
> 综上所述，这段代码展示了如何利用NX Open C++ API来操作NX中的用户定义对象，以及在模型更新时如何避免调用UDO的回调函数，具有很好的NX二次开发参考价值。
>
