### 【2543】section using planes perpendicular to curve 创建与曲线垂直的平面剖面

#### 代码

```cpp
    /*HEAD SECTION_USING_PLANES_PERPENDICULAR_TO_CURVE CCC UFUN */  
    /*  Note this does not work!  See PR 4482507 里海译:注意，这不起作用！请参阅 PR 4482507。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。

翻译结果：UF_print_syslog是V18版本中的新功能。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            sect_feat;  
        UF_CURVE_section_general_data_t  
            general_data = { NULL, 1, 1, 0, 0, 0.001 };  
        UF_CURVE_section_perpcrv_data_t  
            perpcrv_data = { NULL_TAG, 1, 1, 1, 50, 50, 0, 0, 0 };  
        general_data.objects = &body;  
        if (((body = select_a_body("Section")) != NULL_TAG) &&  
            ((perpcrv_data.curve_eid =  
                select_a_curve("Perpendicular planes along")) != NULL_TAG) &&  
            !UF_CALL(UF_CURVE_section_from_perpcrv_planes(&general_data,  
                &perpcrv_data, &sect_feat)))  
            WRITE_D(sect_feat);  
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

> 这段代码是基于NX的二次开发代码，主要用于创建基于曲线的法向截面的特征。代码的主要功能包括：
>
> 1. 选择一个实体作为截面实体。
> 2. 选择一条曲线作为法向参考曲线。
> 3. 沿着法向参考曲线创建法向平面，然后在这些平面上创建截面特征。
>
> 代码的主要流程包括：
>
> 1. 定义错误报告函数report_error，用于打印UF函数调用失败时的错误信息。
> 2. 定义两个选择函数mask_for_bodies和mask_for_curves，分别用于设置选择过滤条件，以选择实体和曲线。
> 3. 定义两个选择函数select_a_body和select_a_curve，用于弹出选择对话框，让用户选择实体和曲线。
> 4. 定义写入整数的函数write_integer_to_listing_window，用于将信息输出到日志窗口。
> 5. 定义主函数do_it，用于实现创建截面的流程：选择一个实体作为截面实体。选择一条曲线作为法向参考曲线。创建法向平面，然后在这些平面上创建截面特征。输出截面特征的ID。
> 6. 选择一个实体作为截面实体。
> 7. 选择一条曲线作为法向参考曲线。
> 8. 创建法向平面，然后在这些平面上创建截面特征。
> 9. 输出截面特征的ID。
> 10. 定义ufusr函数，作为NX的二次开发入口函数，调用主函数do_it。
> 11. 定义卸载函数ufusr_ask_unload，用于处理模块卸载。
>
> 总体来说，这段代码通过用户交互的方式，实现了基于曲线的法向截面的创建功能。
>
