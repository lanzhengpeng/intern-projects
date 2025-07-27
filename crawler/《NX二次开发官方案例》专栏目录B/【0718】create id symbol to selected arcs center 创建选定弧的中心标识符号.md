### 【0718】create id symbol to selected arcs center 创建选定弧的中心标识符号

#### 代码

```cpp
    /*HEAD CREATE_ID_SYMBOL_TO_SELECTED_ARCS_CENTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能，用于打印系统日志信息。 */  
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
    /* 里海 */  
    static int mask_for_drf_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_circle_type, 0, 0 },  
                       {UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE },  
                       { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_arc(char *prompt, UF_DRF_object_p_t arc)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_arcs, NULL, &resp,  
            &arc->object_tag, cp, &arc->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(arc->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            id_sym;  
        double  
            cp[3] = { 0,0,0 },  
            pos[3];  
        char  
            id_text[MAX_ID_SYM_TEXT_LENGTH + 1];  
        UF_DRF_object_t  
            curve;  
        UF_CALL(UF_DRF_init_object_structure(&curve));  
        while (select_drf_arc("Select arc for id symbol", &curve))  
        {  
            if (select_pos("Indicate origin for id symbol", pos))  
            {  
            /*  The following call is a work around - see PR 1236714 译:The following call is a workaround - see PR 1236714 */  
                UF_CALL(UF_SO_create_point_conic_center(curve.object_tag,  
                    UF_SO_update_after_modeling, curve.object_tag,  
                    &curve.object_tag));  
                sprintf(id_text, "%d", ++ii);  
                UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, id_text, NULL,  
                    pos, UF_DRF_with_leader, UF_DRF_leader_attach_object, &curve,  
                    cp, &id_sym));  
            }  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open的二次开发示例，主要功能是创建特征标识符号。以下是代码的主要功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在发生错误时打印错误信息。
> 2. 选择过滤函数：定义了一个选择过滤函数mask_for_drf_arcs，用于在选择对话框中仅显示圆弧。
> 3. 选择圆弧函数：定义了一个选择圆弧函数select_drf_arc，用于提示用户选择一个圆弧，并高亮显示。
> 4. 选择位置函数：定义了一个选择位置函数select_pos，用于提示用户输入一个点坐标。
> 5. 主体函数：定义了一个主体函数do_it，用于循环提示用户选择圆弧和位置，然后为每个圆弧创建一个标识符号。
> 6. ufusr函数：定义了ufusr函数，用于初始化和清理NX Open，并调用主体函数。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载时立即卸载。
>
> 总体来说，这段代码通过对话框让用户选择圆弧和位置，并为每个选择的圆弧在指定位置创建一个标识符号。
>
