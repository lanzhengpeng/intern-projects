### 【0956】create trimmed tube 创建修剪管

#### 代码

```cpp
    /*HEAD CREATE_TRIMMED_TUBE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            n_features;  
        char  
            *dia[2] = { "10.", "5." };  
        /*************************************************************************  
             NOTE:  If you assign expressions, you must either NOT assign the   
             left-hand side - the expression name - or you must ensure that it   
             is unique if you plan to call the _create_trimmed_tube function   
             more than once in a given part file.  
             Failure to observe this will result in the error "Rule already exists"  
             the second time it is called, because the expression with that name  
             already exists.  
        **************************************************************************/  
        tag_t  
            *curves = NULL,  
            *features = NULL;  
        UF_MODL_SWEEP_TRIM_object_p_t  
            trim_data = NULL;  
        UF_FEATURE_SIGN  
            mode_sign = UF_NULLSIGN;  
        while ((n_curves = select_curves("Select tube path", &curves)) > 0)  
        {  
            UF_CALL(UF_MODL_create_trimmed_tube(curves, n_curves, trim_data,  
                dia, mode_sign, &features, &n_features));  
            UF_free(curves);  
            curves = NULL;  
            if (n_features > 0)   
            {  
                UF_free(features);  
                features = NULL;  
            }  
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

> 这是段NX二次开发代码，其主要功能是创建修剪后的管状特征。代码的主要流程如下：
>
> 1. 包含必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于在出错时输出错误信息。
> 3. 定义了一个用于设置选择过滤器的函数mask_for_curves，使得在选择对话框中只能选择线、圆、圆锥曲线、样条曲线等曲线类型。
> 4. 定义了一个选择曲线的函数select_curves，用于打开选择对话框，让用户选择曲线，并返回选择的曲线数组。
> 5. 定义了一个执行创建修剪后管状特征功能的函数do_it：
>
> 1. 定义了ufusr函数，用于在NX中加载并运行该代码。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示该代码在NX退出时不需要卸载。
>
> 综上所述，这段代码实现了根据用户选择的曲线创建修剪后的管状特征的功能，并提供了错误报告、选择过滤等辅助功能。
>
