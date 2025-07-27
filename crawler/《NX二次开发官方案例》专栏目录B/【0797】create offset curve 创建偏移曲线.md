### 【0797】create offset curve 创建偏移曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_modl_types.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static void do_it(void)  
    {  
        int  
            num_curves;  
        tag_t  
            *offset_curves;  
        double  
            base_pt[3],  
            dist = 3.0,  
            draft_vec[3],  
            offset_vec[3];  
        char  
            dist_str[30];  
        UF_STRING_t  
            curvs;  
        UF_CURVE_offset_data_t  
            offset_data;  
        UF_CURVE_offset_distance_data_t  
            distance_data;  
        if (!specify_string("Offset curve",  &curvs)) return;  
        UF_CALL( UF_CURVE_ask_offset_direction_2( &curvs, offset_vec, draft_vec,  
            base_pt));  
        UF_DISP_refresh();  
        UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, base_pt, offset_vec, 0, "+ DIR");  
        if (!prompt_for_a_number("Offset", "Distance", &dist)) return;  
        sprintf(dist_str, "%f", dist);  
        strip_trailing_zeros(dist_str);  
        offset_data.input_curves              = &curvs;  
        offset_data.approximation_tolerance   = .001;  
        UF_MODL_ask_distance_tolerance(&offset_data.string_tolerance);  
        offset_data.offset_type               = UF_CURVE_OFFSET_DISTANCE_NO_TRIM;  
        offset_data.offset_def.distance_type1 = &distance_data;  
        distance_data.distance = dist_str;  
        distance_data.rough_type = 1;  
        UF_CALL(UF_CURVE_create_offset_curve(&offset_data, &num_curves,  
            &offset_curves));  
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

> 这段代码是NX Open C++ API编写的二次开发程序，主要用于实现曲线偏移功能。
>
> 主要功能包括：
>
> 1. 提示用户选择要偏移的曲线，并将这些曲线存储在一个字符串变量中。
> 2. 提示用户指定偏移方向，并计算偏移向量。
> 3. 提示用户输入偏移距离。
> 4. 根据用户输入的偏移距离和偏移方向，调用UF_CURVE_create_offset_curve函数创建偏移曲线。
> 5. 输出错误信息，设置选择掩码等辅助功能。
> 6. 提供ufusr和ufusr_ask_unload函数作为程序入口。
>
> 该程序使用了UF_CURVE模块的函数来实现曲线偏移功能，通过交互方式获取用户输入，并调用UF函数来创建偏移曲线。
>
