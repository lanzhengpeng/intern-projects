### 【0165】ask cset tag 查询组件集标签

#### 代码

```cpp
    static tag_t ask_cset_tag(char *cset_name)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, cset_name,   
            UF_component_set_type, FALSE, &cset));  
        return cset;  
    }

```

#### 代码解析

> 这段代码是用于在NX Open API中进行组件集操作的函数。其主要功能是根据组件集名称获取组件集的标签(tag)。具体步骤如下：
>
> 1. 定义函数ask_cset_tag，接受一个字符串参数cset_name，表示要查询的组件集名称。
> 2. 初始化组件集标签cset为NULL_TAG。
> 3. 获取当前显示部件part的标签，通过UF_PART_ask_display_part()函数。
> 4. 使用UF_OBJ_cycle_by_name_and_type()函数，根据组件集名称cset_name和组件集类型UF_component_set_type，在当前显示部件part下循环查找组件集。查找成功后，将组件集的标签赋值给cset。
> 5. 返回找到的组件集标签cset。
>
> 这个函数的主要作用是根据组件集名称，在当前显示部件下查询对应的组件集，并返回其标签，以便后续进行其他操作，如获取组件集内的组件列表等。
>
