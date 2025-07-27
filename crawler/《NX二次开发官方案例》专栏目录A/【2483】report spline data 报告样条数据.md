### 【2483】report spline data 报告样条数据

#### 代码

```cpp
    /*HEAD REPORT_SPLINE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的功能。

因此，对于此问题的回答是：

UF_print_syslog 是 V18 新增的功能。 */  
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
    static int mask_for_splines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_BCURVE_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_spline(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_splines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_spline_data(tag_t spline)  
    {  
        int  
            ii;  
        char  
            msg[300] = { "" };  
        UF_CURVE_spline_t  
            spline_data;  
        UF_CALL(UF_CURVE_ask_spline_data(spline, &spline_data));  
        sprintf(msg, "spline = %d\n", spline);  
        WRITE( msg );  
        sprintf(msg, "  order = %d\n",spline_data.order);  
        WRITE( msg );  
        switch (spline_data.is_rational)  
        {  
            case RATIONAL_NO:  
                sprintf(msg, "  rational = no\n");  
                WRITE( msg );  
                break;  
            case RATIONAL_YES:  
                sprintf(msg, "  rational = yes\n");  
                WRITE( msg );  
                break;  
            default:  
                sprintf(msg, "  rational = Unknown\n");  
            WRITE( msg );  
        }  
        sprintf(msg, "  start parameter = %f\n", spline_data.start_param);  
        WRITE( msg );  
        sprintf(msg, "  end parameter = %f\n",spline_data.end_param);  
        WRITE( msg );  
        sprintf(msg, "  number of knots = %d\n",  
            spline_data.num_poles + spline_data.order);  
        WRITE( msg );  
        for (ii = 0; ii < spline_data.num_poles + spline_data.order; ii++)  
        {  
             sprintf(msg, "    %f\n", ii+1,spline_data.knots[ii]);  
             WRITE( msg );  
        }  
        sprintf(msg, "  number of poles = %d\n",spline_data.num_poles);  
        WRITE( msg );  
        for(ii = 0; ii < spline_data.num_poles; ii++)  
        {  
            sprintf(msg, "    %-8f, %-8f, %-8f, %-8f\n",  
                spline_data.poles[ii][0], spline_data.poles[ii][1],  
                spline_data.poles[ii][2], spline_data.poles[ii][3]);  
            WRITE( msg );  
        }  
        UF_free(spline_data.knots);  
        UF_free(spline_data.poles);  
    }  
    static void do_it(void)  
    {  
        tag_t     
            spline;  
        while((spline = select_a_spline("Select spline")) != NULL_TAG)  
            report_spline_data(spline);  
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

> 根据代码内容，这是使用NX Open C API编写的NX二次开发示例代码，其主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息到日志窗口。
> 2. 定义了一个选择过滤函数mask_for_splines，用于在选择对话框中过滤只显示样条曲线。
> 3. 定义了一个选择样条曲线函数select_a_spline，用于通过选择对话框让用户选择一个样条曲线，并返回其tag。
> 4. 定义了一个报告样条数据函数report_spline_data，用于查询并打印指定样条曲线的详细信息，包括阶数、是否为有理样条、起始和结束参数、结点数和位置、控制点数和位置等。
> 5. 定义了一个主函数do_it，用于循环选择样条曲线并报告其数据，直到用户取消选择。
> 6. 定义了ufusr入口函数，用于初始化NX环境，调用主函数，然后终止NX环境。
> 7. 定义了一个卸载函数ufusr_ask_unload，用于在卸载ufusr时立即清理资源。
>
> 总的来说，这段代码的主要功能是让用户可以方便地选择并查看NX模型中的样条曲线的详细参数信息。
>
