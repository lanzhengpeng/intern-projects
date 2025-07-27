### 【3154】report selected curve data 选择并报告曲线数据

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_CURVE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是 V18 中的新增功能，只提供译文，不要添加无关内容。 */  
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
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
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
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:In case its not found，如果找不到的话 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_Tt(X) report_tag_type_and_subtype(#X, X)  
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        char  
            desc[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            ask_object_type_descriptor(type, subtype, desc);  
            sprintf(msg,"%s = %d, type = %d, subtype = %d (%s)\n", name, object,  
                type, subtype, desc);  
        }  
        else  
            sprintf(msg, "%s = %d (NULL_TAG)\n", name, object);  
        WRITE(msg);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    static void report_curve_data(tag_t curve)  
    {  
        int  
            type;  
        double  
            *curve_data;  
        UF_CURVE_struct_p_t  
            curve_struct;  
        if (UF_CALL(UF_CURVE_ask_curve_struct(curve, &curve_struct))) return;  
        UF_CALL(UF_CURVE_ask_curve_struct_data(curve_struct,&type,&curve_data));  
        switch (type)  
        {  
            case UF_point_type:  
                WRITE("Point:\n ");  
                WRITE(" Absolute coordinates:");  
                WRITE3F(curve_data);  
                break;  
            case UF_line_type:  
                WRITE("Line:\n");  
                WRITE(" Start coordinates:  ");  
                WRITE3F(&curve_data[3]);  
                WRITE(" End coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                break;  
            case UF_circle_type:  
                WRITE("Arc:\n");  
                WRITE(" Start angle (radians):  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" End angle (radians):  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                WRITE(" X axis of construction plane:  ");  
                WRITE3F(&curve_data[9]);  
                WRITE(" Y axis of construction plane:  ");  
                WRITE3F(&curve_data[12]);  
                break;  
            case UF_conic_type:  
                WRITE("Conic:\n");  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[8]);  
                WRITE(" axis1:  ");  
                WRITE3F(&curve_data[11]);  
                WRITE(" axis2:  ");  
                WRITE3F(&curve_data[14]);  
                break;  
            case UF_spline_type:  
                WRITE("Spline:\n");  
                WRITE(" Number of poles:  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" Order:  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Knots:  ");  
                WRITEnF((int)(curve_data[3]+curve_data[4]),&curve_data[5]);  
                WRITE(" Poles:  ");  
                WRITEnF4((&curve_data[5])+(int)(curve_data[3]+curve_data[4]),  
                    curve_data[3]);  
                break;  
            default:  
                WRITE("Unknown curve type ");  
                WRITE_D(type);  
                break;  
        }  
        UF_CALL(UF_CURVE_free_curve_struct(curve_struct));  
        UF_free(curve_data);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves;  
        tag_t  
            *curves;  
        while ((n_curves = select_curves("Report curve data", &curves)) > 0)  
        {  
            for (ii = 0; ii < n_curves; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_Tt(curves[ii]);  
                report_curve_data(curves[ii]);  
                WRITE("\n");  
            }  
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

> 这是段NX二次开发代码，主要功能是选择曲线并报告曲线数据。
>
> 主要流程如下：
>
> 1. 选择曲线：通过对话框让用户选择曲线，并限定只能选择线、圆弧、圆锥曲线、样条和点曲线。
> 2. 报告曲线数据：对用户选择的每条曲线，报告其类型、位置坐标、参数等数据。
> 3. 错误处理：使用宏定义封装UF函数调用，进行错误处理。
> 4. 日志输出：通过UF UI函数，将报告信息输出到日志窗口。
> 5. 类型转换：通过解析头文件获取曲线类型和子类型的字符串描述。
> 6. 数据报告：针对不同类型的曲线，使用不同的方式报告其数据。
> 7. 内存释放：在报告完成后，释放曲线结构体和数据的内存。
> 8. 程序入口：通过ufusr函数实现程序的初始化、执行主体逻辑和终止。
>
> 总体来说，这段代码实现了选择曲线并报告其详细数据的功能，使用了NX的UF API进行曲线操作和信息查询。
>
