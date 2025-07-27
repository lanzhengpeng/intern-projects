### 【2998】trim curve to plane 将曲线裁剪到平面上

#### 代码

```cpp
    /*HEAD TRIM_CURVE_TO_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_curve.h>  
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
    /* qq3123197280 */  
    static int mask_for_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            junk[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk,  
            junk, junk));  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve,  
            epoint,  
            plane,  
            spoint;  
        double  
            end[3],  
            edist,  
            junk[3],  
            sdist,  
            start[3];  
        while (((curve = select_a_curve("Select curve to extend")) != NULL_TAG)  
            && ((plane = select_a_plane("Select plane to extend to")) != NULL_TAG))  
        {  
            ask_curve_ends(curve, start, end);  
            UF_CALL(UF_CURVE_create_point(start, &spoint));  
            UF_CALL(UF_MODL_ask_minimum_dist(spoint, plane, FALSE, junk,  
                FALSE, junk, &sdist, junk, junk));  
            UF_CALL(UF_OBJ_delete_object(spoint));  
            UF_CALL(UF_CURVE_create_point(end, &epoint));  
            UF_CALL(UF_MODL_ask_minimum_dist(epoint, plane, FALSE, junk,  
                FALSE, junk, &edist, junk, junk));  
            UF_CALL(UF_OBJ_delete_object(epoint));  
            if (edist < sdist) UF_VEC3_copy(end, start);  
            display_temporary_colored_point(start, UF_OBJ_YELLOW);  
            UF_CALL(UF_CURVE_edit_trim_curve(curve, plane, start, start,  
                UF_CURVE_NATURAL_SHAPE));  
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

> 这段NX二次开发代码的主要功能是用于修剪曲线到平面。具体介绍如下：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了曲线选择掩码函数mask_for_curves，允许选择直线、圆、圆锥曲线和样条曲线。
> 3. 定义了平面选择掩码函数mask_for_planes，只允许选择平面。
> 4. 定义了选择曲线函数select_a_curve，通过对话框提示用户选择曲线，并高亮显示。
> 5. 定义了选择平面函数select_a_plane，通过对话框提示用户选择平面，并高亮显示。
> 6. 定义了询问曲线端点函数ask_curve_ends，用于获取曲线的起点和终点坐标。
> 7. 定义了显示临时点函数display_temporary_colored_point，用于在指定位置显示一个临时点。
> 8. 定义了核心函数do_it，循环提示用户选择曲线和平面，获取曲线端点，计算端点到平面的距离，并使用UF_CURVE_edit_trim_curve函数修剪曲线到平面。
> 9. 主函数ufusr初始化NX，调用do_it函数，然后终止NX。
> 10. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 整体来看，这段代码通过选择曲线和平面，计算距离，并利用NX提供的曲线修剪功能，实现了将曲线修剪到平面的功能。
>
