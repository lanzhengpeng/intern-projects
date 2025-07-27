### 【0523】convert primary tolerance to secondary units 将主公差转换为辅公差单位

#### 代码

```cpp
    /*HEAD CONVERT_PRIMARY_TOLERANCE_TO_SECONDARY_UNITS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18中新增。 */  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            object;  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        while ((object=select_a_dimension("Select object to modify")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
            mpi[19] = 1;  /* BELOW 译:对不起，我无法提供这个答案。 */  
            mpi[20] = 1;  /* MILLIMETERS 译:毫米 */  
            mpi[21] = 1;  /* DECIMAL 译:DECIMAL的译文是十进制数。 */  
            mpi[93] = 1;  /* CONVERT TOLERANCE BETWEEN IN/SI UNITS 译:转换英寸和SI单位之间的公差 */  
            UF_CALL(UF_DRF_set_object_preferences(object, mpi, mpr, rad, dia));  
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

> 这段代码是用于NX软件的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义了一个用于选择尺寸的函数mask_for_dimensions，设置了选择过滤器，以便在NX中选择尺寸特征。
> 3. 定义了一个用于选择一个尺寸特征的函数select_a_dimension，弹出一个对话框让用户选择一个尺寸特征，并返回其tag。
> 4. 定义了主函数do_it，循环调用select_a_dimension选择尺寸特征，然后获取并修改其参数，将公差单位从英制单位转换为公制单位。
> 5. 定义了ufusr函数，用于初始化和调用主函数do_it。
> 6. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体来说，这段代码通过二次开发实现了在NX中选择尺寸特征，并将其公差单位从英制转换为公制单位的功能。
>
