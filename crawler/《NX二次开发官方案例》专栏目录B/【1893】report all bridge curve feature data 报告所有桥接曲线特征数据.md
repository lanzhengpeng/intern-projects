### 【1893】report all bridge curve feature data 报告所有桥接曲线特征数据

#### 代码

```cpp
    /*HEAD REPORT_ALL_BRIDGE_CURVE_FEATURE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。

只回答译文，不要废话。

UF_print_syslog是V18版本中新增的函数。 */  
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
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnL(X,n) (write_logical_array_to_listing_window(#X, X, n))  
    void write_logical_array_to_listing_window(char *title, logical *logicals,  
        int n_logicals)  
    {  
        int  
            ii;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_logicals; ii++)  
        {  
            if (logicals[ii])  
                sprintf(msg, "%s[%d] = TRUE\n", title, ii);  
            else  
                sprintf(msg, "%s[%d] = FALSE\n", title, ii);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
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
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITEnS(X,n) (write_string_array_to_listing_window(#X, X, n))  
    void write_string_array_to_listing_window(char *title, char **strings,  
        int n_strings)  
    {  
        int  
            ii;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            sprintf(msg, "%s[%d] = %s\n", title, ii, strings[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    #define WRITE_SL(X) report_string_list_info(#X, X)  
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
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, tangent, junk, junk,  
                junk, junk));  
        display_colored_conehead(start, tangent, UF_OBJ_RED);  
    }  
    static void show_curve_flip_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            end[3],  
            flip_dir[3],  
            tangent[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, tangent, junk, junk,  
                junk, junk));  
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
    static void report_string_list_info(char *name, UF_STRING_t *string)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        sprintf(msg, "%s->num = %d\n", name, string->num);  
        WRITE(msg);  
        for (ii = 0; ii < string->num; ii++)  
        {  
            sprintf(msg, "%s->string[%d] = %d\n", name, ii, string->string[ii]);  
            WRITE(msg);  
            sprintf(msg, "%s->dir[%d] = %d\n", name, ii, string->dir[ii]);  
            WRITE(msg);  
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
                sprintf(msg, "%s->id[%d] = %d\n", name, kk, string->id[kk]);  
                WRITE(msg);  
                UF_CALL(UF_DISP_set_highlight(string->id[kk], TRUE));  
                kk++;  
            }  
            sprintf(msg, "%d string curves highlighted", string->string[ii]);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        }  
    }  
    static void report_bridge_curve_feature_data( UF_CURVE_bridge_data_p_t  
            bridge_data)  
    {  
        WRITE_D(bridge_data->method);  
        WRITE_SL(bridge_data->input_curve1);  
        WRITE_SL(bridge_data->input_curve2);  
        WRITEnS(bridge_data->matchpt_parms, 2);  
        WRITEnD(2, (int *)bridge_data->match_point);  
        WRITEnL(bridge_data->reverse_tangents, 2);  
        WRITE_S(bridge_data->shape_control1);  
        WRITE_S(bridge_data->shape_control2);  
        WRITE_D(bridge_data->stiffness_method);  
        if (bridge_data->inherit_curve != NULL)  
        {  
            WRITE_SL(bridge_data->inherit_curve);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CURVE_bridge_data_t  
            bridge_data;  
        while ((feat = ask_next_feature_of_type(part, "BRIDGE_CURVE", feat)) != 0)  
        {  
            UF_CALL(UF_CURVE_ask_bridge_feature(feat, &bridge_data));  
            report_bridge_curve_feature_data(&bridge_data);  
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

> 这段代码是一个NX Open的二次开发示例，主要用于获取和报告NX部件中所有桥接曲线特征的数据。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数执行出错时输出错误信息。
> 2. 定义了一个函数ask_next_feature_of_type，用于遍历部件中的特征，并返回下一个指定类型(如"BRIDGE_CURVE")的特征。
> 3. 定义了一系列WRITE宏，用于在列表窗口中输出不同类型的数据。
> 4. 定义了一个函数display_colored_conehead，用于在指定位置显示带颜色的小圆锥。
> 5. 定义了函数show_curve_direction和show_curve_flip_direction，用于显示曲线的方向和翻转方向。
> 6. 定义了函数display_temporary_text，用于在指定位置显示临时文本。
> 7. 定义了函数report_string_list_info，用于报告字符串列表信息，包括其中的曲线ID和方向等。
> 8. 定义了函数report_bridge_curve_feature_data，用于报告桥接曲线特征的所有数据。
> 9. 主函数do_it遍历部件中的所有桥接曲线特征，并调用report_bridge_curve_feature_data函数报告每个特征的数据。
> 10. ufusr函数在NX启动时调用，初始化后执行do_it函数，完成后终止NX。
> 11. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 总体而言，该代码通过遍历部件中的桥接曲线特征，获取每个特征的数据，并在列表窗口中输出这些数据，同时通过显示圆锥和临时文本直观地展示曲线的方向。
>
