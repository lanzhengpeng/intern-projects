### 【1921】report all gdt size tolerance parameters 报告所有GDT尺寸公差参数

#### 代码

```cpp
    /*HEAD REPORT_ALL_GDT_SIZE_TOLERANCE_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_gdt.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，用于将系统日志信息打印到文件。 */  
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
    #define WRITE_E(X) (write_expression_to_listing_window(#X, X))  
    void write_expression_to_listing_window(char *title, tag_t expression)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *string;  
        UF_CALL(UF_MODL_ask_exp_tag_string(expression, &string));  
        sprintf(msg, "%s = %s\n", title, string);  
        WRITE(msg);  
        UF_free(string);  
    }  
    static tag_t ask_next_size_tolerance_feature(tag_t part, tag_t feat)  
    {  
        int  
            ii,  
            n_types,  
            subtype,  
            type;  
        UF_GDT_tolerance_type_t  
            *types;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tolerance_feature_type,  
                &feat)) && (feat != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(feat, &type, &subtype));  
            if (subtype == UF_tolerance_feature_subtype)  
            {  
                UF_CALL(UF_GDT_ask_tolerance_types(feat, &n_types, &types));  
                for (ii = 0; ii < n_types; ii++)  
                {  
                    if (types[ii] == UF_GDT_SIZE_TOLERANCE_TYPE)  
                    {  
                        UF_free(types);  
                        return feat;  
                    }  
                }  
                UF_free(types);  
            }  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_GDT_size_tolerance_p_t  
            size_data;  
        while ((feat = ask_next_size_tolerance_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_GDT_ask_size_tolerance_parms(feat, &size_data));  
            WRITE_F(size_data->nominal_value);  
            WRITE_E(size_data->upper_tol.expression);  
            WRITE_F(size_data->upper_tol.value);  
            WRITE_D(size_data->upper_tol.decimal_format);  
            WRITE_E(size_data->lower_tol.expression);  
            WRITE_F(size_data->lower_tol.value);  
            WRITE_D(size_data->lower_tol.decimal_format);  
            WRITE_D(size_data->tol_format);  
            WRITE_D(size_data->decimal_format);  
            UF_CALL(UF_GDT_free(UF_GDT_SIZE_TOL_TYPE, (void *)size_data));  
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

> 这段代码是一个NX Open API的二次开发示例，主要实现了以下功能：
>
> 1. 定义了宏UF_CALL，用于调用NX的API函数，并捕获错误，记录错误信息。
> 2. 定义了report_error函数，用于打印错误信息到系统日志和列表窗口。
> 3. 定义了宏WRITE、WRITE_D、WRITE_F、WRITE_E，用于在列表窗口中输出不同类型的数据。
> 4. 实现了ask_next_size_tolerance_feature函数，用于遍历部件中的所有公差特征，并找到下一个尺寸公差特征。
> 5. 在do_it函数中，调用ask_next_size_tolerance_feature获取尺寸公差特征，并使用UF_GDT_ask_size_tolerance_parms查询尺寸公差的参数，包括公称值、上限公差、下限公差等，并输出到列表窗口。
> 6. ufusr函数是NX的回调函数，在其中调用do_it函数，实现主要功能。
> 7. ufusr_ask_unload函数返回立即卸载，表示在NX退出时可以立即卸载该二次开发程序。
>
> 总体来说，这段代码实现了在NX中遍历尺寸公差特征，并输出尺寸公差参数的功能，通过NX Open API实现与NX的交互，展示了NX二次开发的基本流程。
>
