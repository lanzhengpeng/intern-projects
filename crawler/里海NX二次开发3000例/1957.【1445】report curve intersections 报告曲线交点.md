### 【1445】report curve intersections 报告曲线交点

#### 代码

```cpp
    /*HEAD REPORT_CURVE_INTERSECTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
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
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_curve_intersections(tag_t curve1, tag_t curve2)  
    {  
        int  
            ii,  
            n_intersections;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        WRITE_D(curve1);  
        WRITE_D(curve2);  
        UF_CALL(UF_MODL_intersect_objects(curve1, curve2, 0, &n_intersections,  
            &intersections));  
        WRITE_D(n_intersections);  
        for (ii = 0; ii < n_intersections; ii++)  
        {  
            switch (intersections[ii]->intersect_type)  
            {  
                case UF_MODL_INTERSECT_POINT:  
                    WRITE("UF_MODL_INTERSECT_POINT: - ");  
                    WRITE_D(ii);  
                    WRITE3F(intersections[ii]->intersect.point.coords);  
                    WRITE_F(intersections[ii]->intersect.point.object_1_u_parm);  
                    WRITE_F(intersections[ii]->intersect.point.object_1_v_parm);  
                    WRITE_F(intersections[ii]->intersect.point.object_2_u_parm);  
                    WRITE_F(intersections[ii]->intersect.point.object_2_v_parm);  
                    break;  
                case UF_MODL_INTERSECT_COINCIDE:  
                    WRITE("UF_MODL_INTERSECT_COINCIDE: - ");  
                    WRITE_D(ii);  
                    UF_CALL(UF_OBJ_delete_object(  
                        intersections[ii]->intersect.coincide.identifier));  
                    WRITE_F(intersections[ii]->  
                        intersect.coincide.object_1_first_parm);  
                    WRITE_F(intersections[ii]->  
                        intersect.coincide.object_1_second_parm);  
                    WRITE_F(intersections[ii]->  
                        intersect.coincide.object_2_first_parm);  
                    WRITE_F(intersections[ii]->  
                        intersect.coincide.object_2_second_parm);  
                    break;  
                case UF_MODL_INTERSECT_CURVE:  
                    WRITE("UF_MODL_INTERSECT_CURVE: - ");  
                    WRITE_D(ii);  
                    UF_CALL(UF_OBJ_delete_object(  
                        intersections[ii]->intersect.curve.identifier));  
                    break;  
                default:  
                    WRITE("unknown intersection type: - ");  
                    WRITE_D(ii);  
                    break;  
            }  
            UF_free(intersections[ii]);  
        }  
        if (n_intersections > 0) UF_free(intersections);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curves[2];  
        while (((curves[0] = select_a_curve_or_edge("First curve")) != NULL_TAG) &&  
               ((curves[1] = select_a_curve_or_edge("Select curve")) != NULL_TAG))  
            report_curve_intersections(curves[0], curves[1]);  
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

> 根据代码的注释和功能，这是一个用于报告曲线交集的NX二次开发代码，其主要功能如下：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了用于写入列表窗口的宏WRITE、WRITE_D、WRITE_F、WRITE3F。
> 3. 定义了用于曲线和边的选择掩码mask_for_curves_and_edges。
> 4. 定义了选择曲线或边的函数select_a_curve_or_edge。
> 5. 定义了报告曲线交集的函数report_curve_intersections，其中使用UF_MODL_intersect_objects函数计算两条曲线的交集，并打印出交集点的信息。
> 6. 定义了主函数do_it，用于循环选择两条曲线，并报告它们的交集。
> 7. 定义了ufusr函数作为程序的入口，初始化NX环境，调用主函数do_it，然后终止NX环境。
> 8. 定义了卸载函数ufusr_ask_unload。
>
> 综合来看，这段代码的主要功能是让用户选择两条曲线，然后报告这两条曲线的交集信息，包括交点坐标、参数等信息。它通过NX的UF函数来实现曲线选择、交集计算和结果输出等功能。
>
