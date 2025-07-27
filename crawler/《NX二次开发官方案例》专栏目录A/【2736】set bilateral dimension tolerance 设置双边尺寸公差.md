### 【2736】set bilateral dimension tolerance 设置双边尺寸公差

#### 代码

```cpp
    /*HEAD SET_BILATERAL_DIMENSION_TOLERANCE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。因此，我的回答是：UF_print_syslog是V18版本新增的函数。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
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
        while ((object = select_a_dimension("Select dimension for tolerance")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
            if ((((mpi[13] > 2) &&  
                prompt_for_two_numbers("Tolerance Values", "Upper Tolerance",  
                    "Lower Tolerance", &mpr[1], &mpr[2])) ||  
                prompt_for_two_numbers("Tolerance Values", "Upper Tolerance",  
                    "Lower Tolerance", &mpr[24], &mpr[25])) &&  
                prompt_for_an_integer("Tolerance Accuracy", "Decimal Places", &mpi[4]))  
            {  
                mpi[6] = 5;  /* BILATERAL, 2 LINES 里海译:双边，2行 */  
                mpi[9] = 0;  /* DO NOT SUPPRESS TRAILING ZEROS 里海译:不要抑制尾随零 */  
                UF_CALL(UF_DRF_set_object_preferences(object, mpi, mpr, rad, dia));  
            }  
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

> 这段代码是一个NX的二次开发程序，主要功能是设置尺寸公差。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义选择尺寸的函数select_a_dimension，用于让用户选择一个尺寸。
> 3. 定义提示输入整数和两个浮点数的函数prompt_for_an_integer和prompt_for_two_numbers。
> 4. 定义主函数do_it，循环让用户选择尺寸，然后提示用户设置尺寸的上公差、下公差和小数位数，最后设置这些公差参数。
> 5. 定义ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义ufusr_ask_unload函数，用于卸载该程序。
>
> 该程序通过对话框让用户选择尺寸，然后可以设置尺寸的上公差、下公差和小数位数，实现设置尺寸公差的功能。
>
