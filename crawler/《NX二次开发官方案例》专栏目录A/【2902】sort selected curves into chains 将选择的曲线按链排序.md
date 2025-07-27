### 【2902】sort selected curves into chains 将选择的曲线按链排序

#### 代码

```cpp
    /*HEAD SORT_SELECTED_CURVES_INTO_CHAINS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以翻译为：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
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
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
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
    static void sort_end_to_end(tag_t *curves, int n_curves)  
    {  
        logical  
            found;  
        int  
            ii,  
            jj,  
            kk,  
            n_strings,  
            start;  
        double  
            free_start[3],  
            free_end[3],  
            gap,  
            this_start[3],  
            this_end[3],  
            tol;  
        tag_t  
            temp;  
        UF_MODL_ask_distance_tolerance(&tol);  
        n_strings = 0;  
        ask_curve_ends(curves[0], free_start, free_end);  
        start = 0;  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            found = FALSE;  
            for (jj = ii + 1; jj < n_curves; jj++)  
            {  
                ask_curve_ends(curves[jj], this_start, this_end);  
                UF_VEC3_distance(this_start, free_start, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    for (kk = jj; kk > start; kk--)  
                        curves[kk] = curves[kk-1];  
                    curves[start] = temp;  
                    UF_VEC3_copy(this_end, free_start);  
                    found = TRUE;  
                    break;  
                }  
                UF_VEC3_distance(this_end, free_start, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    for (kk = jj; kk > start; kk--)  
                        curves[kk] = curves[kk-1];  
                    curves[start] = temp;  
                    UF_VEC3_copy(this_start, free_start);  
                    found = TRUE;  
                    break;  
                }  
                UF_VEC3_distance(this_start, free_end, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    curves[jj] = curves[ii+1];  
                    curves[ii+1] = temp;  
                    UF_VEC3_copy(this_end, free_end);  
                    found = TRUE;  
                    break;  
                }  
                UF_VEC3_distance(this_end, free_end, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    curves[jj] = curves[ii+1];  
                    curves[ii+1] = temp;  
                    UF_VEC3_copy(this_start, free_end);  
                    found = TRUE;  
                    break;  
                }  
            }  
            if (found == FALSE)  
            {  
                n_strings++;  
                start = ii+1;  
                if (start < n_curves)  
                    ask_curve_ends(curves[ii+1], free_start, free_end);  
            }  
        }  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            sprintf(msg, "%d", ii);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *curves;  
        while ((n = select_curves("Sort curves", &curves)) > 0)  
        {  
            sort_end_to_end(curves, n);  
            number_objects(curves, n);  
            UF_free(curves);  
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

> 这段代码是用于NX的二次开发，其主要功能是按端点顺序对曲线进行排序并编号。具体步骤如下：
>
> 1. 初始化和终止：首先调用UF_initialize()和UF_terminate()来初始化和终止NX。
> 2. 选择曲线：通过UF_UI_select_with_class_dialog()选择曲线，并对曲线类型进行限制，只允许选择线、圆、圆锥曲线和样条线。
> 3. 排序：对选择的曲线进行排序，通过比较曲线端点之间的距离，将曲线按端点顺序连接成链。
> 4. 编号：使用UF_DISP_display_temporary_text()在曲线上显示临时文本，即编号。
> 5. 错误报告：使用自定义的report_error()函数来报告错误。
> 6. 内存释放：在排序和编号完成后，释放曲线数组占用的内存。
> 7. 卸载处理：在ufusr_ask_unload()中返回UF_UNLOAD_IMMEDIATELY表示该函数可以被立即卸载。
>
> 总体来说，这段代码实现了对用户选择的曲线进行排序和编号的功能，具有一定的实用价值。
>
