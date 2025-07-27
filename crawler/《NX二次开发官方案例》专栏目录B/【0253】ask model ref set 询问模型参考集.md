### 【0253】ask model ref set 询问模型参考集

#### 代码

```cpp
    static tag_t ask_model_ref_set(tag_t part)  
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
            if (subtype == UF_reference_design_model_subtype) return refset;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其功能是遍历指定部件(part)中的所有引用集(reference set)，并找出类型为设计模型引用集的引用集。
>
> 主要思路如下：
>
> 1. 定义一个空引用集变量refset，用于存储找到的设计模型引用集。
> 2. 使用UF_OBJ_cycle_objs_in_part函数遍历指定部件中的所有引用集，直到遍历结束。
> 3. 在每次遍历中，使用UF_OBJ_ask_type_and_subtype函数获取当前引用集的类型和子类型。
> 4. 判断子类型是否为UF_reference_design_model_subtype，如果是，则表示找到设计模型引用集，返回该引用集。
> 5. 如果遍历结束仍未找到设计模型引用集，则返回空引用集。
>
> 通过以上步骤，这段代码实现了在NX部件中查找设计模型引用集的功能。
>
