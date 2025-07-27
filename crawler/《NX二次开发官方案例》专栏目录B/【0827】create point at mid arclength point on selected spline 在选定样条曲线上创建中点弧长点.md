### 【0827】create point at mid arclength point on selected spline 在选定样条曲线上创建中点弧长点

#### 代码

```cpp
    /*HEAD CREATE_POINT_AT_MID_ARCLENGTH_POINT_ON_SELECTED_SPLINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_splines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_BCURVE_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_spline(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a spline", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_splines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void create_point_at_mid_length_of_curve(tag_t curve)  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_invisible));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.5, &offset));  
        UF_CALL(UF_SO_create_point_along_curve(curve, UF_SO_update_after_modeling,  
            curve, last, offset, UF_SO_point_along_curve_percent, FALSE, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            spline;  
        while ((spline = select_a_spline("Create mid-length point")) != NULL_TAG)  
            create_point_at_mid_length_of_curve(spline);  
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

> 这段NX二次开发代码的主要功能是在选择的样条曲线上创建一个中点。以下是代码的主要内容和功能介绍：
>
> 1. 包含了必要的NX Open API头文件。
> 2. 定义了一个错误报告函数report_error，用于输出错误代码和消息。
> 3. 定义了一个选择样条曲线的函数select_a_spline，它会弹出一个选择对话框让用户选择一个样条曲线。
> 4. 定义了一个在曲线中点创建点的函数create_point_at_mid_length_of_curve，它通过参数化创建一个参数为0.5的点作为曲线的中点。
> 5. 定义了一个主函数do_it，它会循环调用select_a_spline函数让用户选择样条曲线，然后对每条选择的曲线调用create_point_at_mid_length_of_curve函数创建中点。
> 6. 定义了UFusr函数，它是NX二次开发的入口函数，调用do_it函数执行主逻辑。
> 7. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总的来说，这段代码通过NX Open API实现了一个简单的样条曲线中点创建功能，用户可以通过选择样条曲线，在每条曲线上创建一个中点。
>
