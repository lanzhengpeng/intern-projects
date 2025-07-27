### 【0458】change degree of selected spline 修改选择曲线的度数

#### 代码

```cpp
    /*HEAD CHANGE_DEGREE_OF_SELECTED_SPLINE CCC UFUN */  
    /*  
        There is no API equivalent to Edit-> Curve-> Edit Spline-> Change Degree  
        so this program demonstrates calling a GRIP program to make this  
        modification.  
        The corresponding GRIP program "change_degree.grs" looks like this:  
            ENTITY/ obj  
            UFARGS/ obj, n  
            &BSDATA(obj,DEGREE) = n  
            HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a spline", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_splines, NULL, &resp,  
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
    static void do_it(void)  
    {  
        tag_t  
            spline;  
        int  
            new_degree;  
        double  
            degree;  
        UF_EVAL_p_t  
            eval;  
        UF_EVAL_spline_t  
            spline_eval;  
        UF_args_t  
            ufargs[2];  
        ufargs[0].type = UF_TYPE_TAG_T;  
        ufargs[0].length = 0;  
        ufargs[0].address = &spline;  
        ufargs[1].type = UF_TYPE_DOUBLE;  
        ufargs[1].length = 0;  
        ufargs[1].address = &degree;  
        while ((spline = select_a_spline("Change Degree")) != NULL_TAG)  
        {  
            UF_CALL(UF_EVAL_initialize(spline, &eval));  
            UF_CALL(UF_EVAL_ask_spline(eval, &spline_eval));  
            UF_CALL(UF_EVAL_free(eval));  
            new_degree = spline_eval.order - 1;  
            if (prompt_for_an_integer("Enter spline parameter", "Degree",  
                &new_degree))  
            {  
                degree = (double)new_degree;  
            /*  change this line to point to the grx file 译:将这行代码改为指向grx文件 */  
                UF_CALL(UF_call_grip("grip/change_degree.grx", 2, ufargs));  
            }  
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

> 根据代码的注释和逻辑，这段代码的主要功能是调用一个GRIP程序来改变选中样条曲线的阶数。
>
> 关键要点包括：
>
> 1. 定义了一个名为report_error的函数，用于报告错误。
> 2. 定义了一个名为mask_for_splines的函数，用于设置选择掩码，只选择样条曲线。
> 3. 定义了一个名为select_a_spline的函数，用于弹出对话框让用户选择一个样条曲线。
> 4. 定义了一个名为prompt_for_an_integer的函数，用于弹出一个对话框让用户输入一个整数。
> 5. 在do_it函数中，循环调用select_a_spline获取用户选择的样条曲线，然后获取其阶数，通过prompt_for_an_integer让用户输入新的阶数，最后调用UF_call_grip调用GRIP程序change_degree.grx来改变样条曲线的阶数。
> 6. 在ufusr函数中调用do_it函数来实现上述功能。
> 7. 在ufusr_ask_unload函数中返回UF_UNLOAD_IMMEDIATELY，表示在NX关闭时会立即卸载该代码。
>
> 总的来说，这段代码通过GRIP程序实现了改变样条曲线阶数的功能，用户交互友好，通过对话框进行选择和输入，使用GRIP程序进行底层操作。
>
