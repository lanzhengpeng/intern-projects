### 【0299】ask next tabular note 询问下一个表格注释

#### 代码

```cpp
    static tag_t ask_next_tabular_note(tag_t part, tag_t tnote)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &tnote)) && (tnote != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tnote, &type, &subtype));  
            if (subtype == UF_tabular_note_subtype) return tnote;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能是在NX部件中遍历所有表格注释，并返回下一个表格注释对象。
>
> 具体来说，代码首先定义了一个名为ask_next_tabular_note的函数，该函数接收两个参数part和tnote，分别表示NX部件和当前表格注释对象。函数的返回值类型为tag_t，代表NX对象的标识。
>
> 在函数内部，通过UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type, &tnote))循环遍历部件中的所有表格注释对象。当循环结束且tnote不为空时，通过UF_OBJ_ask_type_and_subtype(tnote, &type, &subtype)获取当前表格注释的类型和子类型。如果子类型为UF_tabular_note_subtype，则表示该对象是表格注释，并返回该对象。如果循环结束且没有找到下一个表格注释对象，则返回NULL_TAG。
>
> 综上所述，该函数的主要功能是在NX部件中遍历所有表格注释，并返回下一个表格注释对象。通过循环遍历和类型判断实现。
>
