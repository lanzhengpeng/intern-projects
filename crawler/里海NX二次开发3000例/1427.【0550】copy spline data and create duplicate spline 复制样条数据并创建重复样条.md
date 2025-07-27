### 【0550】copy spline data and create duplicate spline 复制样条数据并创建重复样条

#### 代码

```cpp
    /*HEAD COPY_SPLINE_DATA_AND_CREATE_DUPLICATE_SPLINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。翻译如下：

翻译：UF_print_syslog是V18版本新增的函数，只回答译文，不要废话。 */  
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
    /* 里海 */  
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
    static void copy_spline_data(tag_t spline, UF_CURVE_spline_p_t copy_data)  
    {  
        int  
            ii, jj,  
            n_knots,  
            resp;  
        UF_CURVE_spline_t  
            spline_data;  
        UF_CALL(UF_CURVE_ask_spline_data(spline, &spline_data));  
        copy_data->num_poles = spline_data.num_poles;  
        copy_data->order = spline_data.order;  
        copy_data->is_rational = spline_data.is_rational;  
        n_knots = spline_data.num_poles + spline_data.order;  
        copy_data->knots = UF_allocate_memory(n_knots * sizeof(double), &resp);  
        for (ii = 0; ii < n_knots; ii++)  
            copy_data->knots[ii] = spline_data.knots[ii];  
        copy_data->poles = UF_allocate_memory(spline_data.num_poles *  
            4 * sizeof(double), &resp);  
        for (ii = 0; ii < spline_data.num_poles; ii++)  
            for (jj = 0; jj < 4; jj++)  
                copy_data->poles[ii][jj] = spline_data.poles[ii][jj];  
        copy_data->start_param = spline_data.start_param;  
        copy_data->end_param = spline_data.end_param;  
    }  
    static void do_it(void)  
    {  
        int  
            n_states;  
        tag_t  
            new_spline,  
            spline;  
        UF_CURVE_spline_t  
            data;  
        UF_CURVE_state_t  
            *states = NULL;  
        while((spline = select_a_spline("Select spline")) != NULL_TAG)  
        {  
            copy_spline_data(spline, &data);  
            UF_CALL(UF_CURVE_create_spline(&data, &new_spline, &n_states, &states));  
            UF_free(data.knots);  
            UF_free(data.poles);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：通过UF_CALL宏将UF函数调用封装，并在失败时调用report_error函数报告错误信息。
> 2. 选择蒙版：定义了mask_for_splines函数，用于设置选择蒙版，以选择曲面上的样条线。
> 3. 选择样条线：定义了select_a_spline函数，通过对话框让用户选择一个样条线，并高亮显示。
> 4. 复制样条线数据：定义了copy_spline_data函数，用于复制选中样条线的所有数据，包括顶点、节点、起始参数等。
> 5. 创建样条线：在do_it函数中，循环调用select_a_spline选择样条线，复制数据，并调用UF_CURVE_create_spline函数创建新的样条线。
> 6. 主函数：ufusr函数初始化UF，调用do_it函数，然后终止UF。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，这段代码通过二次开发实现了在NX中选择一个样条线，复制其数据并创建新的样条线的功能。
>
