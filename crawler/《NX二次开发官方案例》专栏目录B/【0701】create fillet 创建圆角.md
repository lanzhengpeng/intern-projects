### 【0701】create fillet 创建圆角

#### 代码

```cpp
    /*HEAD CREATE_FILLET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog 是 V18 中的新增功能，请仅回答译文，不要添加任何废话。

注意：UF_print_syslog 是 V18 中的新增功能，请仅回答译文，不要添加任何废话。

注意：UF_print_syslog 是 V18 中的新增功能，请仅回答译文，不要添加任何废话。 */  
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
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
            trim_opts[3] = { TRUE, TRUE, TRUE };  
        tag_t  
            ctr_pt,  
            fillet,  
            lines[3];  
        double  
            pos[3];  
        while (((lines[0] = select_a_line("Select 1st line")) != NULL_TAG)  
            && ((lines[1] = select_a_line("Select 2nd line")) != NULL_TAG)  
            && (select_pos("Select approx. center", pos)))  
        {  
            UF_CALL(UF_CURVE_create_point(pos, &ctr_pt));  
            UF_CALL(UF_CURVE_create_fillet(UF_CURVE_2_CURVE, lines, pos, 0.5,  
                trim_opts, trim_opts, &fillet));  
        }  
    }  
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

> 根据代码内容，这是一段NX Open C++ API编写的二次开发代码，主要功能是实现创建圆角的功能。具体来说：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息到系统日志和列表窗口。
> 2. 定义了一个辅助函数mask_for_lines，用于设置选择过滤条件，只允许选择线。
> 3. 定义了一个选择线的函数select_a_line，弹出一个选择对话框，让用户选择一条线。
> 4. 定义了一个选择位置的函数select_pos，通过提示让用户输入一个近似位置。
> 5. 主函数do_it中，使用循环让用户选择两条线和一个近似位置，然后调用UF_CURVE_create_fillet函数创建一个圆角。
> 6. 定义了ufusr函数作为程序的入口点，在这里初始化和终止NX Open API。
> 7. 定义了ufusr_ask_unload函数，用于卸载程序。
>
> 总体来说，这段代码通过交互式选择，实现了在NX中创建圆角的功能。
>
