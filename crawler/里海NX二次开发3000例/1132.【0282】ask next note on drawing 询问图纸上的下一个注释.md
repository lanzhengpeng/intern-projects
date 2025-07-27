### 【0282】ask next note on drawing 询问图纸上的下一个注释

#### 代码

```cpp
    static tag_t ask_next_note_on_drawing(tag_t part, char *dwg_name, tag_t a_note)  
    {  
        int  
            subtype,  
            status,  
            type;  
        char  
            drawing_view[MAX_ENTITY_NAME_SIZE+3],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        sprintf(drawing_view, "%s@0", dwg_name);  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && (a_note != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
            if (subtype == UF_draft_note_subtype)  
            {  
                uc6409(a_note, &status, view_name);  
                if (!strcmp(drawing_view, view_name)) return a_note;  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD软件中遍历零件对象，并找到特定视图下的下一个注释对象。
>
> 主要功能包括：
>
> 1. 遍历零件对象：使用UF_OBJ_cycle_objs_in_part()函数在指定零件对象中遍历所有对象，直到返回的下一个对象为空，即遍历结束。
> 2. 判断对象类型：使用UF_OBJ_ask_type_and_subtype()函数获取每个遍历到的对象的类型和子类型。如果子类型为UF_draft_note_subtype，即注释类型，则执行下一步。
> 3. 获取注释视图名称：使用自定义函数uc6409()获取注释对象的视图名称。
> 4. 判断视图名称：如果注释对象的视图名称与指定的视图名称相同，则返回该注释对象。
> 5. 返回结果：遍历结束后，如果未找到满足条件的注释对象，则返回空标签(NULL_TAG)。
>
> 整体而言，这段代码实现了在NX CAD软件中按视图名称查找下一个注释对象的功能。
>
