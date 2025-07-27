### 【1010】delete all notes with no text 删除所有无文字的注释

#### 代码

```cpp
    /*HEAD DELETE_ALL_NOTES_WITH_NO_TEXT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

UF_print_syslog 是在 V18 中新增的功能。

只回答译文，不要添加其他废话。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_of_type_and_subtype(tag_t part, int type, int subtype,  
        tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static void do_it(void)  
    {  
        logical  
            bad;  
        int  
            ii, jj, kk,  
            n_bad,  
            n_notes,  
            n_text;  
        tag_t  
            *the_notes,  
            part = UF_PART_ask_display_part();  
        char  
            msg[133];  
        uf_list_p_t  
            bad_list;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        n_notes = ask_all_of_type_and_subtype(part, UF_drafting_entity_type,  
            UF_draft_note_subtype, &the_notes);  
        if (n_notes > 0)  
        {  
            UF_CALL(UF_MODL_create_list(&bad_list));  
            for (ii = 0; ii < n_notes; ii++)  
            {  
                bad = TRUE;  
                UF_CALL(UF_DRF_ask_draft_aid_text_info(the_notes[ii], &n_text,  
                    &text_info));  
                if (n_text > 0)  
                {  
                    for (jj = 0; (jj < n_text) && bad; jj++)  
                        for (kk = 0; (kk < text_info[jj].num_lines) && bad; kk++)  
                            if (strlen(text_info[jj].text[kk].string) > 0)  
                                bad = FALSE;  
                    UF_CALL(UF_DRF_free_text(n_text, &text_info));  
                }  
                if (bad) UF_CALL(UF_MODL_put_list_item(bad_list, the_notes[ii]));  
            }  
            UF_free(the_notes);  
            n_bad = make_an_array(&bad_list, &the_notes);  
            if (n_bad > 0)  
            {  
                UF_CALL(UF_OBJ_delete_array_of_objects(n_bad, the_notes, NULL));  
                UF_free(the_notes);  
                sprintf(msg, "Deleted %d invisible notes", n_bad);  
                uc1601(msg, TRUE);  
            }  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，其主要功能是删除当前零件中所有没有文字的注释。以下是代码的主要逻辑：
>
> 1. 通过UF_PART_ask_display_part()获取当前显示的零件。
> 2. 调用ask_all_of_type_and_subtype()函数获取当前零件中所有的注释对象。
> 3. 遍历所有注释对象，调用UF_DRF_ask_draft_aid_text_info()获取注释中的文字信息。
> 4. 如果注释对象中没有文字，将其添加到bad_list列表中。
> 5. 最后删除bad_list列表中的所有注释对象。
> 6. 通过uc1601()函数在NX中显示删除的注释数量。
> 7. 提供了UF_CALL宏来调用NX的API，并在出错时报告错误。
> 8. 提供了allocate_memory()和make_an_array()辅助函数来处理NX的内存分配和链表。
> 9. ufusr()是NX调用的入口函数，用于初始化和执行主要逻辑。
> 10. ufusr_ask_unload()用于设置二次开发的unload策略。
>
> 总的来说，这段代码通过遍历当前零件的注释对象，找出没有文字的注释并删除，以提高图纸的质量。
>
