### 【0432】ask point along spline 询问曲线上的点

#### 代码

```cpp
    /*HEAD ASK_POINT_ALONG_SPLINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 新增的。 */  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    /* 里海 */  
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
    /* 里海 */  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_splines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static void ask_curve_point(tag_t curve, double point[3], double p_parm)  
    {  
        double  
            limits[2];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        UF_CALL(UF_EVAL_evaluate(eval, 0, ( (limits[1] - limits[0]) * p_parm),  
                                                                  point, NULL));  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
            UF_DISP_USE_ACTIVE_PLUS, point, &attrib, UF_DISP_POINT));  
        UF_CALL(UF_EVAL_free(eval));  
    }  
    /* 里海 */  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
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
            my_spline = NULL_TAG;  
        double  
            point_on_curve[3] = { 0., 0., 0. },  
            parm = 0.;  
        while( ((my_spline = select_a_spline( "Pick a Spline:" )) != NULL_TAG) &&  
               (prompt_for_a_number("Unitized Parameter?","Unitized Parm?", &parm)  
                                                                           == TRUE)  
             )  
        {  
            ask_curve_point( my_spline, point_on_curve, parm );  
            WRITE3F( point_on_curve );  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_set_retiring_flag( 2 ); /* notify about retiring functions 译:通知关于废弃功能 */  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++ API开发的用户函数，其主要功能是：
>
> 1. 提供一个对话框，让用户选择一个样条曲线。
> 2. 提示用户输入一个单位化参数，该参数表示样条曲线上点的位置。
> 3. 根据用户输入的参数，计算样条曲线上的点，并在NX视图中高亮显示该点。
> 4. 重复执行上述步骤，直到用户取消选择样条或取消输入参数。
> 5. 输出计算得到的点的坐标。
> 6. 使用UF_PRINT_SYLOG宏函数输出错误信息，以便调试。
> 7. 使用UF_DISP库显示临时点。
> 8. 使用UF_EVAL库计算样条曲线上的点。
> 9. 使用UF_UI库提供用户交互界面。
> 10. 使用UF_OBJ库获取样条曲线的属性。
> 11. 使用UF_CALL宏函数简化错误处理。
> 12. 提供卸载函数。
>
> 该函数为用户提供了一个简单而直观的界面，用于在样条曲线上指定点，并查看该点的坐标。
>
