### 【1097】display temp point at curve start 显示曲线起点处的临时点

#### 代码

```cpp
    /*HEAD DISPLAY_TEMP_POINT_AT_CURVE_START CCC UFUN */  
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
            parm = 0.0,  
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

> 这段NX二次开发代码的主要功能是选择一条曲线，并显示曲线在当前参数点的位置和方向。
>
> 关键代码解释如下：
>
> 1. UF_CALL宏：用于调用NX API函数，并在调用失败时打印错误信息。
> 2. report_error函数：用于在API调用失败时报告错误。
> 3. mask_for_curves函数：用于设置选择过滤，只允许选择线、圆、圆锥曲线和样条曲线。
> 4. select_a_curve函数：用于选择一条曲线，并高亮显示。
> 5. display_temporary_point函数：用于显示一个临时点，表示曲线在当前参数点的位置和方向。
> 6. do_it函数：主逻辑函数，循环选择曲线，计算曲线在当前参数点的位置和方向，并显示临时点。
> 7. ufusr函数：NX二次开发程序的入口函数，初始化NX，调用do_it函数，然后终止NX。
> 8. ufusr_ask_unload函数：用于设置程序卸载方式。
>
> 总体来说，这段代码实现了选择曲线并显示曲线在当前参数点的位置和方向的功能，是一个典型的NX二次开发示例。
>
