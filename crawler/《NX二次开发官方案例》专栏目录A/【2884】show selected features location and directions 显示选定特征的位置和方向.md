### 【2884】show selected features location and directions 显示选定特征的位置和方向

#### 代码

```cpp
    /*HEAD SHOW_SELECTED_FEATURES_LOCATION_AND_DIRECTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog在V18中新增，请只提供翻译，不要添加其他内容。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_FNT(X) report_feature_name_and_type(#X, X)  
    static void report_feature_name_and_type(char *which, tag_t feature)  
    {  
        char  
            *name,  
            *type,  
            msg[MAX_LINE_LENGTH+1];  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feature, &type)))  
        {  
            sprintf(msg, "%s - tag_t %d - %s (%s)\n", which, feature, name, type);  
            WRITE(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void ask_wcs_location_and_directions(double origin[3], double x_axis[3],  
        double y_axis[3])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            axes[9],  
            mag;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        UF_VEC3_unitize(&axes[0], 0, &mag, x_axis);  
        UF_VEC3_unitize(&axes[3], 0, &mag, y_axis);  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical ask_feature_location(tag_t feature, double loc[3])  
    {  
        logical  
            found = FALSE;  
        int  
            ii,  
            n_members;  
        tag_t  
            *members;  
        char  
            *type;  
        if (!UF_MODL_ask_feat_location(feature, loc)) return TRUE;  
        UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
        if (!strcmp(type, "UDF_INS") ||  
            !strcmp(type, "MIRROR_SET") ||  
            !strcmp(type, "INSTNACE_SET") ||  
            !strcmp(type, "INSTANCE_SET") ||  
            !strcmp(type, "LINEAR_PATTERN") ||  
            !strcmp(type, "CIRCULAR_PATTERN") ||  
            !strcmp(type, "FSET"))  
        {  
            UF_CALL(UF_MODL_ask_all_members_of_set(feature, &members, &n_members));  
            for (ii = 0; ii < n_members; ii++)  
            {  
                found = ask_feature_location(members[ii], loc);  
                if (found) break;  
            }  
            if (n_members > 0) UF_free(members);  
        }  
        UF_free(type);  
        return found;  
    }  
    static logical ask_feature_directions(tag_t feature, double x_dir[3],  
        double y_dir[3])  
    {  
        logical  
            found = FALSE;  
        int  
            ii,  
            n_exps,  
            n_members,  
            pr5233539;  
        tag_t  
            *exps,  
            *members;  
        double  
            loc_one[3],  
            loc_two[3],  
            n_x,  
            mag;  
        char  
            **descs,  
            *type;  
        UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
        if (!UF_CALL(UF_MODL_ask_feat_direction(feature, x_dir, y_dir)))  
        {  
            UF_VEC3_is_equal(x_dir, y_dir, 0, &pr5233539);  
            if (!strcmp(type, "LINEAR_PATTERN") && pr5233539)  
            {  
                WRITE("From UF_MODL_ask_feat_direction:\n");  
                WRITE3F(x_dir);  
                WRITE3F(y_dir);  
                WRITE("After work around for PR 5233539:\n");  
                UF_CALL(UF_MODL_ask_exp_desc_of_feat(feature, &n_exps, &descs,  
                    &exps));  
                for (ii = 0; ii < n_exps; ii++)  
                {  
                    if (!strcmp(descs[ii], "Number along X"))  
                    {  
                        UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &n_x));  
                        break;  
                    }  
                }  
                UF_free_string_array(n_exps, descs);  
                UF_free(exps);  
                UF_CALL(UF_MODL_ask_all_members_of_set(feature, &members,  
                    &n_members));  
                if (n_members > (int)n_x)  
                {  
                    if (ask_feature_location(members[0], loc_one) &&  
                        ask_feature_location(members[(int)n_x], loc_two))  
                    {  
                        UF_VEC3_sub(loc_two, loc_one, y_dir);  
                        UF_VEC3_unitize(y_dir, 0, &mag, y_dir);  
                        found = TRUE;  
                    }  
                    UF_free(members);  
                }  
            }  
            else found = TRUE;  
        }  
        else  
        {  
            if (!strcmp(type, "UDF_INS") ||  
                !strcmp(type, "MIRROR_SET") ||  
                !strcmp(type, "INSTNACE_SET") ||  
                !strcmp(type, "INSTANCE_SET") ||  
                !strcmp(type, "LINEAR_PATTERN") ||  
                !strcmp(type, "CIRCULAR_PATTERN") ||  
                !strcmp(type, "FSET"))  
            {  
                UF_CALL(UF_MODL_ask_all_members_of_set(feature, &members,  
                    &n_members));  
                for (ii = 0; ii < n_members; ii++)  
                {  
                    found = ask_feature_directions(members[ii], x_dir, y_dir);  
                    if (found) break;  
                }  
                if (n_members > 0) UF_free(members);  
            }  
            UF_free(type);  
        }  
        return found;  
    }  
    static void show_feature_location_and_direction(tag_t feature)  
    {  
        double  
            loc[3],  
            x_dir[3],  
            y_dir[3];  
        WRITE_FNT(feature);  
        ask_wcs_location_and_directions(loc, x_dir, y_dir);  
        if (ask_feature_location(feature, loc))  
        {  
            WRITE3F(loc);  
        }  
        if (ask_feature_directions(feature, x_dir, y_dir))  
        {  
            WRITE3F(x_dir);  
            WRITE3F(y_dir);  
            display_colored_conehead(loc, x_dir, UF_OBJ_RED);  
            display_colored_conehead(loc, y_dir, UF_OBJ_GREEN);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
            show_feature_location_and_direction(feat);  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是：
>
> 1. 提示用户选择一个特征，然后显示该特征在当前工作坐标系(WCS)中的位置和方向。
> 2. 如果特征是实例集合、线性阵列等，则获取第一个成员的位置和方向。
> 3. 在NX图形窗口中用不同颜色的锥头表示特征的方向向量。
> 4. 将位置和方向信息输出到列表窗口中。
> 5. 提供错误处理，捕获函数调用错误并打印到系统日志和列表窗口。
> 6. 使用宏简化代码，如WRITE和WRITE3F用于输出信息。
> 7. 包含了二次开发常用的函数，如UF_MODL_ask_feat_name和UF_MODL_ask_feat_direction。
> 8. 通过UF_CALL宏封装函数调用，简化错误处理。
> 9. 提供了unload函数，允许立即卸载二次开发应用程序。
> 10. main函数封装在ufusr中，这是NX二次开发的约定。
>
> 总体来说，这段代码涵盖了NX二次开发中常用的功能，包括错误处理、位置方向获取、图形显示和信息输出，具有一定的参考价值。
>
