### 【3156】report true angle between line and plane 计算直线和平面之间的真实角度

#### 代码

```cpp
    /*HEAD REPORT_TRUE_ANGLE_BETWEEN_LINE_AND_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的功能。 */  
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
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            line,  
            plane;  
        double  
            ccw[3],  
            c_end[3],  
            c_start[3],  
            end[3],  
            l_vec[3],  
            junk[3],  
            p_vec[3],  
            start[3],  
            true_angle;  
        while (((line = select_a_line("Select line")) != NULL_TAG) &&  
               ((plane = select_a_plane("Select plane")) != NULL_TAG))  
        {  
            ask_curve_ends(line, start, end);  
            UF_CALL(UF_MODL_ask_minimum_dist_2(NULL_TAG, plane, TRUE, start,  
                FALSE, junk, junk, junk, c_start, junk));  
            UF_CALL(UF_MODL_ask_minimum_dist_2(NULL_TAG, plane, TRUE, end,  
                FALSE, junk, junk, junk, c_end, junk));  
            UF_VEC3_sub(end, start, l_vec);  
            UF_VEC3_sub(c_end, c_start, p_vec);  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, start, l_vec, 0);  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, start, p_vec, 0);  
            UF_CALL(UF_MODL_ask_plane(plane, junk, ccw));  
            UF_VEC3_angle_between(l_vec, p_vec, ccw, &true_angle);  
            WRITE_F(true_angle * RADEG);  
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

> 这段代码是一个NX Open C++二次开发程序，用于计算直线与平面之间的真实夹角。主要功能包括：
>
> 1. 选择直线和平面：使用对话框让用户选择一个直线和一个平面，并高亮显示所选对象。
> 2. 获取直线端点：通过UF_MODL_ask_curve_props函数获取直线的起点和终点坐标。
> 3. 获取直线与平面的交点：使用UF_MODL_ask_minimum_dist_2函数获取直线与平面的交点。
> 4. 计算直线和平面的向量：通过起点和终点计算直线的向量和平面的法线向量。
> 5. 计算真实夹角：使用UF_VEC3_angle_between函数计算直线向量和平面法线向量之间的真实夹角。
> 6. 显示结果：将计算出的真实夹角以弧度或度数的形式输出到列表窗口。
> 7. 错误报告：使用自定义的report_error函数，可以打印出函数调用失败时的错误信息。
> 8. 重复选择：在用户没有选择退出时，程序会循环让用户继续选择直线和平面，并计算真实夹角。
> 9. 程序入口：ufusr函数是程序的入口函数，在NX中运行二次开发程序时会调用这个函数。
>
> 总体来说，这段代码通过二次开发实现了直线与平面之间真实夹角的测量，并提供了友好的用户交互界面。
>
