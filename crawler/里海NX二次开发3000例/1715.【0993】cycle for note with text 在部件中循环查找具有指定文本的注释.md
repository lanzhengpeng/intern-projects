### 【0993】cycle for note with text 在部件中循环查找具有指定文本的注释

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
    static tag_t cycle_for_note_with_text(tag_t part, char *text, tag_t after)  
    {  
        int  
            ii, jj,  
            n_text;  
        tag_t  
            a_note = after;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((a_note = ask_next_note(part, a_note)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(a_note, &n_text, &text_info));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                for (jj = 0; jj < text_info[ii].num_lines; jj++)  
                {  
                    if (strstr(text_info[ii].text[jj].string, text) != NULL)  
                        break;  
                }  
                if (jj < text_info[ii].num_lines) break;  
            }  
            UF_CALL(UF_DRF_free_text(n_text, &text_info));  
            if (ii < n_text) return a_note;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码用于在NX部件中查找包含特定文本的注释，并实现以下功能：
>
> 1. ask_next_note函数：遍历指定部件中的所有注释实体，并返回下一个注释实体的tag，若已遍历完则返回NULL_TAG。它通过调用UF_OBJ_cycle_objs_in_part遍历实体，并使用UF_OBJ_ask_type_and_subtype判断实体是否为注释类型。
> 2. cycle_for_note_with_text函数：在指定部件中循环查找包含特定文本的注释。它首先调用ask_next_note获取注释实体，然后调用UF_DRF_ask_draft_aid_text_info获取注释文本，最后使用strstr在文本中搜索特定字符串。如果找到包含字符串的注释，则返回该注释的tag；若遍历完所有注释后未找到，则返回NULL_TAG。
> 3. 该代码通过两个函数的配合，实现了在NX部件中高效查找包含特定文本的注释的功能，为NX的二次开发提供了便利。
> 4. 代码中使用了NX Open API的UF_CALL宏来简化API调用，并处理了必要的内存释放，避免内存泄漏。
> 5. 这段代码体现了NX二次开发中实体遍历、属性获取和字符串搜索等典型操作，对NX二次开发具有一定的参考价值。
>
