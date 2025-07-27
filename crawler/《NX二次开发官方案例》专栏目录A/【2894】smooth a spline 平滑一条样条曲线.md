### 【2894】smooth a spline 平滑一条样条曲线

#### 代码

```cpp
    /*HEAD SMOOTH_A_SPLINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define WRITE(X) ECHO(X)  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_an_integer_and_two_reals(char *prompt, char *item1,  
        char *item2, char *item3, int *number1, double *number2, double *number3)  
    {  
        int  
            ia[3],  
            ip[3] = { 101, 201, 201 },  
            resp;  
        double  
            da[3];  
        char  
            ca[3][31],  
            menu[3][16];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        strcpy(&menu[2][0], item3);  
        ia[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1613(prompt, menu, 3, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = ia[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        ECHO(msg);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE3F(X)(write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    static void report_curve_states(int num_states, UF_CURVE_state_t *states)  
    {  
        int  
            ii;  
        WRITENZ(num_states);  
        for (ii = 0; ii < num_states; ii++)  
        {  
            WRITE_D(ii);  
            switch (states[ii].state_code)  
            {  
                case UF_CURVE_STATE_DEGENERACY:  
                    WRITE_D(UF_CURVE_STATE_DEGENERACY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE(" near ");  
                    WRITE3F(states[ii].value);  
                    break;  
                case UF_CURVE_STATE_G1_DISCONTINUITY:  
                    WRITE_D(UF_CURVE_STATE_G1_DISCONTINUITY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_KNOT_NONC0:  
                    WRITE_D(UF_CURVE_STATE_KNOT_NONC0);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_KNOT_MULT:  
                    WRITE_D(UF_CURVE_STATE_KNOT_MULT);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_KNOT_DECREASING:  
                    WRITE_D(UF_CURVE_STATE_KNOT_DECREASING);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_CLOSED:  
                    WRITE_D(UF_CURVE_STATE_CLOSED);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_CURVE_STATE_PERIODIC:  
                    WRITE_D(UF_CURVE_STATE_PERIODIC);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_CURVE_STATE_KNOT_TOOCLOSE:  
                    WRITE_D(UF_CURVE_STATE_KNOT_TOOCLOSE);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_NOT_TRIMMED:  
                    WRITE_D(UF_CURVE_STATE_PERIODIC);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_CURVE_STATE_UNNORMALIZED:  
                    WRITE_D(UF_CURVE_STATE_UNNORMALIZED);  
                    if (states[ii].flag & 2) { WRITE("Along V factor "); }  
                    else { WRITE("Along U factor "); }  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_CLAMPED:  
                    WRITE_D(UF_CURVE_STATE_CLAMPED);  
                    WRITE("Knot value ");  
                    WRITE_F(states[ii].value[0]);  
                    WRITE("minimum distance change ");  
                    WRITE_F(states[ii].value[1]);  
                    WRITE("maximum angle change ");  
                    WRITE_F(states[ii].value[2]);  
                    break;  
                case UF_CURVE_STATE_SMOOTHED_TO_CN:  
                    WRITE_D(UF_CURVE_STATE_SMOOTHED_TO_CN);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                default:  
                    WRITE("Unknown state ");  
                    WRITE_D(states[ii].state_code);  
                    break;  
            }  
        }  
        if (num_states > 0) UF_free(states);  
    }  
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        logical  
            good = FALSE;  
        int  
            cont_order,  
            num_states;  
        tag_t  
            new_spline,  
            spline;  
        double  
            a_tol,  
            d_tol;  
        UF_CURVE_state_t  
            *states;  
        UF_CURVE_spline_t  
            spline_data;  
        UF_MODL_ask_distance_tolerance(&d_tol);  
        UF_MODL_ask_angle_tolerance(&a_tol);  
        while((spline = select_a_spline("Select spline to smooth")) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_spline_data(spline, &spline_data));  
            cont_order = 1;  
            good = FALSE;  
            while (!good && prompt_for_an_integer_and_two_reals(  
                "Smooth spline parameters", "continuity", "dist tol", "angle tol",  
                &cont_order, &d_tol, &a_tol))  
            {  
                UF_CALL(UF_CURVE_fix_spline_data(&spline_data, d_tol, &num_states,  
                    &states));  
                report_curve_states(num_states, states);  
                UF_CALL(UF_CURVE_smooth_spline_data(&spline_data, cont_order,  
                    d_tol, a_tol, &num_states, &states));  
                report_curve_states(num_states, states);  
                UF_CALL(UF_CURVE_fix_spline_data(&spline_data, d_tol, &num_states,  
                    &states));  
                report_curve_states(num_states, states);  
                UF_CALL(UF_CURVE_create_spline(&spline_data, &new_spline,  
                    &num_states, &states));  
                report_curve_states(num_states, states);  
                UF_CALL(UF_DISP_set_highlight(new_spline, TRUE));  
                good = TRUE;  
                if (ask_yes_or_no("OK?", &good) && good)  
                {  
                    set_undo_mark("Smooth spline");  
                    UF_CALL(UF_DISP_set_highlight(new_spline, FALSE));  
                    UF_CALL(UF_OBJ_replace_object_data(spline, new_spline));  
                }  
                else  
                    UF_OBJ_delete_object(new_spline);  
            }  
            UF_free( spline_data.knots );  
            UF_free( spline_data.poles );  
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

> 这段代码是用于NX的二次开发，其主要功能是平滑曲线。代码主要包含以下部分：
>
> 1. 头文件包含：引入了NX提供的UF库的必要头文件，包括通用UF、UI、对象类型、对象、曲线、模型、显示等。
> 2. 错误报告宏：定义了用于报告UF函数调用错误的宏。
> 3. 选择曲线函数：用于选择一个曲线，并返回其tag。
> 4. 提示输入参数函数：用于提示用户输入平滑参数。
> 5. 报告曲线状态函数：用于报告曲线的状态信息。
> 6. 设置撤销标记函数：用于设置撤销标记。
> 7. 主要逻辑函数：包括读取曲线数据，设置平滑参数，报告曲线状态，平滑曲线，创建新曲线，询问用户是否确认，并替换旧曲线。
> 8. ufusr函数：用于初始化NX环境，执行主要逻辑，并清理环境。
> 9. 卸载询问函数：用于询问是否立即卸载。
>
> 总的来说，这段代码实现了曲线平滑的功能，用户可以交互式地选择曲线，设置平滑参数，查看状态，确认后替换原曲线。代码结构清晰，注释详细，便于理解和使用。
>
