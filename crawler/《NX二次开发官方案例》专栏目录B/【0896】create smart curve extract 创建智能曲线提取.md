### 【0896】create smart curve extract 创建智能曲线提取

#### 代码

```cpp
    /*HEAD CREATE_SMART_CURVE_EXTRACT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文意，UF_print_syslog是V18版本新增的，所以翻译为：

注意：UF_print_syslog是在V18版本中新增的。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve1,  
            xform = NULL_TAG,  
            so_line = NULL_TAG;  
        int   
            type,  
            subtype;  
        while (((curve1 = select_a_curve("Select Curve")) != NULL_TAG) &&  
            (!UF_CALL(UF_OBJ_ask_type_and_subtype(curve1, &type, &subtype))))  
        {  
            UF_SO_create_curve_extract(curve1, UF_SO_update_within_modeling,  
                curve1, type, subtype, xform, &so_line);  
            UF_CALL(UF_SO_set_visibility_option(so_line, UF_SO_visible));  
            UF_CALL(UF_SO_display(so_line, UF_DISP_ALL_ACTIVE_VIEWS,  
                UF_DISP_USE_SPECIFIED_COLOR, UF_OBJ_YELLOW));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误代码和错误信息。
> 2. 定义了一个选择曲面的函数select_a_curve，用于使用对话框选择一个曲面，并返回曲面的tag。
> 3. 定义了一个设置选择掩码的函数mask_for_curves，用于只允许选择线、圆、圆锥曲线和样条曲线。
> 4. 定义了一个主函数do_it，用于循环选择曲线，对每条曲线调用UF_SO_create_curve_extract函数提取曲线特征，并设置显示选项。
> 5. 定义了ufusr函数，作为程序的入口，初始化NX环境，调用主函数，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 该代码通过提取曲线特征，并设置显示选项，实现了高亮显示曲线的功能。它是一个典型的NX二次开发示例，展示了错误处理、选择过滤、特征提取等常见的开发技术。
>
