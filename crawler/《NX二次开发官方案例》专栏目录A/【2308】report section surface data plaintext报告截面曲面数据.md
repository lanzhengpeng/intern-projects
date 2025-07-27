### 【2308】report section surface data plaintext报告截面曲面数据

#### 代码

```cpp
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

```

#### 代码解析

> 这段NX二次开发代码主要用于处理和显示NX中的曲线和曲面数据。具体来说，代码的主要功能包括：
>
> 1. 显示曲线的方向和反方向：使用圆锥头标注曲线的起点和终点方向，通过函数show_curve_direction和show_curve_flip_direction实现。
> 2. 在曲线上显示临时文字：通过display_temporary_text函数在曲线上显示文字，用于标识曲线。
> 3. 报告字符串列表信息：使用report_string_list_info函数报告字符串列表中的曲线信息，并高亮显示。
> 4. 报告截面曲面数据：使用report_section_surface_data函数报告截面曲面的各种参数，包括控制曲线、半径、角度等数据。
> 5. 定义宏以简化输出：通过WRITE_D、WRITE_F、WRITE_SL等宏简化输出函数参数的代码。
> 6. 报告参数数据：使用report_secsrf_param函数报告参数数据，包括数据方法、常数值、指数值等。
>
> 总的来说，这段代码主要用于NX二次开发中处理和显示曲线、曲面数据，通过调用NX的API函数实现相关功能。
>
