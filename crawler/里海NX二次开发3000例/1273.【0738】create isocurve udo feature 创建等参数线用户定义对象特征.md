### 【0738】create isocurve udo feature 创建等参数线用户定义对象特征

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_wave.h>  
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
    static UF_UDOBJ_class_t ISO_class_id = 0;  
    DllExport extern UF_UDOBJ_class_t get_ISO_class_id(void)  
    {  
        return ISO_class_id;  
    }  
    /* 里海 */  
    DllExport extern void isocurve_update(tag_t udo,  
        UF_UDOBJ_link_p_t update_cause)  
    {  
        int  
            dir,  
            ii,  
            n_curves,  
            n_old_curves,  
            n_new_curves,  
            subtype,  
            status,  
            type;  
        tag_t  
            *old_curves,  
            *new_curves,  
            face,  
            exp;  
        double  
            fparm,  
            parm,  
            tol,  
            uvs[4];  
        UF_UDOBJ_all_data_t  
            data;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_ask_udo_data( udo, &data ));  
        dir = data.ints[0];  
        tol = data.doubles[0];  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(data.link_defs[ii].assoc_ug_tag,  
                &type, &subtype));  
            if (type == UF_scalar_type)  
            {  
                UF_CALL(UF_SO_ask_exp_of_scalar(data.link_defs[ii].assoc_ug_tag,  
                    &exp));  
                UF_CALL(UF_MODL_ask_exp_tag_value(exp, &parm));  
                break;  
            }  
        }  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(data.link_defs[ii].assoc_ug_tag,  
                &type, &subtype));  
            if (type == UF_solid_type)  
            {  
                face = data.link_defs[ii].assoc_ug_tag;  
                status = UF_OBJ_ask_status(face);  
                if (status != UF_OBJ_ALIVE) continue;  
                UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
                fparm = parm * (uvs[2*dir-1] - uvs[2*dir-2]) + uvs[2*dir-2];  
                if (!UF_CALL(UF_MODL_create_isocurve(face, dir, fparm, tol,  
                        &new_curves, &n_new_curves)))  
                {  
                    UF_CALL(UF_UDOBJ_ask_owned_objects(udo, &n_old_curves,  
                        &old_curves));  
                    n_curves = (n_old_curves < n_new_curves) ? n_old_curves :  
                        n_new_curves;  
                    UF_CALL(UF_OBJ_replace_object_array_data(n_curves,  
                        old_curves, new_curves));  
                    for (ii = n_old_curves; ii < n_new_curves; ii++)  
                    {  
                        UF_CALL(UF_UDOBJ_add_owning_links(udo, ii,  
                            &new_curves[ii]));  
                    }  
                    for (ii = n_new_curves; ii < n_old_curves; ii++)  
                    {  
                        UF_CALL(UF_UDOBJ_delete_owning_link(udo, old_curves[ii]));  
                        UF_CALL(UF_OBJ_delete_object(old_curves[ii]));  
                    }  
                    if (n_old_curves > 0) UF_free(old_curves);  
                    if (n_new_curves > 0) UF_free(new_curves);  
                }  
            }  
        }  
        UF_CALL(UF_UDOBJ_free_udo_data( &data ));  
        UF_terminate();  
    }  
    static void register_isocurve_udo(void)  
    {  
        UF_CALL(UF_UDOBJ_create_class("isocurve_UDO", "ISOCURVE", &ISO_class_id));  
        UF_CALL(UF_UDOBJ_set_owned_object_selection(ISO_class_id,  
            UF_UDOBJ_ALLOW_SELECTION));  
        UF_CALL(UF_UDOBJ_register_update_cb(ISO_class_id, isocurve_update));  
    }  
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
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
    static logical show_face_parm_dirs(tag_t face)  
    {  
        double  
            junk[3],  
            origin[3],  
            parm[2],  
            uvs[4],  
            u_dir[3],  
            v_dir[3];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = uvs[0];  
        parm[1] = uvs[2];  
        UF_CALL(UF_MODL_ask_face_props(face, parm, origin, u_dir, v_dir,  
            junk, junk, junk, junk));  
        display_colored_conehead(origin, u_dir, UF_OBJ_BLUE);  
        display_colored_conehead(origin, v_dir, UF_OBJ_GREEN);  
        return TRUE;  
    }  
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 译:返回1或2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            dir = { 1 };  
        tag_t  
            face,  
            feat,  
            exp,  
            *isocurves,  
            so_exp,  
            udo,  
            udo_feat;  
        double  
            fparm,  
            parm = 0.5,  
            tol,  
            uvs[4];  
        char  
            *pprompts[2] = { "U Parm", "V Parm" },  
            exp_str[UF_MAX_EXP_LENGTH];  
        UF_UDOBJ_class_t  
            iso_class = get_ISO_class_id();  
        UF_UDOBJ_link_t  
            link_defs[2];  
        if (iso_class == 0)  
        {  
            register_isocurve_udo();  
            iso_class = get_ISO_class_id();  
        }  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((face = select_a_face("Select face for isocurve")) != NULL_TAG) &&  
                show_face_parm_dirs(face) &&  
                pick_one_of_two_choices("Which direction?", "U (Green)",  
                    "V (Blue)", &dir) &&  
                prompt_for_a_number("Constant value (0 - 1)", pprompts[dir-1],  
                    &parm))  
        {  
        /*  create a timestamp copy of the face and base the isocurve on it  
            in case the user decides to subdivide the original face with its own  
            isocurve which would create a circular dependency */  
            UF_CALL(UF_WAVE_create_linked_face(face, NULL_TAG, face, TRUE, &feat));  
            UF_CALL(UF_WAVE_ask_linked_feature_geom(feat, &face));  
            UF_CALL(UF_OBJ_set_blank_status(face, UF_OBJ_BLANKED));  
        /*  the above 3 lines can be deleted if the output isocurve will never  
            be used to modify its parent face */  
            sprintf(exp_str, "%f", parm);  
            strip_trailing_zeros(exp_str);  
            UF_CALL(UF_MODL_create_exp_tag(exp_str, &exp));  
            UF_CALL(UF_UDOBJ_create_udo_feature(iso_class, &udo, &udo_feat));  
            UF_CALL(UF_MODL_update());  
            UF_CALL(UF_SO_create_scalar_exp(exp, UF_SO_update_within_modeling,  
                exp, &so_exp));  
            link_defs[0].link_type = 1;  
            link_defs[0].assoc_ug_tag = so_exp;  
            link_defs[0].object_status = 0;  
            link_defs[1].link_type = 1;  
            link_defs[1].assoc_ug_tag = face;  
            link_defs[1].object_status = 0;  
            UF_CALL(UF_UDOBJ_add_links(udo, 2, link_defs));  
            UF_CALL(UF_UDOBJ_add_integers(udo, 1, &dir));  
            UF_CALL(UF_UDOBJ_add_doubles(udo, 1, &tol));  
            UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
            fparm = parm * (uvs[2*dir-1] - uvs[2*dir-2]) + uvs[2*dir-2];  
            if (!UF_CALL(UF_MODL_create_isocurve(face, dir, fparm, tol,  
                &isocurves, &n_curves)) && (n_curves > 0))  
            {  
                UF_CALL(UF_UDOBJ_add_owning_links(udo, n_curves, isocurves));  
                UF_free(isocurves);  
            }  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    /* 里海 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_isocurve_udo();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于创建和更新等参数曲线(ISO曲线)。具体功能包括：
>
> 1. 创建名为"ISOCURVE"的UDO(用户定义对象)类，并注册更新回调函数isocurve_update。
> 2. isocurve_update函数根据UDO中的参数，在关联的面上创建等参数曲线。参数包括方向(U或V)、参数值(0-1之间)和公差。
> 3. 主函数do_it实现用户交互，选择一个面，指定方向和参数值，然后在该面上创建等参数曲线，并将结果保存在UDO中。
> 4. ufusr函数在NX启动时执行，调用do_it函数实现用户交互。
> 5. ufsta函数在NX启动时注册UDO类。
> 6. 代码中还包含一些辅助函数，如错误报告、提示框、选择面、提示输入参数等。
>
> 总体来说，这段代码实现了NX中创建和更新等参数曲线的二次开发功能。
>
