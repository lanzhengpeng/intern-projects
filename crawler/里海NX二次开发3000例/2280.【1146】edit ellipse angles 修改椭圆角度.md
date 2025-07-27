### 【1146】edit ellipse angles 修改椭圆角度

#### 代码

```cpp
    /*HEAD EDIT_ELLIPSE_ANGLES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文意，UF_print_syslog 是 V18 中的新功能。 */  
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
    static int mask_for_ellipses(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_conic_type, UF_conic_ellipse_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_an_ellipse(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an Ellipse", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_ellipses, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
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
        tag_t  
            conic;  
        double  
            end_angle,  
            n_adj,  
            start_angle;  
        UF_CURVE_conic_t  
            conic_data;  
        while ((conic = select_an_ellipse("Edit Angles")) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_conic_data(conic, &conic_data));  
            start_angle = RADEG * atan( tan( conic_data.start_param) *  
                    (conic_data.k2 / conic_data.k1));  
            modf((conic_data.start_param + (PI/2))/PI, &n_adj);  
            start_angle = (n_adj * 180) + start_angle;  
            end_angle = RADEG * atan( tan( conic_data.end_param) *  
                    (conic_data.k2 / conic_data.k1));  
            modf((conic_data.end_param + (PI/2))/PI, &n_adj);  
            end_angle = (n_adj * 180) + end_angle;  
            if (prompt_for_two_numbers("Enter Ellipse parameters", "Start Angle", "End Angle",  
                &start_angle, &end_angle))  
            {  
                while (start_angle < 0) start_angle = 360 + start_angle;  
                conic_data.start_param = atan( tan( DEGRA * start_angle) *  
                    (conic_data.k1 / conic_data.k2));  
                modf((start_angle + 90)/180, &n_adj);  
                conic_data.start_param = (n_adj * PI) + conic_data.start_param;  
                while (end_angle < 0) end_angle = 360 + end_angle;  
                conic_data.end_param = atan( tan( DEGRA * end_angle ) *  
                    (conic_data.k1 / conic_data.k2));  
                modf((end_angle + 90)/180, &n_adj);  
                conic_data.end_param = (n_adj * PI) + conic_data.end_param;  
                if (end_angle < start_angle)  
                    conic_data.end_param = TWOPI + conic_data.end_param;  
                UF_CALL(UF_CURVE_edit_conic_data(conic, &conic_data));  
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

> 这段NX二次开发代码的主要功能是编辑椭圆的两个端角。
>
> 具体来说，代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了mask_for_ellipses函数，用于设置选择过滤器，以便只选择椭圆。
> 3. 定义了select_an_ellipse函数，用于提示用户选择一个椭圆，并返回其tag。
> 4. 定义了prompt_for_two_numbers函数，用于提示用户输入两个角度值。
> 5. 定义了do_it函数，用于循环执行以下操作：调用select_an_ellipse获取椭圆tag获取椭圆数据，并计算当前的两个端角调用prompt_for_two_numbers提示用户输入新的两个端角根据用户输入更新椭圆数据，并设置新的两个端角
> 6. 调用select_an_ellipse获取椭圆tag
> 7. 获取椭圆数据，并计算当前的两个端角
> 8. 调用prompt_for_two_numbers提示用户输入新的两个端角
> 9. 根据用户输入更新椭圆数据，并设置新的两个端角
> 10. 定义了ufusr函数，作为NX二次开发的入口函数。在UF_initialize和UF_terminate之间调用do_it函数。
> 11. 定义了ufusr_ask_unload函数，用于在NX关闭时卸载二次开发程序。
>
> 总的来说，这段代码实现了编辑椭圆端角的功能，并提供了友好的用户交互界面。
>
