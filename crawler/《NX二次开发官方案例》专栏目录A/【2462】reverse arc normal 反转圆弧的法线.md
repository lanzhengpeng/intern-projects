### 【2462】reverse arc normal 反转圆弧的法线

#### 代码

```cpp
    /*HEAD REVERSE_ARC_NORMAL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_circle_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_arcs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void map_matrix2matrix(double *ref_mx, double *dest_mx, double loc[3])  
    {  
        int  
            ii,  
            irc;  
        double  
            d_csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            mx[16],  
            r_csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 };  
        for (ii = 0; ii < 9; ii++)  
        {  
            r_csys[ii+3] = ref_mx[ii];  
            d_csys[ii+3] = dest_mx[ii];  
        }  
        FTN(uf5940)(r_csys, d_csys, mx, &irc);  
        FTN(uf5941)(loc, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            arc;  
        double  
            flip_csys[9],  
            orig_csys[9];  
        UF_CURVE_arc_t  
            orig_data,  
            flip_data;  
        while ((arc = select_an_arc("Reverse Normal")) != NULL_TAG)  
        {  
            set_undo_mark("Reverse Arc Normal");  
            UF_CALL(UF_CURVE_ask_arc_data(arc, &orig_data));  
            UF_CALL(UF_CSYS_ask_matrix_values(orig_data.matrix_tag, orig_csys));  
            UF_VEC3_negate(orig_csys, flip_csys);  
            UF_VEC3_negate(&(orig_csys[3]), &(flip_csys[3]));  
            UF_VEC3_negate(&(orig_csys[6]), &(flip_csys[6]));  
            UF_CALL(UF_CSYS_create_matrix(flip_csys, &flip_data.matrix_tag));  
            flip_data.start_angle = orig_data.start_angle - PI;  
            if (flip_data.start_angle < 0)  
                flip_data.start_angle = flip_data.start_angle + TWOPI;  
            flip_data.end_angle = orig_data.end_angle + PI;  
            if ((flip_data.end_angle > TWOPI) &&  
                ((flip_data.end_angle - TWOPI) > flip_data.start_angle))  
                flip_data.end_angle = flip_data.end_angle - TWOPI;  
            UF_VEC3_copy(orig_data.arc_center, &(flip_data.arc_center));  
            map_matrix2matrix(orig_csys, flip_csys, flip_data.arc_center);  
            flip_data.radius = orig_data.radius;  
            UF_CALL(UF_CURVE_edit_arc_data(arc, &flip_data));  
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

> 这段代码是一个NX的二次开发示例，其主要功能是选择一个圆弧并反转其法线方向。以下是代码的主要组成部分：
>
> 1. 包含了必要的NX二次开发头文件，包括uf.h、uf_ui.h、uf_curve.h等。
> 2. 定义了一个ECHO宏，用于在列表窗口和系统日志中打印信息。
> 3. 定义了一个report_error函数，用于在函数调用出错时报告错误信息。
> 4. 定义了一个mask_for_arcs函数，用于设置选择掩码，只允许选择圆弧。
> 5. 定义了一个select_an_arc函数，用于提示用户选择一个圆弧，并返回其tag。
> 6. 定义了一个set_undo_mark函数，用于设置撤销标记。
> 7. 定义了一个map_matrix2matrix函数，用于将一个坐标系映射到另一个坐标系。
> 8. 定义了一个do_it函数，用于实现反转圆弧法线的功能：获取圆弧原始数据获取圆弧坐标系矩阵反转坐标系矩阵创建反转后的坐标系反转圆弧起始角度更新圆弧数据映射圆心点编辑圆弧数据
> 9. 获取圆弧原始数据
> 10. 获取圆弧坐标系矩阵
> 11. 反转坐标系矩阵
> 12. 创建反转后的坐标系
> 13. 反转圆弧起始角度
> 14. 更新圆弧数据
> 15. 映射圆心点
> 16. 编辑圆弧数据
> 17. 定义了ufusr主函数，用于初始化NX，调用do_it函数，然后终止NX。
> 18. 定义了一个ufusr_ask_unload函数，用于卸载NX时立即卸载二次开发代码。
>
> 综上所述，该代码通过反转圆弧坐标系矩阵，实现了圆弧法线方向的反转功能。
>
