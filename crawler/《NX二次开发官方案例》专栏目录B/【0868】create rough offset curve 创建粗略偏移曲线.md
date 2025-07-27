### 【0868】create rough offset curve 创建粗略偏移曲线

#### 代码

```cpp
    /*HEAD CREATE_ROUGH_OFFSET_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_modl_types.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数，用于打印系统日志消息。 */  
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
            if (curves[kk] != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(curves[kk], TRUE));  
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
                UF_CALL(UF_DISP_set_highlight(curves[ii], FALSE));  
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
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
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
            *offset_curves;  
        double  
            base_pt[3],  
            dist = 1.0,  
            draft_vec[3],  
            offset_vec[3];  
        char  
            distance[20];  
        UF_STRING_t  
            curvs;  
        UF_CURVE_offset_data_t  
            offset_data;  
        UF_CURVE_offset_distance_data_t  
            distance_data;  
        offset_data.input_curves              = &curvs;  
        UF_MODL_ask_distance_tolerance(&offset_data.approximation_tolerance);  
        UF_MODL_ask_distance_tolerance(&offset_data.string_tolerance);  
        offset_data.offset_type               = UF_CURVE_OFFSET_DISTANCE_FILLET;  
        offset_data.offset_def.distance_type1 = &distance_data;  
        distance_data.distance = distance;  
        distance_data.rough_type = 1;  
        while (specify_string("Offset curve", &curvs) &&  
                !UF_CALL(UF_CURVE_ask_offset_direction(&curvs, offset_vec,  
                    draft_vec, base_pt)) &&  
               prompt_for_a_number("Offset", "Distance", &dist))  
        {  
            set_undo_mark("Create Rough Offset");  
            sprintf(distance, "%f", dist);  
            strip_trailing_zeros(distance);  
            UF_CALL(UF_CURVE_create_offset_curve(&offset_data, &num_curves,  
                &offset_curves));  
            if (num_curves > 0) UF_free(offset_curves);  
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

> 这段代码是一个NX Open C++ API开发的宏，主要功能是创建曲线的粗略偏移。以下是代码的主要部分：
>
> 1. 头文件包含：包含了必要的NX Open C++ API头文件。
> 2. 错误报告函数：定义了report_error函数，用于在发生错误时报告错误信息。
> 3. 曲线选择函数：定义了mask_for_curves和select_a_curve函数，用于选择一条曲线。
> 4. 字符串指定函数：定义了specify_string函数，用于连续选择多条曲线形成字符串。
> 5. 参数提示函数：定义了prompt_for_a_number函数，用于提示用户输入偏移距离。
> 6. 零尾处理函数：定义了strip_trailing_zeros函数，用于去除字符串尾部的零。
> 7. 撤销标记设置函数：定义了set_undo_mark函数，用于设置撤销标记。
> 8. 主要功能实现：在do_it函数中实现了主要功能，包括提示用户选择曲线、设置偏移方向、提示输入偏移距离、创建偏移曲线等。
> 9. 宏入口函数：定义了ufusr函数，作为宏的入口函数，在NX中加载运行。
> 10. 卸载函数：定义了ufusr_ask_unload函数，用于设置宏的卸载方式。
>
> 总的来说，这段代码通过NX Open C++ API实现了曲线的粗略偏移功能，用户可以连续选择多条曲线，设置偏移距离和方向，以创建曲线的偏移副本。
>
