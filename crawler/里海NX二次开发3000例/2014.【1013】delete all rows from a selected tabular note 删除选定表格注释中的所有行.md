### 【1013】delete all rows from a selected tabular note 删除选定表格注释中的所有行

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_tabnot.h>  
    #include <uf_undo.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static int mask_for_tabular_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_tabular_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a tabular note", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_tabular_notes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        int  
            ii = 0,  
            nm_rows = 0;  
        tag_t  
            *rows = NULL,  
            row = NULL_TAG,  
            section = NULL_TAG,  
            tabnote = NULL_TAG;  
        uf_list_p_t  
            row_list;  
        UF_UNDO_mark_id_t  
            mark_id;  
        section = select_a_tabular_note("Select a tabular note");  
        if (section == NULL_TAG) return;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Remove All Rows from Tabular Note", &mark_id));  
        UF_CALL( UF_TABNOT_ask_tabular_note_of_section ( section, &tabnote ) );  
        // ask how many rows there are  
        UF_CALL(UF_TABNOT_ask_nm_rows ( tabnote, &nm_rows ));  
        // For speed tell NX not to update while deleting  
        UF_CALL( UF_TABNOT_enable_automatic_update(false));  
        UF_CALL(UF_MODL_create_list(&row_list));  
        // remove all the rows from the tabular note  
        for (ii = nm_rows; ii > 0; ii--)  
        {  
            UF_CALL( UF_TABNOT_ask_nth_row (tabnote, ii-1, &row ) );  
            UF_CALL( UF_TABNOT_remove_row( row ) );  
            UF_CALL( UF_MODL_put_list_item( row_list, row));  
        }  
        // now delete the row objects  
        nm_rows = make_an_array(&row_list, &rows);  
        UF_CALL(UF_OBJ_delete_array_of_objects(nm_rows, rows, NULL));  
        UF_free(rows);  
        // turn update back on and force update the tabular note  
        UF_CALL( UF_TABNOT_enable_automatic_update(true));  
        UF_CALL( UF_TABNOT_update(tabnote));  
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

> 根据代码的内容，这是一段用于NX的二次开发代码，主要用于从表格注释中移除所有行的功能。
>
> 主要步骤包括：
>
> 1. 选择一个表格注释对象。
> 2. 获取表格注释对象中的所有行，并保存到一个列表中。
> 3. 禁止NX自动更新，并从表格注释对象中移除所有行。
> 4. 删除保存的行对象。
> 5. 恢复NX自动更新，并强制更新表格注释对象。
>
> 代码中使用了一些NX提供的API函数，如UF_UI、UF_MODL、UF_OBJ、UF_TABNOT、UF_UNDO等，用于实现上述功能。
>
> 另外，代码还定义了一些辅助函数，如ECHO用于输出信息，report_error用于报告错误，allocate_memory用于分配内存，make_an_array用于将列表转换为数组等。
>
> 总的来说，这段代码实现了从NX的表格注释对象中移除所有行的功能。
>
