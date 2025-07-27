### 【2770】set highlight udo 设置高亮显示UDO

#### 代码

```cpp
    static void set_highlight_udo(tag_t udo, int flag)  
    {  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            *objects;  
        UF_CALL(UF_DISP_set_highlight(udo, flag));  
        UF_CALL(UF_UDOBJ_ask_owned_objects(udo, &n, &objects));  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_user_defined_object_type)  
                set_highlight_udo(objects[ii], flag);  
        }  
        if (n > 0) UF_free(objects);  
    }

```

#### 代码解析

> 这段代码是用于设置NX中用户定义对象(UDO)高亮显示的函数。
>
> 函数的主要功能包括：
>
> 1. 调用UF_DISP_set_highlight函数，设置传入的udo对象的高亮显示标志。
> 2. 调用UF_UDOBJ_ask_owned_objects函数，获取udo对象拥有的所有子对象，并保存到objects数组中。
> 3. 遍历objects数组，对每个子对象调用UF_DISP_set_highlight函数，设置高亮显示标志。
> 4. 获取子对象的类型和子类型，如果是用户定义对象类型，递归调用本函数，设置子对象的高亮显示。
> 5. 释放objects数组占用的内存。
>
> 通过递归调用，该函数可以设置udo对象及其所有子对象的高亮显示，实现层次化高亮显示。
>
