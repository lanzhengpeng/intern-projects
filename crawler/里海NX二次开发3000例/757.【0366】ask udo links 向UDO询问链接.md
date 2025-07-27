### 【0366】ask udo links 向UDO询问链接

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

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，用于在NX中处理用户定义对象(UDO)。代码主要包含两个静态方法：
>
> 1. allocate_memory方法：该静态方法用于在NX中分配内存。它接受要分配的字节数和一个指向void指针的指针，用于指向分配的内存。方法内部调用了UF_allocate_memory函数，并返回错误码。
> 2. ask_udo_links方法：该静态方法用于获取一个UDO的所有链接。它接受一个UDO的tag和一个指向UF_UDOBJ_link_t指针数组的指针。方法内部首先调用UF_UDOBJ_ask_udo_data函数获取UDO的所有数据，然后计算链接数量并分配内存，接着遍历链接数量，将每个链接的定义赋值给links数组，最后释放UDO数据内存。方法返回链接数量。
>
> 总体来说，这段代码通过静态方法封装了NX API，提供了在NX中分配内存和获取UDO链接的便捷方式。
>
