### 【2389】report text orientation 报告文本方向

#### 代码

```cpp
    /*HEAD REPORT_TEXT_ORIENTATION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_csys.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
    #define ECHOF(X)   (printf("\t%s = %f\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是一个新功能，在V18版本中首次引入。文档只要求翻译这个描述，而不需要其他任何解释或评论。因此，UF_print_syslog 在V18中是新增的。 */  
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
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%f", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            matrix,  
            object;  
        double  
            mpr[70],  
            orientation[9];  
        char  
            rad[27],  
            dia[27];  
        while ((object = select_a_dim_or_draft("Select object")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
            ECHO(mpi[9]);  
            ECHOF(mpr[3]);  
            if (mpi[9] == 0)  
            {  
                UF_CALL(UF_CSYS_ask_matrix_of_object(object, &matrix));  
                UF_CALL(UF_CSYS_ask_matrix_values(matrix, orientation));  
                report_double_array("orientation", orientation, 3, 3);  
            }  
        }  
    }  
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

> 这段代码是一个NX二次开发示例，其主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义了一个选择过滤函数mask_for_dim_or_draft，用于在单选对话框中过滤出图元和尺寸。
> 3. 定义了一个选择函数select_a_dim_or_draft，用于弹出单选对话框，让用户选择一个图元或尺寸。
> 4. 定义了一个报告函数report_double_array，用于打印一个二维double数组。
> 5. 定义了主要函数do_it，用于循环让用户选择图元或尺寸，并获取其显示参数，打印尺寸文本位置或图元方向矩阵。
> 6. 定义了ufusr函数，作为二次开发程序的入口函数，在其中调用do_it函数执行主要功能。
> 7. 定义了ufusr_ask_unload函数，用于在程序卸载时立即卸载。
>
> 通过这些函数的协作，该程序实现了让用户选择图元或尺寸，并获取其显示参数的功能，打印尺寸文本位置或图元方向矩阵，以帮助用户了解图元或尺寸的显示设置。
>
