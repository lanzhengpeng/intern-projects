### 【0328】ask parts list tag pre nx2 询问NX2零件清单标签

#### 代码

```cpp
    static tag_t ask_parts_list_tag(tag_t part, tag_t plist)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_report_entity_type,  
            &plist)) && (plist != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(plist, &type, &subtype));  
            if (subtype == UF_parts_list_subtype) return plist;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能是在NX部件中遍历对象，并查找指定的零件列表(part list)。
>
> 主要步骤包括：
>
> 1. 定义了一个名为ask_parts_list_tag的静态函数，接收部件(part)和对象列表(plist)两个参数，并返回找到的零件列表标签(tag)。
> 2. 在while循环中，调用UF_OBJ_cycle_objs_in_part函数遍历部件中的对象。此函数会调用回调函数UF_report_entity_type来获取对象的类型。
> 3. 在循环中，使用UF_OBJ_ask_type_and_subtype函数来获取对象类型和子类型。如果子类型是UF_parts_list_subtype(即零件列表)，则返回该对象列表标签。
> 4. 如果没有找到零件列表，则返回NULL_TAG。
> 5. 函数利用了NX的UF_OBJ模块提供的API，通过遍历和类型判断，实现了在NX部件中查找指定子类型的对象列表的功能。
> 6. 函数以静态形式定义，可以在不同的地方调用，提高了代码的复用性。
> 7. 函数名和参数名使用下划线命名法，提高了代码的可读性。
> 8. 通过UF_CALL宏调用NX API，增强了代码的健壮性。
>
> 总体来说，这段代码实现了在NX部件中查找特定子类型的对象列表的功能，采用了NX的API进行对象遍历和类型判断，并且代码结构清晰，命名规范，具有一定的可读性和复用性。
>
