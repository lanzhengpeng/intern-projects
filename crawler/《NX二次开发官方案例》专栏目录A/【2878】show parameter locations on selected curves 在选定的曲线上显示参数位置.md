### 【2878】show parameter locations on selected curves 在选定的曲线上显示参数位置

#### 代码

```cpp
    /*HEAD SHOW_PARAMETER_LOCATIONS_ON_SELECTED_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的注释，UF_print_syslog 是 V18 版本中新增的功能，文档要求仅提供翻译，不添加任何其他内容。因此，针对该注释的翻译为：

UF_print_syslog 是 V18 版本新增的功能。 */  
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
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
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
            deriv_flag = UF_MODL_LOC;  
        tag_t  
            curve;  
        double  
            junk[3],  
            parm = 0.5,  
            position[3];  
        UF_EVAL_p_t  
            evaluator;  
        while ((curve = select_a_curve("Show parameter position")) != NULL_TAG)  
            while (prompt_for_a_number("Enter value", "parameter", &parm))  
            {  
            /*  These functions take normalized parameters (0.0 to 1.0) 里海译:这些函数采用标准化参数（0.0到1.0）。 */  
                UF_CALL(UF_MODL_ask_curve_props(curve, parm, position, junk,  
                    junk, junk, junk, junk));  
                display_temporary_colored_point(position, UF_OBJ_RED);  
                UF_CALL(UF_MODL_evaluate_curve(curve, &parm, &deriv_flag,  
                    position));  
                display_temporary_colored_point(position, UF_OBJ_YELLOW);  
            /*  These functions take non-normalized parameters - arcs and ellipses  
                are in radians */  
                UF_CALL(UF_CURVE_evaluate_curve(curve, parm, UF_CURVE_LOC,  
                    position));  
                display_temporary_colored_point(position, UF_OBJ_WHITE);  
                UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
                UF_CALL(UF_EVAL_evaluate(evaluator, 0, parm, position, NULL));  
                display_temporary_colored_point(position, UF_OBJ_GREEN);  
                UF_CALL(UF_EVAL_evaluate_unit_vectors(evaluator, parm,  
                    position, junk, junk, junk));  
                display_temporary_colored_point(position, UF_OBJ_BLUE);  
                UF_CALL(UF_EVAL_free(evaluator));  
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

> 这段代码是一个NX的二次开发示例，用于显示曲线上的参数位置。
>
> 主要功能包括：
>
> 1. 选择曲线：通过对话框让用户选择一个曲线，并高亮显示。
> 2. 输入参数值：提示用户输入一个参数值，范围是0到1。
> 3. 显示参数位置：根据用户输入的参数值，显示曲线在该参数值处的位置点。使用了两种方法计算位置，一种是使用UF_MODL模块的函数，另一种是使用UF_CURVE和UF_EVAL模块的函数。位置点分别用不同颜色显示。
> 4. 显示单位向量：计算曲线在该参数值处的单位向量，并用不同颜色显示。
> 5. 错误处理：使用UF_print_syslog函数打印错误信息。
> 6. 用户界面：使用了UF_UI模块进行用户交互。
> 7. 自动卸载：在函数结束时，设置了自动卸载标志。
>
> 总体来说，这段代码实现了选择曲线、输入参数值、显示参数位置和单位向量的功能，并提供了错误处理和用户交互。
>
