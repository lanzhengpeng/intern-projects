### 【0364】ask udo doubles 询问UDO双精度浮点数

#### 代码

```cpp
    static int ask_udo_doubles(tag_t udo, double **udo_doubles)  
    {  
        int  
            n = 0;  
        UF_UDOBJ_all_data_t  
            data;  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        n = data.num_doubles;  
        if ((n > 0) && !UF_CALL(allocate_memory(n * sizeof(double), udo_doubles)))  
            for (ii = 0; ii < n; ii++)  
                (*udo_doubles)[ii] = data.doubles[ii];  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        return n;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发中获取用户定义对象（UDO）中双精度浮点数属性的函数。主要功能包括：
>
> 1. 定义了一个静态函数ask_udo_doubles，用于获取传入的UDO对象中的双精度浮点数数组。
> 2. 使用UF_UDOBJ_ask_udo_data函数来获取UDO对象的数据，包括其中的双精度浮点数。
> 3. 计算双精度浮点数的个数，并动态分配内存来存储这些双精度浮点数。
> 4. 如果双精度浮点数个数大于0，则将获取到的双精度浮点数复制到新分配的内存中。
> 5. 释放使用UF_UDOBJ_free_udo_data函数释放UDO数据结构占用的内存。
> 6. 返回双精度浮点数的个数。
> 7. 使用UF_CALL宏来简化UF函数的调用，并处理错误。
>
> 该函数实现了从NX UDO对象中获取双精度浮点数属性的功能，并具有内存分配和释放等安全处理。
>
