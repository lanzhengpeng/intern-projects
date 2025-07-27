### 【0168】ask current cset 询问当前组件集

#### 代码

```cpp
    static tag_t ask_current_cset(void)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, "CurrentComponents",  
            UF_component_set_type, FALSE, &cset));  
        return cset;  
    }

```

#### 代码解析

> 这段代码是用于NX（NX是西门子的一款CAD/CAM/CAE软件）的二次开发，其主要功能是获取当前组件集（current component set）。
>
> 具体来说，代码首先声明了两个tag_t类型的变量cset和part，分别用于存储组件集和显示部分的tag。然后，通过UF_OBJ_cycle_by_name_and_type函数遍历当前显示部分的组件，查找名称为"CurrentComponents"且类型为UF_component_set_type的组件集，并将其tag赋值给cset。最后，函数返回找到的组件集cset。
>
> 因此，这段代码的作用是查询当前活动组件集的tag，以便后续进行相关的操作。
>
