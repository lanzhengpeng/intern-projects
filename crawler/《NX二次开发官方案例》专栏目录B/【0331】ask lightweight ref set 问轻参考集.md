### 【0331】ask lightweight ref set 问轻参考集

#### 代码

```cpp
    static tag_t ask_lightweight_ref_set(tag_t part)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            refset = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if (subtype == UF_reference_design_lightweight_subtype) return refset;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是遍历指定部件中的所有引用集，并寻找类型为“轻量级设计引用”的引用集。具体来说：
>
> 1. 函数ask_lightweight_ref_set接受一个部件tag作为输入参数。
> 2. 使用UF_OBJ_cycle_objs_in_part函数遍历该部件中的所有对象。这个函数会调用回调函数UF_reference_set_type来检查每个对象的类型是否为引用集。
> 3. 如果找到一个引用集，就使用UF_OBJ_ask_type_and_subtype函数获取该引用集的类型和子类型。
> 4. 如果子类型是UF_reference_design_lightweight_subtype，则表示这是一个轻量级设计引用集，函数返回该引用集的tag。
> 5. 如果遍历完所有对象都没有找到轻量级设计引用集，则函数返回NULL_TAG。
>
> 总的来说，这段代码通过遍历部件中的对象，实现了查找指定类型引用集的功能。这对于需要在NX中操作特定类型引用集的场景非常有用。
>
