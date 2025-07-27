### 【1474】is immediate child component is immediate child component翻译为“是直接子组件”

#### 代码

```cpp
    static logical is_immediate_child_component(tag_t component)  
    {  
        tag_t  
            inst = UF_ASSEM_ask_inst_of_part_occ(component),  
            parent = UF_ASSEM_ask_parent_of_instance(inst),  
            part = UF_ASSEM_ask_work_part();  
        if (parent == part) return TRUE;  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于判断组件是否为当前工作部件的立即子组件。其工作原理如下：
>
> 1. 通过UF_ASSEM_ask_inst_of_part_occ函数获取组件对应的实例。
> 2. 使用UF_ASSEM_ask_parent_of_instance函数获取该实例的父部件。
> 3. 通过UF_ASSEM_ask_work_part函数获取当前的工作部件。
> 4. 如果实例的父部件等于当前的工作部件，则认为组件是当前工作部件的立即子组件，返回TRUE。
> 5. 否则返回FALSE。
>
> 因此，这段代码的作用就是通过获取组件实例的父部件，并与当前工作部件进行比较，来判断组件是否为当前工作部件的立即子组件。
>
