### 【1289】find note with text 寻找包含特定文字的注释

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
    static tag_t find_note_with_text(tag_t part, char *text)  
    {  
        int  
            ii, jj,  
            n_text;  
        tag_t  
            a_note = NULL_TAG;  
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

> 这段代码是NX Open C++ API编写的，主要实现了在NX部件中查找包含特定文本的注释的功能。
>
> 首先，ask_next_note函数用于遍历部件中的注释对象，并返回下一个注释对象。它接受部件标签和当前注释标签作为参数，通过UF_OBJ_cycle_objs_in_part函数遍历部件中的注释对象，直到找到下一个注释对象或者遍历完毕。然后，它通过UF_OBJ_ask_type_and_subtype函数检查注释对象的子类型，如果子类型是UF_draft_note_subtype（即注释子类型），则返回该注释对象。
>
> 接下来，find_note_with_text函数用于在部件中查找包含特定文本的注释。它接受部件标签和待查找的文本作为参数。首先，它通过ask_next_note函数遍历部件中的注释对象。对于每个注释对象，它通过UF_DRF_ask_draft_aid_text_info函数获取注释中的文本信息，然后遍历每行文本，使用strstr函数检查是否包含待查找的文本。如果找到包含文本的注释，则返回该注释对象。
>
> 最后，UF_DRF_free_text函数用于释放获取的文本信息占用的内存。
>
> 综上所述，这段代码实现了在NX部件中高效查找包含特定文本的注释的功能，通过遍历和逐行匹配文本，实现了注释的检索。
>
