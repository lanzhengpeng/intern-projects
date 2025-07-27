### 【0901】create smart point at midpoint of selected curve or edge 在选择的曲线或边缘的中点创建智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINT_AT_MIDPOINT_OF_SELECTED_CURVE_OR_EDGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。

翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
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
            curve,  
            scalar,  
            smart_point;  
        while ((curve=select_a_curve_or_edge("Create smart midpoint"))!=NULL_TAG)  
        {  
            UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
                        0.5, &scalar));  
            UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
                curve, scalar, &smart_point));  
            UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_visible));  
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

> 根据代码注释和结构，这是用于NX的二次开发代码，主要功能是在NX中创建选中曲线或边缘的中点智能点。
>
> 代码的主要逻辑如下：
>
> 1. 错误处理函数：report_error()用于输出错误信息。
> 2. 选择曲线或边缘：mask_for_curves_and_edges()函数用于设置选择掩码，允许用户选择曲线或边缘。select_a_curve_or_edge()函数用于显示对话框，让用户选择曲线或边缘。
> 3. 创建智能点：do_it()函数负责循环调用select_a_curve_or_edge()选择曲线或边缘，然后调用UF_SO_create_scalar_double()和UF_SO_create_point_on_curve()函数在曲线或边缘的中点创建一个智能点，并设置智能点为可见。
> 4. 主函数：ufusr()负责初始化和终止NX，并调用do_it()函数。ufusr_ask_unload()函数用于卸载用户自定义程序。
>
> 综上所述，这段代码的主要目的是简化用户在NX中创建曲线或边缘中点智能点的过程。
>
