### 【1400】highlight section view and label of selected sxseg line 高亮选中SXSEG线的截面视图和标签

#### 代码

```cpp
    /*HEAD HIGHLIGHT_SECTION_VIEW_AND_LABEL_OF_SELECTED_SXSEG_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

请注意，UF_print_syslog 是 V18 版本新增的。 */  
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
    /* qq3123197280 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_section_segment_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
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
        int  
            ii,  
            n_sxviews;  
        tag_t  
            label,  
            line,  
            part = UF_PART_ask_display_part(),  
            *sxviews;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
        n_sxviews = ask_all_of_type_and_subtype(part, UF_view_type,  
            UF_view_section_subtype, &sxviews);  
        while ((line = select_a_line("Select line to query")) != NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            for (ii = 0; ii < n_sxviews; ii++)  
            {  
                UF_CALL(UF_OBJ_delete_object(sxviews[ii]));  
                if (UF_OBJ_ask_status(line) == UF_OBJ_DELETED) break;  
            }  
            UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            UF_CALL(UF_DISP_regenerate_display());  
            UF_CALL(UF_DISP_set_highlight(line, TRUE));  
            if (ii < n_sxviews)  
            {  
                UF_CALL(UF_DISP_set_highlight(sxviews[ii], TRUE));  
                UF_CALL(UF_DRAW_ask_view_label(sxviews[ii], &label));  
                UF_CALL(UF_DISP_set_highlight(label, TRUE));  
                uc1601("Section view & label highlighted", TRUE);  
                UF_CALL(UF_DISP_set_highlight(sxviews[ii], FALSE));  
                UF_CALL(UF_DISP_set_highlight(label, FALSE));  
            }  
            else  
                uc1601("This is NOT a section line segment", TRUE);  
            UF_CALL(UF_DISP_set_highlight(line, FALSE));  
        }  
        if (n_sxviews > 0) UF_free(sxviews);  
        UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
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

> 这段代码是一个NX的二次开发示例，主要功能是选择线段，高亮显示相关的截面视图和标签。
>
> 主要步骤包括：
>
> 1. 设置选择掩码，只允许选择线段。
> 2. 通过对话框让用户选择一个线段。
> 3. 在当前部件中查询所有的截面视图。
> 4. 遍历每个截面视图，尝试删除选择的线段，如果成功，说明该线段属于这个截面视图。
> 5. 高亮显示该截面视图和对应的标签。
> 6. 如果没有找到相关的截面视图，提示用户该线段不是截面线段。
> 7. 重复上述过程，直到用户没有选择新的线段。
>
> 代码中还包括错误报告、内存分配、列表操作、对象类型查询等辅助函数。整体结构清晰，逻辑严谨，是一个很好的NX二次开发示例。
>
