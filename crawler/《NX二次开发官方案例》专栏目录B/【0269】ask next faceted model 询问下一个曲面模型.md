### 【0269】ask next faceted model 询问下一个曲面模型

#### 代码

```cpp
    static tag_t ask_next_faceted_model(tag_t part, tag_t model)  
    {  
        while  
        (  
            !UF_CALL(UF_OBJ_cycle_objs_in_part(part,UF_faceted_model_type,&model))  
            && (model != NULL_TAG)  
        )  
            if (!UF_ASSEM_is_occurrence(model)) return model;  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，其主要功能是遍历NX部件中的所有面模型，并返回下一个面模型。
>
> 具体来说：
>
> 1. ask_next_faceted_model是一个静态函数，接受两个参数：part表示NX部件，model表示当前的面模型。
> 2. 使用UF_OBJ_cycle_objs_in_part函数遍历部件中的所有面模型。该函数的回调函数为UF_faceted_model_type，表示只遍历面模型类型的对象。
> 3. 循环条件是遍历没有结束且当前面模型不为空。
> 4. 在循环体内，使用UF_ASSEM_is_occurrence判断当前面模型是否为实例。如果是实例，则返回当前面模型。
> 5. 如果遍历结束或者当前面模型为空，则返回空标签。
> 6. 综上，该函数的功能是遍历部件中的所有面模型，并返回下一个不是实例的面模型。如果所有面模型都是实例，则返回空标签。
>
> 这段代码封装了NX部件中面模型的遍历逻辑，为后续处理提供了便利。
>
