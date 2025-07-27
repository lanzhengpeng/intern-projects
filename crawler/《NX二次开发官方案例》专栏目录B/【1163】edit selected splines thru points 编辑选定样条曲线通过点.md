### 【1163】edit selected splines thru points 编辑选定样条曲线通过点

#### 代码

```cpp
    /*HEAD EDIT_SELECTED_SPLINES_THRU_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_spline_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_spline_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_spline_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a spline curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_spline_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_spline_thru_points(tag_t spline)  
    {  
        int  
            degree,  
            ii,  
            num_points,  
            periodicity;  
        double  
            *parms;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        if (!UF_CALL(UF_CURVE_ask_spline_thru_pts(spline, &degree, &periodicity,  
            &num_points, &point_data, &parms)))  
        {  
            WRITE_D(spline);  
            WRITE_D(degree);  
            WRITE_D(periodicity);  
            WRITE_D(num_points);  
            for (ii = 0; ii < num_points + periodicity; ii++)  
            {  
                WRITE_D(ii);  
                WRITE3F(point_data[ii].point);  
                WRITE_D(point_data[ii].slope_type);  
                WRITE_F(parms[ii]);  
            }  
            UF_free(point_data);  
            UF_free(parms);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            is_so;  
        int  
            degree,  
            ii, jj,  
            num_pts,  
            periodicity;  
        tag_t  
            *parents,  
            new_point,  
            spline,  
            xyz[3];  
        double  
            distance,  
            loc[3],  
            *parms;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        while ((spline = select_a_spline_curve("Select spline")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_CURVE_ask_spline_thru_pts(spline, &degree,  
                &periodicity, &num_pts, &point_data, &parms))) continue;  
            report_spline_thru_points(spline);  
            UF_CALL(UF_SO_is_so(spline, &is_so));  
            if (is_so)  
            {  
                UF_CALL(UF_SO_ask_parents(spline, UF_SO_ASK_ALL_PARENTS, &num_pts,  
                    &parents));  
                for (ii = 0; ii < num_pts; ii++)  
                {  
                    UF_CALL(UF_CURVE_ask_point_data(parents[ii], loc));  
                    UF_VEC3_distance(loc, point_data[0].point, &distance);  
                    if (distance > 0) continue;  
                    loc[0] += 1.1;  
                    for (jj = 0; jj < 3; jj++)  
                        UF_CALL(UF_SO_create_scalar_double(spline,  
                            UF_SO_update_within_modeling, loc[jj], &xyz[jj]));  
                    UF_CALL(UF_SO_create_point_3_scalars(spline,  
                        UF_SO_update_within_modeling, xyz, &new_point));  
                    UF_CALL(UF_SO_replace_parms(parents[ii], new_point));  
                    UF_CALL(UF_MODL_update());  
                }  
                UF_free(parents);  
                WRITE("\nTweaked first point\n\n");  
            }  
            else  
            {  
                point_data[num_pts-1].point[0] += 1.1;  
                UF_CALL(UF_CURVE_edit_spline_thru_pts(spline, degree,  
                    periodicity, num_pts, point_data, parms, TRUE));  
                WRITE("\nTweaked last point\n\n");  
            }  
            report_spline_thru_points(spline);  
            UF_free(point_data);  
            UF_free(parms);  
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

> 这段代码是NX的二次开发代码，主要功能是选择一个样条曲线，然后修改其通过点。
>
> 主要步骤如下：
>
> 1. 使用UF_UI库中的函数选择一个样条曲线。
> 2. 使用UF_CURVE库中的函数获取样条曲线的通过点信息，包括度数、周期性、通过点数量、通过点坐标和斜率类型等。
> 3. 打印出样条曲线的通过点信息。
> 4. 如果样条曲线是子对象，则获取其父对象，并修改第一个通过点的坐标。
> 5. 如果样条曲线不是子对象，则直接修改最后一个通过点的坐标。
> 6. 调用UF_CURVE库中的函数，根据修改后的通过点信息重新编辑样条曲线。
> 7. 打印出修改后的样条曲线通过点信息。
> 8. 重复上述步骤，直到用户选择退出。
>
> 代码中还包含一些辅助函数，用于打印信息、选择样条曲线、报告通过点信息等。整体而言，该代码实现了选择和编辑样条曲线通过点的功能。
>
