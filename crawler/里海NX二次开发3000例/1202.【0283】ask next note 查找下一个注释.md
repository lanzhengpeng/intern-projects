### 【0283】ask next note 查找下一个注释

#### 代码

```cpp
    static tag_t ask_next_note(tag_t part, tag_t a_note)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && (a_note != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
            if (subtype == UF_draft_note_subtype) return a_note;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++函数，用于在NX部件中遍历具有特定类型的对象，并返回下一个草稿笔记对象。
>
> 主要功能包括：
>
> 1. 定义了一个静态函数ask_next_note，接收两个输入参数：part表示要遍历的NX部件，a_note表示当前遍历到的对象标签。
> 2. 使用UF_OBJ_cycle_objs_in_part函数在部件part中遍历具有UF_drafting_entity_type类型的对象。当遍历到的对象不是NULL_TAG时，进入循环。
> 3. 在循环中，使用UF_OBJ_ask_type_and_subtype函数获取当前遍历对象的类型和子类型。
> 4. 判断当前对象的子类型是否为UF_draft_note_subtype，如果是，则返回当前对象标签。
> 5. 如果遍历结束，没有找到下一个草稿笔记对象，则返回NULL_TAG。
> 6. 该函数可用于在NX部件中遍历所有草稿笔记对象，每次调用返回下一个对象。
> 7. 使用了NX Open C++的UF对象函数，实现了NX对象类型和子类型的判断，以及部件中对象的遍历。
> 8. 该函数体现了NX二次开发中对象遍历和类型判断的常见用法。
>
