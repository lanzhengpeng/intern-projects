### 【1096】display temp point at curve end 在曲线端点显示临时点

#### 代码

```cpp
    /*HEAD DISPLAY_TEMP_POINT_AT_CURVE_END CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        int  
            deriv_flag = UF_MODL_LOC;  
        tag_t  
            curve = NULL_TAG;  
        double  
            parm = 1.0,  
            pos_deriv[6] = { 0., 0., 0., 0., 0., 0. };  
        while((curve=select_a_curve("Select Curve:")) != NULL_TAG )  
        {  
            UF_CALL(UF_MODL_evaluate_curve( curve, &parm,  
                                  &deriv_flag, pos_deriv ));  
            display_temporary_point( pos_deriv );  
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

> 这段代码实现了在NX中显示曲线端点坐标的功能。主要功能如下：
>
> 1. 错误报告函数：定义了report_error函数，用于输出错误信息。
> 2. 曲线选择遮罩：定义了mask_for_curves函数，用于设置选择曲线的遮罩，仅允许选择线、圆、圆锥曲线和样条曲线。
> 3. 选择曲线：定义了select_a_curve函数，用于使用对话框让用户选择一条曲线，并返回曲线的tag。
> 4. 显示临时点：定义了display_temporary_point函数，用于在指定坐标处显示一个临时点。
> 5. 主体函数：定义了do_it函数，用于循环调用select_a_curve选择曲线，然后调用UF_MODL_evaluate_curve计算曲线端点坐标，并调用display_temporary_point显示该点。
> 6. 初始化和终止：ufusr函数用于初始化和调用do_it，然后终止。
> 7. 卸载请求：ufusr_ask_unload函数用于卸载请求，返回立即卸载。
>
> 总体来说，这段代码实现了曲线端点坐标的显示功能，为用户提供了一个简单的交互式操作。
>
