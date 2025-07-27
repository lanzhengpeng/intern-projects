### 【1478】is object a component member 判断对象是否为组件成员

#### 代码

```cpp
    static logical is_object_a_component_member(tag_t component, tag_t object)  
    {  
        tag_t  
            obj = NULL_TAG;  
        while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(component, &obj)) &&  
            (obj != NULL_TAG)) if (obj == object) return TRUE;  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的一个示例，其功能是判断一个对象是否属于一个组件的成员。代码主要实现如下：
>
> 1. 定义了一个静态逻辑函数is_object_a_component_member，用于判断对象是否属于组件成员。
> 2. 该函数接受两个参数：组件的tag(component)和对象的tag(object)。
> 3. 使用一个循环遍历组件中的所有对象，直到组件中没有更多对象为止。
> 4. 在循环中，使用UF_ASSEM_cycle_objs_in_comp API来遍历组件中的对象，并将下一个对象tag赋值给变量obj。
> 5. 如果在遍历过程中找到了与输入参数object相同的对象，则返回TRUE，表示该对象是组件的成员。
> 6. 如果遍历完组件中的所有对象后没有找到与object相同的对象，则返回FALSE。
> 7. 通过这个函数，我们可以方便地判断一个对象是否属于某个组件的成员，从而实现组件与对象之间的关联关系检查。
>
> 这段代码利用了NX Open C++ API提供的组件遍历功能，实现了一个实用的组件成员判断功能，对NX的二次开发具有参考价值。
>
