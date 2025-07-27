### 【0556】create 3D axial offset curve 创建三维轴向偏移曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
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
    static logical specify_string(char *which, UF_STRING_t *string)  
    {  
        int  
            ii = 0,  
            jj = 0,  
            kk = -1;  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        do  
        {  
            if (jj > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s string", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            sprintf(prompt, "Select %s String - curve #%d", which, ++jj);  
            curves[++kk] = select_a_curve(prompt);  
        } while ((curves[kk] != NULL_TAG) && (kk < 50));  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            UF_MODL_init_string_list(string);  
            UF_MODL_create_string_list(1, kk, string);  
            string->string[0] = kk;  
            string->dir[0] = 1;  
            for (ii = 0; ii < kk; ii++)  
            {  
                string->id[ii] = curves[ii];  
                UF_CALL(UF_DISP_set_highlight(curves[ii], 0));  
            }  
            return TRUE;  
        }  
        return FALSE;  
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
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = (int)strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
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
        int  
            num_curves;  
        tag_t  
            *offset_curves,  
            offset_feat;  
        double  
            base_pt[3],  
            dist = 1.0,  
            offset_vec[3];  
        char  
            dist_str[30];  
        UF_STRING_t  
            curvs;  
        UF_CURVE_offset_data_t  
            offset_data;  
        UF_CURVE_offset_axial_data_t  
            axial_data;  
        offset_data.input_curves              = &curvs;  
        offset_data.approximation_tolerance   = .001;  
        UF_MODL_ask_distance_tolerance(&offset_data.string_tolerance);  
        offset_data.offset_type = UF_CURVE_OFFSET_3D_AXIAL;  
        offset_data.offset_def.axial_type = &axial_data;  
        axial_data.offset_value = dist_str;  
        while (specify_string("curves to offset",  &curvs) &&  
            prompt_for_a_number("3D Axial Offset", "Distance", &dist) &&  
            specify_vector("3D Axial Offset direction", offset_vec, base_pt))  
        {  
            set_undo_mark("3D Axial Offset Curve");  
            sprintf(dist_str, "%f", dist);  
            strip_trailing_zeros(dist_str);  
            UF_CALL(UF_SO_create_dirr_doubles(curvs.id[0], UF_SO_dont_update,  
                offset_vec, &axial_data.axis_vector));  
            if (ask_yes_or_no("Axial Offset", "Associative?"))  
                // The offset direction is ignored - see PR 6642982  
                UF_CALL(UF_CURVE_create_offset_object(&offset_data, &offset_feat));  
            else  
                // Prior to NX7.5.4, the offset direction is ignored - see PR 2140851  
                UF_CALL(UF_CURVE_create_offset_curve(&offset_data, &num_curves,  
                    &offset_curves));  
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

> 根据代码内容，这是一个NX Open C++开发的二次开发应用程序，其主要功能是实现对曲线进行3D轴向偏移。
>
> 主要功能包括：
>
> 1. 提供了错误处理函数ECHO和UF_CALL，用于输出错误信息。
> 2. 提供了mask_for_curves函数，用于设置选择掩码，只允许选择直线、圆、圆锥曲线和样条曲线。
> 3. 提供了select_a_curve函数，用于弹出一个对话框，让用户选择一条曲线。
> 4. 提供了specify_string函数，用于提示用户选择多条曲线，并保存为字符串。
> 5. 提供了prompt_for_a_number函数，用于提示用户输入一个数字。
> 6. 提供了specify_vector函数，用于提示用户指定一个向量。
> 7. 提供了set_undo_mark函数，用于设置撤销标记。
> 8. 提供了do_it函数，实现了主要功能，包括提示用户选择曲线、输入偏移距离、指定偏移方向，并调用UF_CURVE_create_offset_object或UF_CURVE_create_offset_curve函数进行曲线偏移。
> 9. 提供了ufusr函数，作为程序的入口函数。
> 10. 提供了ufusr_ask_unload函数，用于处理卸载请求。
>
> 综上所述，该代码实现了对曲线进行3D轴向偏移的功能，用户可以通过对话框选择曲线、输入偏移距离和指定偏移方向，程序会根据用户输入生成偏移曲线。
>
