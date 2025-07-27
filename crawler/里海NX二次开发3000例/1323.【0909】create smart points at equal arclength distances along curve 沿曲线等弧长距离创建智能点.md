### 【0909】create smart points at equal arclength distances along curve 沿曲线等弧长距离创建智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINTS_AT_EQUAL_ARCLENGTH_DISTANCES_ALONG_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是在V18版本中新增的。

只回答译文，不要废话。

 */  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void create_smart_points_spaced_along_curve(tag_t curve, double distance)  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        double  
            total_dist,  
            length = 0.;  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0., 1., UF_MODL_UNITS_PART, &length));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            distance, &offset));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        for (total_dist = distance; total_dist <= length; total_dist = total_dist + distance )  
        {  
            UF_CALL(UF_SO_create_point_along_curve(curve,  
                UF_SO_update_after_modeling, curve, last, offset,  
                  UF_SO_point_along_curve_distance, FALSE, &last));  
            UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            dst = 10.0;  
        while (((curve = select_a_curve("Create points along")) != NULL_TAG)  
            && prompt_for_a_number("Distance", "Distance:", &dst))  
            create_smart_points_spaced_along_curve(curve, dst);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是在曲线上按照等距创建智能点。
>
> 1. 首先，定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个选择曲线的函数select_a_curve，它使用对话框让用户选择一个曲线。
> 3. 定义了一个提示输入距离的函数prompt_for_a_number，它使用对话框让用户输入距离。
> 4. 定义了主函数do_it，它在循环中先选择一个曲线，然后提示用户输入距离，接着在曲线上按照这个距离创建智能点。
> 5. ufusr是NX调用的主函数，在这里初始化NX环境，调用do_it，然后结束NX环境。
> 6. ufusr_ask_unload函数在NX卸载时调用，返回立即卸载标志。
>
> 总的来说，这段代码实现了在曲线上按照等距创建智能点的功能，通过NX提供的API实现。
>
