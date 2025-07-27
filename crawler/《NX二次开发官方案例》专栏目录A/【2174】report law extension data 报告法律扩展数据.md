### 【2174】report law extension data 报告法律扩展数据

#### 代码

```cpp
    /*HEAD REPORT_LAW_EXTENSION_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是在 V18 中新增的，它用于打印系统日志。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_name_and_type(tag_t feature)  
    {  
        char  
            *feature_name,  
            *feature_type;  
        WRITE_D(feature);  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &feature_name)))  
        {  
            WRITE_S(feature_name);  
            UF_free(feature_name);  
        }  
        if (!UF_CALL(UF_MODL_ask_feat_type(feature, &feature_type)))  
        {  
            WRITE_S(feature_type);  
            UF_free(feature_type);  
        }  
    }  
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
    #define WRITEnT(n,X) (write_tag_array_to_listing_window(#X, X, 1, n))  
    static void write_tag_array_to_listing_window(char *name, tag_t *array,  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_law_extension_data(tag_t law_extension)  
    {  
        UF_MODL_lawext_data_t  
            data;  
        data.bisided = 99;  
        report_feature_name_and_type(law_extension);  
        if (UF_CALL(UF_MODL_ask_law_extension(law_extension, &data))) return;  
        WRITE_D(data.ref_type);  
        WRITE_SL(data.curve, FALSE);  
        WRITE_D(data.n_faces);  
        WRITEnT(data.n_faces, data.face_ids);  
        WRITE_D(data.smart_dir);  
        WRITE_SL(data.spine, FALSE);  
        WRITE_F(data.distance_tol);  
        WRITE_F(data.angle_tol);  
        WRITE_D(data.merge);  
        if (data.bisided == 99) WRITE("* ");  
        WRITE_D(data.bisided);  
        if (data.bisided == 99) WRITE("* bisided not returned - see PR 2039516\n");  
        WRITE("\n");  
        UF_MODL_free_law_extension(&data, TRUE);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Report Law Extension")) != NULL_TAG)  
            report_law_extension_data(feat);  
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

> 这是一段用于报告NX Law Extension特征的二次开发代码，主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在出现UF函数调用错误时打印错误信息。
> 2. 定义了选择特征函数select_a_feature，用于提示用户选择Law Extension特征。
> 3. 定义了各种写入信息到列表窗口的函数，用于输出特征的各种信息。
> 4. 定义了显示曲线方向的函数show_curve_direction，用于在曲线的起点显示红色圆锥头表示方向。
> 5. 定义了报告字符串列表信息的函数report_string_list_info，用于输出字符串列表的信息，并高亮显示相关曲线。
> 6. 定义了报告Law Extension特征数据的函数report_law_extension_data，用于输出特征的各种参数信息。
> 7. 主函数do_it循环调用select_a_feature和report_law_extension_data，让用户选择特征并报告信息。
> 8. ufusr函数初始化UF模块，调用do_it，然后终止UF模块。
> 9. ufusr_ask_unload函数返回立即卸载，表示不需要在卸载时保留状态。
>
> 总体来说，这段代码实现了选择Law Extension特征并报告其详细信息的二次开发功能，通过UF函数调用实现了与NX系统的交互。
>
