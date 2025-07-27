### 【0261】ask next component 询问下一个组件

#### 代码

```cpp
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能是在NX中遍历指定组件的子组件，并返回第一个找到的零件组件。
>
> 代码中首先定义了一个静态函数ask_next_component，该函数接受两个参数：part表示父组件，comp表示子组件。函数使用一个while循环来遍历part的所有子组件。在循环中，使用UF_OBJ_cycle_objs_in_part函数来获取下一个子组件，并判断其是否为NULL_TAG，如果是，则退出循环。
>
> 在循环体内，使用UF_OBJ_ask_type_and_subtype函数来获取子组件的类型和子类型。如果子组件的子类型为UF_part_occurrence_subtype（即零件组件），则返回该组件的tag。
>
> 总的来说，这个函数的主要作用是遍历指定组件的子组件，并返回第一个找到的零件组件。这可以用于在NX中遍历装配结构，获取装配树中的零件组件。
>
