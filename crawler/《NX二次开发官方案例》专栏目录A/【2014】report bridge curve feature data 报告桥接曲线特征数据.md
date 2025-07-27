### 【2014】report bridge curve feature data 报告桥接曲线特征数据

#### 代码

```cpp
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

```

#### 代码解析

> 这段代码是NX二次开发的示例代码，主要功能是输出桥接曲线特征的数据。以下是代码的主要功能：
>
> 1. 定义了一系列宏，用于将不同类型的数据格式化输出到列表窗口。这些宏包括：WRITE用于输出字符串，WRITE_D用于输出整数，WRITE_S用于输出字符串，WRITEnL用于输出逻辑数组，WRITEnD用于输出整数数组，WRITEnS用于输出字符串数组，WRITE_SL用于输出字符串列表。
> 2. 定义了write_integer_to_listing_window、write_string_to_listing_window等函数，用于将不同类型的数据写入列表窗口。
> 3. 定义了display_colored_conehead函数，用于显示不同颜色的圆锥头，用于表示曲线的方向。
> 4. 定义了show_curve_direction和show_curve_flip_direction函数，用于显示曲线的切线方向和反转方向。
> 5. 定义了display_temporary_text函数，用于在指定位置显示临时文字。
> 6. 定义了report_string_list_info函数，用于报告字符串列表信息，并在每个字符串曲线处显示数字编号。
> 7. 定义了report_bridge_curve_feature_data函数，用于输出桥接曲线特征的数据，包括方法、输入曲线、匹配点、反转切线、形状控制等。
> 8. 使用了NX提供的UF_CALL宏，用于调用NX的函数，并在调用失败时打印错误信息。
> 9. 使用了NX提供的各种数据结构，如UF_DISP_conehead_attrb_s、UF_OBJ_disp_props_t等。
> 10. 整体来看，这段代码通过宏和函数封装，实现了对桥接曲线特征数据的格式化输出和显示，便于调试和查看。
>
