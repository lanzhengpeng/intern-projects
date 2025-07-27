### 【0304】ask next visible face 问下一个可见面

#### 代码

```cpp
    static tag_t ask_next_visible_face(tag_t face)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &face)) && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_face_subtype))  
                return face;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是一个用于NX Open API开发的函数，其主要功能是在NX中遍历可见对象，并返回下一个可见的实体面。
>
> 具体步骤如下：
>
> 1. 函数的名称是ask_next_visible_face，它接受一个NX实体的标签作为参数。
> 2. 使用一个while循环遍历当前视图中的可见对象。UF_VIEW_cycle_objects函数会返回下一个可见对象的标签，直到遍历结束。
> 3. 在循环中，通过UF_OBJ_ask_type_and_subtype函数获取遍历到的对象的类型和子类型。
> 4. 如果当前对象的类型是实体(UF_solid_type)且子类型是面(UF_solid_face_subtype)，则返回该面。
> 5. 如果没有找到下一个可见的面，则返回NULL_TAG。
> 6. 该函数的作用是在NX模型中遍历可见的实体面，并返回下一个可见的实体面。
> 7. 函数名称以ask_next_开头，表示这是一个查询函数，返回下一个可见面。
> 8. 使用UF_CALL宏来简化函数调用，避免手动检查错误码。
> 9. 整个函数实现了遍历和过滤的功能，返回符合条件的结果。
> 10. 该函数可以用于NX二次开发中的自动化任务，例如遍历模型的面进行后续操作。
>
