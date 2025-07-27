### 【1475】is imported view component is_imported_view_component 表示“是否为导入视图组件”

#### 代码

```cpp
    // NOTE:  This is obsolete as of NX5.0.  See UF_DRAW_is_drafting_component.  
    static logical is_imported_view_component(tag_t component)  
    {  
        logical  
            is_imported = FALSE;  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            *children;  
        UF_CALL(UF_SO_ask_children(component, UF_SO_ASK_ALL_CHILDREN, &n,  
            &children));  
        for (ii = 0; ii < n; ii++)  
        {  
            if (UF_OBJ_ask_status(children[ii]) != UF_OBJ_ALIVE) continue;  
            UF_CALL(UF_OBJ_ask_type_and_subtype(children[ii], &type, &subtype));  
            if ((type == UF_view_type) && (subtype == UF_view_imported_subtype))  
            {  
                is_imported = TRUE;  
                break;  
            }  
        }  
        if (n > 0) UF_free(children);  
        return is_imported;  
    }

```

#### 代码解析

> 这段代码实现了判断NX组件是否为导入视图组件的功能。
>
> 主要步骤包括：
>
> 1. 定义了函数is_imported_view_component，它接收一个组件tag作为参数，并返回一个布尔值，表示该组件是否为导入视图组件。
> 2. 初始化一个布尔变量is_imported，用于标记是否找到导入视图组件，初始值为FALSE。
> 3. 调用UF_SO_ask_children函数，获取组件的所有子组件，并存储在children数组中。
> 4. 遍历children数组，检查每个子组件的状态。如果组件状态为UF_OBJ_ALIVE，则进一步检查其类型和子类型。
> 5. 如果子组件的类型为UF_view_type，并且子类型为UF_view_imported_subtype，则表示找到了导入视图组件，将is_imported变量置为TRUE，并跳出循环。
> 6. 循环结束后，释放children数组占用的内存。
> 7. 函数返回is_imported变量的值，表示是否找到导入视图组件。
>
> 总的来说，这段代码通过遍历组件的子组件，并判断子组件的类型和子类型，实现了判断组件是否为导入视图组件的功能。
>
