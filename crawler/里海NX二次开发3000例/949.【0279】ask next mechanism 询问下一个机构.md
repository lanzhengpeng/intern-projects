### 【0279】ask next mechanism 询问下一个机构

#### 代码

```cpp
    static tag_t ask_next_mechanism(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_mdm_mechanism_type, &object));  
        return (object);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，其作用是在NX中遍历指定部件(part)中的所有机构(mechanism)。
>
> 代码中使用了UF_OBJ_cycle_objs_in_part函数，该函数用于遍历部件中的所有指定类型的对象。在这个例子中，我们指定了UF_mdm_mechanism_type，这意味着我们要遍历部件中的所有机构对象。
>
> 函数的第一个参数是部件对象(part)，表示我们要遍历的部件。第二个参数是对象类型，这里我们指定为UF_mdm_mechanism_type，表示我们要遍历机构对象。第三个参数是对象指针，指向要遍历的第一个对象，这里是object。
>
> 函数返回值是tag_t类型，表示遍历到的下一个对象。如果遍历完成，返回值将是NULL_TAG。
>
> 因此，这段代码的功能是在NX部件中遍历所有机构对象，并返回遍历到的下一个机构对象。如果遍历完成，则返回NULL_TAG。
>
