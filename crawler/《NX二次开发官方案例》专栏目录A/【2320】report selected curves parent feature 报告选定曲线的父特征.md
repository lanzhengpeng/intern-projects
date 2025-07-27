### 【2320】report selected curves parent feature 报告选定曲线的父特征

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *feature;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &feature)))  
        {  
            WRITE_S(feature);  
            UF_free(feature);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve,  
            feat;  
        while ((curve = select_a_curve("Report feature of curve")) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_object_feat(curve, &feat)) &&  
                (feat != NULL_TAG))  
            {  
                ECHO("Curve's parent is ");  
                report_feature_name(feat);  
            }  
            else  
            {  
                uc1601("Curve has no associated feature", TRUE);  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了一个ECHO宏，用于在列表窗口和系统日志中输出信息。
> 2. 定义了一个UF_CALL宏，用于调用NX API函数，并报告错误。
> 3. 定义了一个mask_for_curves函数，用于设置选择曲线时的掩码。
> 4. 定义了一个select_a_curve函数，用于选择一条曲线，并返回其tag。
> 5. 定义了一个write_string_to_listing_window函数，用于在列表窗口输出字符串。
> 6. 定义了一个report_feature_name函数，用于报告曲线所属的特征名称。
> 7. 定义了一个do_it函数，循环选择曲线，并报告每条曲线所属的特征名称。
> 8. 定义了ufusr函数，用于初始化NX，执行do_it函数，然后终止NX。
> 9. 定义了ufusr_ask_unload函数，用于卸载该二次开发应用程序。
>
> 整个代码的主要逻辑是循环选择曲线，并报告每条曲线所属的特征名称。通过定义宏和函数，实现了调用NX API、错误处理、选择掩码设置、信息输出等功能。
>
