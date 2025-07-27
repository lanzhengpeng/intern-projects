### 【0583】create and rpo boss 创建凸台特征并在其上创建径向平面特征

#### 代码

```cpp
    /*HEAD CREATE_AND_RPO_BOSS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的信息，UF_print_syslog 是在 V18 中新增的功能。文档中要求只提供翻译，不添加任何废话。因此，UF_print_syslog 在 V18 中是一个新功能。 */  
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
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static logical work_around_1(tag_t *object)  
    {  
        int  
            d_units,  
            p_units;  
        tag_t  
            comp,  
            part;  
        uf_list_p_t  
            face_list;  
        if (UF_ASSEM_is_occurrence(*object))  
        {  
            comp = UF_ASSEM_ask_part_occurrence(*object);  
            part = UF_ASSEM_ask_prototype_of_occ(comp);  
            UF_CALL(UF_PART_ask_units(part, &p_units));  
            UF_CALL(UF_PART_ask_units(UF_PART_ask_display_part(), &d_units));  
            if (p_units != d_units)  
            {  
                UF_CALL(UF_MODL_extract_face(*object, 0, object));  
                UF_CALL(UF_MODL_ask_body_faces(*object, &face_list));  
                UF_CALL(UF_MODL_ask_list_item(face_list, 0, object));  
                UF_CALL(UF_MODL_delete_list(&face_list));  
                return TRUE;  
            }  
        }  
        return FALSE;  
    }  
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
    static tag_t select_point_on_face(char *prompt, double cp[3])  
    {  
        logical  
            fix;  
        int  
            n,  
            resp;  
        tag_t  
            face,  
            line,  
            object,  
            view;  
        double  
            tol;  
        UF_CURVE_line_t  
            lp;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            map_abs2view(cp);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            face = object;  
            fix = work_around_1(&face);  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            if (fix) UF_CALL(UF_OBJ_delete_object(face));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_face_dir(double *loc, tag_t face, double *z_dir)  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV1[3],  
            dirV2[3],  
            radii[2],  
            x_dir[3];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, dirV1, dirU2,  
            dirV2, z_dir, radii));  
    }  
    static tag_t select_circular_edge_or_point(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            edge,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask[2];  
        opts.num_mask_triples = 2;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask[0].object_type = UF_solid_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = UF_UI_SEL_FEATURE_CIRCULAR_EDGE;  
        mask[1].object_type = UF_point_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static int rpo_boss(tag_t feat)  
    {  
        tag_t  
            target[1],  
            tool[1];  
        int  
            center[1] = { UF_MODL_ARC_CENTER };  
        char  
            *constraints_array[1] = { "PARA_DIST_PARMS" },  
            *constraints_value[1] = { "0.0" };  
        UF_DISP_regenerate_display();  
        if ((target[0] = select_circular_edge_or_point("Select target")) &&  
            (tool[0] = select_circular_edge_or_point("Select tool edge")))  
            return(UF_MODL_create_rpo_constraints(feat, NULL_TAG, NULL_TAG,  
                target, center, tool, center, constraints_value,  
                constraints_array, 1));  
        else return (0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            boss;  
        double  
            loc[3],  
            dir[3];  
        UF_CALL(UF_MODL_register_rpo_routine(rpo_boss));  
        while ((face=select_point_on_face("Select face for boss", loc)) != NULL_TAG)  
        {  
            ask_face_dir(loc, face, dir);  
            UF_CALL(UF_MODL_create_boss(loc, dir, ".5", ".25", "0", face, &boss));  
        }  
        UF_CALL(UF_MODL_unregister_rpo_routine());  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
    /*  Note:  can NOT be unloaded - calls UF_MODL_register_rpo_routine.  
        return (UF_UNLOAD_IMMEDIATELY);  
    */  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能是在NX中创建一个凸台特征。
>
> 主要步骤如下：
>
> 1. 注册凸台约束求解回调：通过UF_MODL_register_rpo_routine函数注册凸台约束求解回调函数rpo_boss，用于在创建凸台时确定其位置和方向。
> 2. 选择面并获取法向：通过循环调用select_point_on_face函数，让用户选择一个面，并获取该面的一个点坐标和法向。
> 3. 创建凸台：根据选择的点和法向，使用UF_MODL_create_boss函数创建一个凸台特征。
> 4. 取消注册：使用UF_MODL_unregister_rpo_routine取消注册的凸台约束求解回调。
> 5. 错误处理：代码中包含错误处理函数report_error，用于打印出函数调用失败的信息。
> 6. 坐标变换：提供了视图坐标和绝对坐标之间的变换函数map_abs2view和map_view2abs。
> 7. NX环境初始化和终止：在ufusr函数中初始化NX环境，调用do_it函数创建凸台，然后终止NX环境。
> 8. 卸载函数：由于代码注册了回调，因此不能直接卸载，ufusr_ask_unload函数返回UF_UNLOAD_UG_TERMINATE表示需要在NX退出时卸载。
>
> 综上所述，这段代码实现了在NX中创建凸台特征的功能，通过用户选择的面来确定凸台的位置和方向，并进行了错误处理。
>
