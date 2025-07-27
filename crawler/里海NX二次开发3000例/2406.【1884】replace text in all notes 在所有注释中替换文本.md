### 【1884】replace text in all notes 在所有注释中替换文本

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical replace_characters_in_a_string(char *from, char *replace,  
        char *with, char *into, logical verbose)  
    {  
        char  
            *loc,  
            rest[133],  
            *start;  
        strcpy(into, from);  
        if (strstr(from, replace) == NULL) return FALSE;  
        if (verbose) WRITE("Replacing \"");  
        if (verbose) WRITE(from);  
        start = into;  
        while ((loc = strstr(start, replace)) != NULL)  
        {  
            strcpy(rest, loc + strlen(replace));  
            strcpy(loc, with);  
            strcat(into, rest);  
            start = loc + strlen(with);  
        }  
        if (verbose) WRITE("\" with \"");  
        if (verbose) WRITE(into);  
        if (verbose) WRITE("\"\n");  
        return TRUE;  
    }  
    static void replace_text_in_all_notes(char *old_text, char *new_text)  
    {  
        logical  
            found;  
        int  
            ii, jj,  
            n_text;  
        tag_t  
            a_note = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            note_text[132][133];  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((a_note = ask_next_note(part, a_note)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(a_note, &n_text, &text_info));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                if (text_info[ii].text_type != 1) continue;  
                found = FALSE;  
                for (jj = 0; jj < text_info[ii].num_lines; jj++)  
                {  
                    if (replace_characters_in_a_string(  
                        text_info[ii].text[jj].string, old_text, new_text,  
                        &note_text[jj][0], TRUE)) found = TRUE;  
                }  
                if (found) uc5566(a_note, 1, text_info[ii].num_lines, note_text);  
            }  
            UF_CALL(UF_DRF_free_text(n_text, &text_info));  
        }  
    }

```

#### 代码解析

> 这段代码是NX二次开发的示例，其功能是遍历当前打开部件的所有注释，并替换其中指定的文本。
>
> 具体来说，代码首先定义了一个宏WRITE，用于在列表窗口中输出信息。
>
> 接着定义了一个replace_characters_in_a_string函数，用于在字符串中查找和替换指定的文本。
>
> 然后定义了一个replace_text_in_all_notes函数，该函数遍历当前打开部件的所有注释，并使用replace_characters_in_a_string函数替换注释中的指定文本。
>
> 代码的主要逻辑是：
>
> 1. 获取当前打开部件的零件对象part。
> 2. 遍历该部件的所有注释对象a_note。
> 3. 对于每个注释对象，获取其文本信息text_info。
> 4. 遍历text_info中的每一行文本，使用replace_characters_in_a_string函数进行文本替换。
> 5. 如果替换成功，则更新注释的文本。
> 6. 释放text_info内存。
> 7. 继续遍历下一个注释对象。
>
> 通过这段代码，可以实现快速替换当前打开部件所有注释中的指定文本。
>
