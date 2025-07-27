### 【1444】report curve dimensionality 报告曲线维数

#### 代码

```cpp
    /*HEAD REPORT_CURVE_DIMENSIONALITY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的，仅回答译文，不要废话。 */  
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
    static tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
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
        int  
            dimensionality;  
        tag_t  
            curve;  
        double  
            data[6];  
        while ((curve = select_a_curve("Select curve")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_obj_dimensionality(curve, &dimensionality, data));  
            ECHO(dimensionality);  
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

> 这段NX二次开发代码的功能是获取选择曲线的维数和参数。
>
> 代码主要包含以下几个部分：
>
> 1. 头文件包含：包含了必要的NX API头文件。
> 2. 宏定义：定义了用于打印错误信息和函数调用信息的宏。
> 3. 错误报告函数：用于报告函数调用失败时的错误信息。
> 4. 曲线选择掩码函数：用于设置曲线选择时的掩码，仅允许选择线、圆、圆锥曲线和样条线。
> 5. 曲线选择函数：使用对话框让用户选择一个曲线，并返回曲线的tag。
> 6. 主函数：循环调用曲线选择函数，对每个选择的曲线，调用UF_MODL_ask_obj_dimensionality函数获取其维数和参数，并打印维数。
> 7. UF初始化/终止函数：用于在进入/退出NX时初始化/终止NX API。
> 8. 卸载函数：用于在卸载NX时立即卸载用户函数。
>
> 综合来看，这段代码通过选择曲线并获取其维数和参数，实现了基本的曲线信息查询功能。
>
