### 【2493】report string list info 报告字符串列表信息

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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

```

#### 代码解析

> 这段代码用于在NX中显示和报告字符串列表信息。
>
> 主要功能包括：
>
> 1. 显示曲线的方向和反向方向，通过在曲线起点和终点显示红色圆锥头。
> 2. 在曲线上显示临时文本，例如曲线的ID号。
> 3. 高亮显示曲线。
> 4. 在列表窗口中报告字符串列表的属性，例如数量、每个字符串的长度、方向等。
> 5. 使用宏WRITE和WRITE_SL来简化在列表窗口中的写入操作。
>
> 代码中的关键函数包括：
>
> 整体上，这段代码通过NX提供的显示和查询函数，实现了对字符串列表的可视化显示和报告。
>
