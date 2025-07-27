### 【1095】display selected datum planes normal 显示选定基准平面的法线

#### 代码

```cpp
    /*HEAD DISPLAY_SELECTED_DATUM_PLANES_NORMAL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18版本新增的功能，请只回答翻译，不要添加其他无关内容。 */  
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
    /* qq3123197280 */  
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_objs,  
            subtype,  
            type;  
        tag_t  
            dplane,  
            feat,  
            matrix,  
            *objs;  
        double  
            normal[3],  
            origin[3],  
            xyz[9];  
        char  
            *angle,  
            *feat_type,  
            *offset;  
        while ((dplane = select_a_datum_plane("Show directions")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_object_feat(dplane, &feat));  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            if (!strcmp(feat_type, "DATUM_CSYS"))  
            {  
                UF_CALL(UF_MODL_ask_feat_object(feat, &n_objs, &objs));  
                for (ii = 0; ii < n_objs; ii++)  
                {  
                    UF_CALL(UF_OBJ_ask_type_and_subtype(objs[ii], &type, &subtype));  
                    if (type == UF_point_type) break;  
                }  
                if (ii < n_objs)  
                {  
                    UF_CALL(UF_CURVE_ask_point_data(objs[ii], origin));  
                    UF_CALL(UF_CSYS_ask_matrix_of_object(dplane, &matrix));  
                    UF_CALL(UF_CSYS_ask_matrix_values(matrix, xyz));  
                    display_colored_conehead(origin, &xyz[6], UF_OBJ_RED);  
                }  
                UF_free(objs);  
            }  
            else  
            {  
                UF_CALL(UF_MODL_ask_datum_plane_parms(feat, origin, normal,  
                    &offset, &angle));  
                display_colored_conehead(origin, normal, UF_OBJ_GREEN);  
                UF_free(offset);  
                UF_free(angle);  
            }  
            UF_free(feat_type);  
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

> 这段代码是用于NX二次开发，其主要功能是选择一个基准平面，并显示该基准平面的法向量和原点。具体步骤如下：
>
> 1. 包含了必要的NX头文件。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了一个用于过滤基准平面的选择掩码mask_for_datum_planes。
> 4. 定义了一个选择基准平面的函数select_a_datum_plane。
> 5. 定义了一个显示有色圆锥头的函数display_colored_conehead。
> 6. 定义了主要函数do_it，用于循环选择基准平面，并显示其法向量和原点。
> 7. ufusr函数是NX二次开发的主函数，调用do_it执行主功能。
> 8. ufusr_ask_unload函数用于卸载二次开发程序。
>
> 总的来说，这段代码通过选择基准平面，并显示其法向量和原点，实现了NX中基准平面的可视化。
>
