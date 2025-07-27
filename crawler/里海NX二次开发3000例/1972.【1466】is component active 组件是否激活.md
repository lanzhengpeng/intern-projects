### 【1466】is component active 组件是否激活

#### 代码

```cpp
    static logical is_component_active(tag_t parent_occ, tag_t component)  
    {  
        logical  
            is_member;  
        tag_t  
            current_cset = ask_current_cset(),  
            member = NULL_TAG;  
        if (is_blanked(component)) return FALSE;  
        UF_CALL(UF_ASSEM_is_member_of_cset(current_cset, component, &is_member));  
        if (!is_member) return FALSE;  
        while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(parent_occ, &member)) &&  
            (member != NULL_TAG)) if (member == component) return TRUE;  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，主要实现了检查组件是否激活的功能。
>
> 代码的主要逻辑如下：
>
> 1. 定义了一个静态的函数is_component_active，用于判断一个组件是否激活。
> 2. 获取当前组件集(current_cset)，并调用UF_ASSEM_is_member_of_cset来判断传入的组件是否属于当前组件集。
> 3. 如果组件不属于当前组件集，则返回FALSE。
> 4. 如果组件属于当前组件集，则遍历父装配体的组件循环，判断传入的组件是否在循环中。
> 5. 如果在循环中找到了传入的组件，则返回TRUE，表示组件是激活的。
> 6. 如果遍历完循环也没有找到传入的组件，则返回FALSE。
> 7. 在遍历循环时，使用了UF_ASSEM_cycle_objs_in_comp来获取循环中的下一个组件，直到返回NULL_TAG为止。
> 8. 整个函数以static修饰，表示只能在当前文件中使用。
>
> 该函数主要用于判断一个组件是否激活，即是否属于当前组件集，并且存在于父装配体的组件循环中。
>
