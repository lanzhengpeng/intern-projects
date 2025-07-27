### 【3093】cycle by name type and subtype 根据名称、类型和子类型循环遍历对象

#### 代码

```cpp
    static tag_t cycle_by_name_type_and_subtype(tag_t part, char *name, int type,  
        int subtype, tag_t object)  
    {  
        int  
            this_type,  
            this_subtype;  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype) return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open API中用于在NX部件中循环查找特定名称、类型和子类型的对象的函数。代码的主要功能是：
>
> 1. 定义了一个名为cycle_by_name_type_and_subtype的函数，它接受5个参数：part: 要在其中搜索对象的NX部件。name: 要查找的对象的名称。type: 要查找的对象的类型。subtype: 要查找的对象的子类型。object: 用于循环查找的对象的句柄，初始值为NULL_TAG。
> 2. part: 要在其中搜索对象的NX部件。
> 3. name: 要查找的对象的名称。
> 4. type: 要查找的对象的类型。
> 5. subtype: 要查找的对象的子类型。
> 6. object: 用于循环查找的对象的句柄，初始值为NULL_TAG。
> 7. 使用UF_CALL宏调用UF_OBJ_cycle_by_name_and_type函数，该函数在部件中按名称和类型循环查找对象。它返回找到的对象的句柄，如果没有找到则返回NULL_TAG。
> 8. 在循环中，使用UF_CALL宏调用UF_OBJ_ask_type_and_subtype函数，获取当前对象的类型和子类型。
> 9. 如果当前对象的子类型等于要查找的子类型，则返回当前对象。
> 10. 如果没有找到符合条件的目标对象，则返回NULL_TAG。
> 11. 该函数的主要作用是在NX部件中按名称、类型和子类型循环查找对象，返回找到的第一个匹配对象。
> 12. 代码中使用了UF_CALL宏来调用NX Open API函数，并处理了返回码。这保证了API调用的正确性。
> 13. 函数名称采用了NX Open API函数的命名风格，具有很高的可读性。
> 14. 整体来说，这段代码实现了在NX部件中按名称、类型和子类型查找对象的功能，具有一定的实用价值。
>
