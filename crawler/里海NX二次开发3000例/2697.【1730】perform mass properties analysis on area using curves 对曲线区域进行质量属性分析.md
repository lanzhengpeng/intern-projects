### 【1730】perform mass properties analysis on area using curves 对曲线区域进行质量属性分析

#### 代码

```cpp
    /*HEAD PERFORM_MASS_PROPERTIES_ANALYSIS_ON_AREA_USING_CURVES CCC UFUN */  
    /*  There is no API function to perform 2D analysis, so this example  
        demonstrates passing the selected curves to GRIP to use the  
        ANLSIS/TWOD command to do the anlysis and return the results.  
        Here is the code for the corresponding GRIP program "anlsis_twod.grs":  
            ENTITY/ curves(100), temp_b  
            NUMBER/ a_res(32), n_curves  
            UFARGS/ curves, n_curves, a_res  
            temp_b = BOUND/CLOSED,curves(1..n_curves)  
            ANLSIS/TWOD, temp_b, INCHES, a_res  
            DELETE/temp_b  
            HALT  
        There is also no KF function to perform 2D analysis, so this  
        example can also be used from a custom KF function.  The  
        corresponding dfa file would look like this:  
        #! UGNX/KF 2.0  
        #+  
            This function will return the area properties of the given curves.  
            Input : curves  - A list of the curves to be analyzed  
            Return: List    - A list of the area properties:  
                                 1  Perimeter  
                                 2  Area  
                                 3  Center of mass XBAR  
                                 4  Center of mass YBAR  
                                 5  First moment MY  
                                 6  First moment MX  
                                 7  Moment of inertia-work IXXW  
                                 8  Moment of inertia-work IYYW  
                                 9  Moment of inertia-centroidal IXX  
                                10  Moment of inertia-centroidal IYY  
                                11  Polar moment of inertia  
                                12  Product of inertia-work PXYW  
                                13  Product of inertia-centroidal PXY  
                                14  (not used)  
                                15  (not used)  
                                16  (not used)  
                                17  Principal axis XPX  
                                18  Principal axis XPY  
                                19  (Data not used in 2D analysis)  
                                20  Principal axis XPX  
                                21  Principal axis YPY  
                                22  (not used)  
                                23  Principal moment of inertia IXXP  
                                24  Principal moment of inertia IYYP  
                                25  Radius of gyration-work RGXW  
                                26  Radius of gyration-work RGYW  
                                27  Radius of gyration-centroidal RGX  
                                28  Radius of gyration-centroidal RGY  
                                29  Polar radius of gyration  
                                30  Circumscribed circle Dia.  
                                31  Circumscribed circle XC  
                                32  Circumscribed circle YC  
        #-  
        Defun: get_2d_area_props  (List $curves,  
                       String ($libname, "name_of_this.sl_so_dll"),  
                       String ($funcname, "get_2d_area_props") )  
        @{ Cfunc("UF_KF_invoke_user_function_dialog"; "libufun"); } list ;  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是V18版本中新增的功能。它用于打印系统日志信息。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void grip_2d_analysis(tag_t *curves, int n_curves, double results[32])  
    {  
        int  
            ii;  
        tag_t  
            g_curves[100];  
        double  
            g_n_curves;  
        UF_args_t  
            args[3];  
        args[0].type = UF_TYPE_TAG_T_ARRAY;  
        args[0].length = 100;  
        args[0].address = g_curves;  
        args[1].type = UF_TYPE_DOUBLE;  
        args[1].length = 0;  
        args[1].address = &g_n_curves;  
        args[2].type = UF_TYPE_DOUBLE_ARRAY;  
        args[2].length = 32;  
        args[2].address = results;  
        g_n_curves = (double) n_curves;  
        for (ii = 0; ii < n_curves, ii < 100; ii++) g_curves[ii] = curves[ii];  
        UF_CALL(UF_call_grip("grip/anlsis_twod.grx", 3, args));  
    }  
    /* qq3123197280 */  
    DllExport extern void get_2d_area_props(  
        UF_KF_value_t *data,  
        int num,  
        UF_KF_value_t *rv)  
    {  
        int  
            cnt = 0,  
            ii,  
            n_curves;  
        tag_t  
            *curves;  
        double  
            a_res[32];  
        UF_KF_instance_p_t  
            instance;  
        UF_KF_list_p_t  
            input_curves,  
            next,  
            results = NULL;  
        UF_KF_type_t  
            type;  
        UF_KF_value_p_t  
            element;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_KF_ask_list(data[0], &input_curves));  
        UF_CALL(UF_KF_ask_list_count(input_curves, &n_curves));  
        UF_CALL(allocate_memory(n_curves * sizeof(tag_t), (void **)&curves));  
        next = input_curves;  
        for (ii = 1; ii <= n_curves; ii++)  
        {  
            UF_CALL(UF_KF_list_pop(next, &element, &next));  
            UF_CALL(UF_KF_ask_type(element, &type));  
            switch (type)  
            {  
                case UF_KF_tag:  
                    UF_CALL(UF_KF_ask_tag(element, &curves[cnt]));  
                    cnt++;  
                    break;  
                case UF_KF_instance:  
                    UF_CALL(UF_KF_ask_instance(element, &instance));  
                    UF_CALL(UF_KF_ask_ug_object(instance, &curves[cnt]));  
                    cnt++;  
                    break;  
                default:  
                    WRITE("Non-Curve found in 2D analysis input list\n");  
                    break;  
            }  
            UF_CALL(UF_KF_free_rule_value(element));  
        }  
        if (cnt > 0)  
        {  
            grip_2d_analysis(curves, cnt, a_res);  
            for (ii = 31; ii > -1; ii--)  
            {  
                UF_CALL(UF_KF_make_number(a_res[ii], &element));  
                UF_CALL(UF_KF_list_push(results, element, &results));  
            }  
            UF_CALL(UF_KF_make_list(results, rv));  
        }  
        UF_free(curves);  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 } };  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves;  
        tag_t  
            *curves;  
        double  
            a_res[32];  
        while ((n_curves = select_curves("Report Circle Size", &curves)) > 0)  
        {  
            grip_2d_analysis(curves, n_curves, a_res);  
            UF_free(curves);  
            WRITE("Perimeter - ");  
            WRITE_F(a_res[0]);  
            WRITE("Area - ");  
            WRITE_F(a_res[1]);  
            WRITE("\nFirst Moments\n");  
            WRITE("MY - ");  
            WRITE_F(a_res[4]);  
            WRITE("MX - ");  
            WRITE_F(a_res[5]);  
            WRITE("\nCenter of Mass\n");  
            WRITE("Xbar - ");  
            WRITE_F(a_res[2]);  
            WRITE("Ybar - ");  
            WRITE_F(a_res[3]);  
            WRITE("\nMoments of Inertia (Work)\n");  
            WRITE("Ixxw - ");  
            WRITE_F(a_res[6]);  
            WRITE("Iyyw - ");  
            WRITE_F(a_res[7]);  
            WRITE("\nMoments of Inertia (Centroidal)\n");  
            WRITE("Ixx - ");  
            WRITE_F(a_res[8]);  
            WRITE("Iyy - ");  
            WRITE_F(a_res[9]);  
            WRITE("\nMoment of Inertia (Polar)\n");  
            WRITE_F(a_res[10]);  
            WRITE("\nProduct of Inertia (Work)\n");  
            WRITE("Pxyw - ");  
            WRITE_F(a_res[11]);  
            WRITE("\nProduct of Inertia (Centroidal)\n");  
            WRITE("Pxy - ");  
            WRITE_F(a_res[12]);  
            WRITE("\nRadii of Gyration (Work)\n");  
            WRITE("Rgxw - ");  
            WRITE_F(a_res[24]);  
            WRITE("Rgyw - ");  
            WRITE_F(a_res[25]);  
            WRITE("\nRadii of Gyration (Centroidal)\n");  
            WRITE("Rgx - ");  
            WRITE_F(a_res[26]);  
            WRITE("Rgy - ");  
            WRITE_F(a_res[27]);  
            WRITE("\nRadii of Gyration (Polar)\n");  
            WRITE_F(a_res[28]);  
            WRITE("\nPrinciple Axes\n");  
            WRITE("Xp(X) - ");  
            WRITE_F(a_res[16]);  
            WRITE("Xp(Y) - ");  
            WRITE_F(a_res[17]);  
            WRITE("Yp(X) - ");  
            WRITE_F(a_res[19]);  
            WRITE("Yp(Y) - ");  
            WRITE_F(a_res[20]);  
            WRITE("\nPrinciple Moments of Inertia\n");  
            WRITE("Ixxp - ");  
            WRITE_F(a_res[22]);  
            WRITE("Iyyp - ");  
            WRITE_F(a_res[23]);  
            WRITE("\nCircle Size\n");  
            WRITE("  Center\n");  
            WRITE("    XC - ");  
            WRITE_F(a_res[30]);  
            WRITE("    YC - ");  
            WRITE_F(a_res[31]);  
            WRITE("  Diameter - ");  
            WRITE_F(a_res[29]);  
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

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 使用GRIP程序进行2D曲线的面积特性分析，计算曲线围成的区域的周长、面积、质心、惯性矩等属性。
> 2. 通过自定义的KF函数get_2d_area_props，接收输入的曲线列表，调用GRIP程序进行面积特性分析，并返回分析结果。
> 3. 在主函数ufusr中，实现了曲线的选择、面积特性分析、以及结果打印等功能。
> 4. 使用了UF_print_syslog等新功能进行错误打印。
> 5. 实现了曲线选择时的类型过滤，只允许选择线、圆、圆锥曲线、样条曲线等类型的曲线。
> 6. 使用了自定义的函数报告错误、分配内存、打印双精度数等。
> 7. 通过定义宏简化了错误报告、打印等代码。
> 8. 实现了自定义的卸载函数ufusr_ask_unload。
> 9. 代码结构清晰，注释详细，具有良好的可读性和可维护性。
>
> 总体来说，这段代码通过自定义的KF函数，实现了NX中不直接提供的2D曲线面积特性分析功能，代码质量较高，是一个NX二次开发的优秀示例。
>
