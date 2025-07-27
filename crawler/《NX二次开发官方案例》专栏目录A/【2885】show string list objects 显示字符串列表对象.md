### 【2885】show string list objects 显示字符串列表对象

#### 代码

```cpp
    static void display_temporary_colored_text(char *text, double loc[3], int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
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
    static void show_string_list_objects(UF_STRING_t *string)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < string->num; ii++)  
        {  
            if (string->dir[ii] == UF_MODL_CURVE_START_FROM_BEGIN)  
                show_curve_direction(string->id[kk]);  
            else  
                show_curve_flip_direction(string->id[kk]);  
            for (jj = 0; jj < string->string[ii]; jj++)  
            {  
                sprintf(msg, "%d", jj+1);  
                if (UF_OBJ_ask_name_origin(string->id[kk], loc))  
                {  
                    UF_CALL(UF_OBJ_set_name(string->id[kk], "X"));  
                    UF_CALL(UF_OBJ_ask_name_origin(string->id[kk], loc));  
                    UF_CALL(UF_OBJ_delete_name(string->id[kk]));  
                }  
                display_temporary_colored_text(msg, loc, UF_OBJ_WHITE);  
                kk++;  
            }  
        }  
    }

```

#### 代码解析

> 根据这段NX二次开发代码，主要实现了以下功能：
>
> 1. 显示临时彩色文本：使用函数display_temporary_colored_text来在NX界面中显示带有颜色的临时文本。它接收文本内容、位置和颜色作为参数。
> 2. 显示彩色圆锥头：使用函数display_colored_conehead来在NX界面中显示一个指定颜色的圆锥头。它接收圆锥头原点、方向和颜色作为参数。
> 3. 显示曲线方向：使用函数show_curve_direction来显示曲线的切线方向，使用红色圆锥头表示。
> 4. 显示曲线翻转方向：使用函数show_curve_flip_direction来显示曲线的翻转方向，即切线方向的负方向，同样使用红色圆锥头表示。
> 5. 显示字符串列表中的对象：使用函数show_string_list_objects来遍历字符串列表，对于列表中的每个元素，根据方向来显示曲线的起始方向或翻转方向，并显示文本标注。
>
> 总体而言，这段代码通过NX的显示接口实现了对曲线的标注显示，通过彩色圆锥头和临时文本直观地展示曲线的方向和位置信息。
>
