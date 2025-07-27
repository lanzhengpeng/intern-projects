### 【2393】report thru curves 报告曲线信息

#### 代码

```cpp
    /*HEAD REPORT_THRU_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_vec.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE6F(X)  (write_double_array_to_listing_window(#X, X, 2, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
                sprintf(msg, "%f", array[kk++]);  
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
                sprintf(msg, "%s->id[%d] = %d\n", name, kk, string->id[kk]);  
                WRITE(msg);  
                sprintf(msg, "%d", kk);  
                if (UF_OBJ_ask_name_origin(string->id[kk], loc))  
                {  
                    UF_CALL(UF_OBJ_set_name(string->id[kk], "X"));  
                    UF_CALL(UF_OBJ_ask_name_origin(string->id[kk], loc));  
                    UF_CALL(UF_OBJ_delete_name(string->id[kk]));  
                }  
                display_temporary_text(msg, loc);  
                UF_CALL(UF_DISP_set_highlight(string->id[kk], TRUE));  
                kk++;  
            }  
            sprintf(msg, "%d string curves highlighted", string->string[ii]);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            alignment,  
            body_type,  
            flags[2],  
            patch,  
            vdegree,  
            vstatus;  
        double  
            tol[3],  
            value[6];  
        tag_t  
            feat = NULL_TAG,  
            faces[2];  
        UF_STRING_t  
            section,  
            spine;  
        while ((feat = select_a_feature("Remove/Add Section String")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_MODL_ask_thru_curves(feat, &section, &spine, &patch,  
                &alignment, value, &vdegree, &vstatus, &body_type, tol, faces,  
                flags))) continue;  
            WRITE_SL(&section);  
            WRITE_SL(&spine);  
            WRITE_D(patch);  
            WRITE_D(alignment);  
            if (alignment == 3) WRITE3F(value);  
            if (alignment == 4) WRITE6F(value);  
            WRITE_D(vdegree);  
            WRITE_D(vstatus);  
            WRITE_D(body_type);  
            WRITE3F(tol);  
            WRITE_D(faces[0]);  
            WRITE_D(faces[1]);  
            WRITE_D(flags[0]);  
            WRITE_D(flags[1]);  
            UF_MODL_free_string_list(&section);  
            UF_MODL_free_string_list(&spine);  
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

> 这段代码是用于NX CAD软件的二次开发代码，主要实现了以下功能：
>
> 1. 选择特征：使用UF_UI_select_feature函数让用户选择特征，并获取特征列表。
> 2. 获取特征信息：调用UF_MODL_ask_thru_curves函数获取特征的穿越曲线信息，包括截面线串、脊柱线串、修补面、对齐方式、参数值、度数、状态、体类型、公差、面等。
> 3. 输出信息：将获取的信息输出到列表窗口，包括线串信息、修补面、对齐方式、参数值、度数、状态、体类型、公差、面、标志等。
> 4. 高亮显示线串：使用UF_DISP函数高亮显示每个线串，并显示其方向。
> 5. 清理：最后清理使用的资源。
>
> 整体来说，这段代码实现了选择特征、获取特征信息、输出信息、高亮显示线串等功能，为用户提供了一个交互式的特征信息查看工具。代码结构清晰，注释详细，易于理解和维护。
>
