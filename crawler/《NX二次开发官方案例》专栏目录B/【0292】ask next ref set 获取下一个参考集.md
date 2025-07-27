### 【0292】ask next ref set 获取下一个参考集

#### 代码

```cpp
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能是从指定的NX部件(part)中查询下一个参考集(reference set)。代码首先定义了一个函数ask_next_ref_set，该函数接受两个参数part和refset。part表示要查询的NX部件，而refset表示要查询的参考集，初始传入NULL_TAG。
>
> 函数体中，通过调用UF_CALL宏，循环调用UF_OBJ_cycle_objs_in_part函数，从part中遍历所有对象，直到遍历完所有对象或找到满足条件的参考集。
>
> 在循环中，每次调用UF_OBJ_ask_type_and_subtype函数，获取当前遍历到的对象的类型和子类型。然后判断子类型是否为参考设计子类型(UF_reference_design_subtype)、参考设计模型子类型(UF_reference_design_model_subtype)或参考设计轻量化子类型(UF_reference_design_lightweight_subtype)。如果是，则返回当前参考集refset。
>
> 如果遍历完所有对象都没有找到满足条件的参考集，则最终返回NULL_TAG。
>
> 总的来说，这段代码实现了在指定NX部件中查询下一个参考集的功能。
>
