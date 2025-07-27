### 【1069】display any parent 显示任何父组件

#### 代码

```cpp
    static void display_any_parent(tag_t component)  
    {  
        int  
            n;  
        tag_t  
            *parents;  
        n = UF_ASSEM_where_is_part_used(component, &parents);  
        if (n > 0)  
        {  
            UF_CALL(UF_PART_set_display_part(parents[0]));  
            UF_free(parents);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是设置组件的显示状态。
>
> 代码首先调用UF_ASSEM_where_is_part_used函数，获取组件component的使用情况，并存储在parents数组中。如果组件被使用，即parents数组的长度大于0，那么调用UF_PART_set_display_part函数，设置parents数组中的第一个组件的显示状态。最后，释放parents数组占用的内存。
>
> 通过这段代码，可以实现以下功能：
>
> 1. 获取组件的使用情况。
> 2. 如果组件被使用，设置其父组件的显示状态。
> 3. 释放内存，防止内存泄漏。
>
> 需要注意的是，此代码仅设置第一个父组件的显示状态，如果组件有多个父组件，需要遍历parents数组来设置所有父组件的显示状态。
>
