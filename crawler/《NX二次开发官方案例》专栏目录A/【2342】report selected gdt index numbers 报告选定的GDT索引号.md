### 【2342】report selected gdt index numbers 报告选定的GDT索引号

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_GDT_INDEX_NUMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_gdt.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您的要求，以下是对翻译的优化结果：

翻译优化结果：

UF_print_syslog是在V18版本中新增的功能。 */  
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
    static int mask_gdt(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            triples = { UF_tol_feature_instance_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &triples)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_gdt(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a GDT", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_gdt, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
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
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        int  
            tol_feat_index,  
            fcf_index,  
            ii = 0,  
            n_fcfs;  
        tag_t  
            *fcfs,  
            instance,  
            tol_feat;  
        double  
            origin[3];  
        while ((instance = select_a_gdt("Select a GDT instance")) != NULL_TAG)  
        {  
            WRITE_D(instance);  
            if (!UF_CALL(UF_DRF_ask_origin(instance, origin)))  
            {  
                WRITE3F(origin);  
                display_temporary_colored_point(origin, UF_OBJ_YELLOW);  
            }  
            UF_CALL(UF_GDT_ask_tol_feat_of_instance(instance, &tol_feat));  
            WRITE_D(tol_feat);  
            if (!UF_CALL(UF_GDT_ask_tolerance_index(tol_feat, &tol_feat_index)))  
                WRITE_D(tol_feat_index);  
            UF_CALL(UF_GDT_ask_fcf_tags(tol_feat, &n_fcfs, &fcfs));  
            WRITE_D(n_fcfs);  
            for (ii = 0; ii < n_fcfs; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(fcfs[ii]);  
                if (!UF_CALL(UF_GDT_ask_tolerance_index(fcfs[ii], &fcf_index)))  
                    WRITE_D(fcf_index);  
            }  
            if (n_fcfs > 0) UF_free(fcfs);  
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

> 这段代码是一个NX Open API的二次开发示例，主要功能是选择GDT（Geometric Dimensioning and Tolerancing）实例，并报告其特征实例、公差特征索引、以及相关FCF（Feature Control Frame）的数量和索引。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件。
> 2. 定义一个错误报告函数report_error。
> 3. 定义一个函数mask_gdt，用于设置选择掩码，只选择特征实例。
> 4. 定义一个函数select_a_gdt，用于选择一个GDT实例。
> 5. 定义一些宏，用于向列表窗口写入整型和数组型数据。
> 6. 定义一个函数display_temporary_colored_point，用于显示一个临时彩色点。
> 7. 定义一个函数do_it，用于循环选择GDT实例，并报告其信息。
> 8. 定义ufusr函数，作为程序的入口。
> 9. 定义ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总体来说，这段代码通过NX Open API实现了选择GDT实例并报告相关信息的功能，展示了一些常用的API调用方法，如UF_UI_select_with_single_dialog、UF_GDT_ask_fcf_tags等。
>
