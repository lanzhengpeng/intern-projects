### 【2363】report selected section surface feature data 报告选定的截面曲面特征数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    #define WRITE_SL(X,Y) report_string_list_info(#X, X, Y)  
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void show_curve_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            start[3],  
            tangent[3];  
        if (UF_MODL_ask_curve_props(curve, 0.0, start, tangent, junk, junk,  
                junk, junk)) return;  
        display_colored_conehead(start, tangent, UF_OBJ_RED);  
    }  
    static void show_curve_flip_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            end[3],  
            flip_dir[3],  
            tangent[3];  
        if (UF_MODL_ask_curve_props(curve, 1.0, end, tangent, junk, junk,  
                junk, junk)) return;  
        UF_VEC3_negate(tangent, flip_dir);  
        display_colored_conehead(end, flip_dir, UF_OBJ_RED);  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void report_string_list_info(char *name, UF_STRING_t *string,  
        logical highlight)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        if (string == NULL)  
        {  
            ECHO("%s = NULL\n", name);  
            return;  
        }  
        ECHO("%s->num = %d\n", name, string->num);  
        for (ii = 0; ii < string->num; ii++)  
        {  
            ECHO("%s->string[%d] = %d\n", name, ii, string->string[ii]);  
            ECHO("%s->dir[%d] = %d\n", name, ii, string->dir[ii]);  
            if (string->dir[ii] == UF_MODL_CURVE_START_FROM_BEGIN)  
                show_curve_direction(string->id[kk]);  
            else  
                show_curve_flip_direction(string->id[kk]);  
            for (jj = 0; jj < string->string[ii]; jj++)  
            {  
                sprintf(msg, "%d", kk);  
                if (UF_OBJ_ask_name_origin(string->id[kk], loc))  
                {  
                    UF_CALL(UF_OBJ_set_name(string->id[kk], "X"));  
                    UF_CALL(UF_OBJ_ask_name_origin(string->id[kk], loc));  
                    UF_CALL(UF_OBJ_delete_name(string->id[kk]));  
                }  
                display_temporary_text(msg, loc);  
                ECHO("%s->id[%d] = %d\n", name, kk, string->id[kk]);  
                if (highlight) UF_CALL(UF_DISP_set_highlight(string->id[kk], TRUE));  
                kk++;  
            }  
            if (highlight)  
            {  
                sprintf(msg, "%d string curves highlighted", string->string[ii]);  
                uc1601(msg, TRUE);  
                UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
            }  
        }  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    static void report_secsrf_param(char *name, UF_MODL_secsrf_param_p_t param)  
    {  
        ECHO("%s.data_method = %d\n", name, param->data_method);  
        ECHO("%s.data_values.data_constant = %f\n", name, param->data_values.data_constant);  
        ECHO("%s.data_values.data_exp[0] = %d\n", name, param->data_values.data_exp[0]);  
        ECHO("%s.data_values.data_exp[1] = %d\n", name, param->data_values.data_exp[1]);  
        ECHO("%s.data_values.data_value[0] = %f\n", name, param->data_values.data_value[0]);  
        ECHO("%s.data_values.data_value[1] = %f\n", name, param->data_values.data_value[1]);  
        // lazy ECHO("%s.data_values.general_law = %f\n", name, param->data_values.  
    }  
    static void report_section_surface_data(UF_MODL_secsrf_data_p_t section_surface_data)  
    {  
        WRITE_D(section_surface_data->create_method);  
        WRITE_D(section_surface_data->polynomial_status);  
        report_secsrf_param("section_surface_data->rho_data",  
            &section_surface_data->rho_data);  
        WRITE_F(section_surface_data->tolerance);  
        WRITE_D(section_surface_data->tangent_faces[0]);  
        WRITE_D(section_surface_data->tangent_faces[1]);  
        WRITE_SL(section_surface_data->control_strings[0], TRUE);  
        WRITE_SL(section_surface_data->control_strings[1], TRUE);  
        WRITE_SL(section_surface_data->control_strings[2], TRUE);  
        WRITE_SL(section_surface_data->control_strings[3], TRUE);  
        WRITE_SL(section_surface_data->control_strings[4], TRUE);  
        WRITE_SL(section_surface_data->control_strings[5], TRUE);  
        WRITE_SL(section_surface_data->spine_string, TRUE);  
        report_secsrf_param("section_surface_data->radius_data",  
            &section_surface_data->radius_data);  
        report_secsrf_param("section_surface_data->angle_data",  
            &section_surface_data->angle_data);  
        WRITE_D(section_surface_data->direction_indicator);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            section_surface;  
        UF_MODL_secsrf_data_t  
            section_surface_data;  
        section_surface = select_a_feature("Report Section Surface data");  
        if (section_surface == NULL_TAG) return;  
        if (UF_CALL(UF_MODL_ask_section_surface(section_surface, &section_surface_data)))  
            return;  
        report_section_surface_data(&section_surface_data);  
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

> 这段NX二次开发代码的主要功能是查询和显示NX中某个截面曲面的参数。以下是代码的主要功能介绍：
>
> 1. 查询截面曲面参数：使用UF_MODL_ask_section_surface函数来获取指定截面曲面的参数。
> 2. 显示参数：通过report_section_surface_data函数打印出截面曲面的各种参数，如创建方法、多项式状态、数据参数等。此外，还通过WRITE_SL宏来显示控制曲线信息。
> 3. 显示控制曲线方向：使用show_curve_direction和show_curve_flip_direction函数来显示控制曲线的方向。
> 4. 显示临时文本：通过display_temporary_text函数在控制曲线旁边显示临时文本。
> 5. 高亮显示控制曲线：在打印控制曲线信息时，使用UF_DISP_set_highlight函数高亮显示当前的控制曲线。
> 6. 错误报告：通过UF_CALL宏来捕获函数调用错误，并使用report_error函数报告错误。
> 7. 初始化和清理：在ufusr函数中，调用UF_initialize进行初始化，在结束时调用UF_terminate进行清理。
> 8. 卸载请求：ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示在NX关闭时立即卸载该代码。
>
> 总的来说，这段代码实现了查询和可视化NX截面曲面参数的功能，具有一定的实用性和参考价值。
>
