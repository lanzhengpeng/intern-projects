### 【0585】create and rpo pads 在UG中创建矩形凸台，并使用RPO约束来定义平行约束

#### 代码

```cpp
    /*HEAD CREATE_AND_RPO_PADS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static void ask_face_dirs(double loc[3], tag_t face, double xyz[9])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV2[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, &xyz[0], &xyz[3], dirU2,  
            dirV2, &xyz[6], radii));  
    }  
    static int mask_for_linear_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_linear_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int rpo_pad(tag_t feat)  
    {  
        tag_t  
            target[2],  
            tool[2];  
        int  
            center[2] = { 0, 0 };  
        char  
            *constraints_array[2] = { "PERP_DIST_PARMS", "PERP_DIST_PARMS" },  
            *constraints_value[2] = { "1.0", "2.0" };  
        if ((target[0] = select_a_linear_edge("Select target edge 1")) &&  
            (tool[0] = select_a_linear_edge("Select parallel tool edge")) &&  
            (target[1] = select_a_linear_edge("Select target edge 2")) &&  
            (tool[1] = select_a_linear_edge("Select parallel tool edge")))  
            return(UF_MODL_create_rpo_constraints(feat, target[0], NULL_TAG,  
                target, center, tool, center, constraints_value,  
                constraints_array, 2));  
        else return (0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            pad;  
        double  
            loc[3],  
            dest_csys[9];  
        char  
            *size[3] = { "2", "3", "1" };  
        UF_CALL(UF_MODL_register_rpo_routine(rpo_pad));  
        while ((face=select_point_on_face("Select face for pad", loc)) != NULL_TAG)  
        {  
            ask_face_dirs(loc, face, dest_csys);  
            UF_CALL(UF_MODL_create_rect_pad(loc, &dest_csys[6], dest_csys,  
                    size, "0.0", "0.0", face, &pad));  
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

> 这段NX二次开发代码的主要功能是在NX中创建矩形垫片，并使用自定义的RPO(矩形垫片优化)约束来实现。以下是代码的主要功能介绍：
>
> 1. 包含头文件：代码包含了NX二次开发常用的头文件，用于调用NX API。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 3. 坐标转换函数：定义了两个坐标转换函数map_abs2view和map_view2abs，用于将绝对坐标系下的点转换到当前视图坐标系，以及相反的转换。
> 4. 工作零件处理函数：定义了一个工作零件处理函数work_around_1，用于处理选择面时可能存在的零件单位不一致的情况。
> 5. 面选择遮罩函数：定义了一个面选择遮罩函数mask_for_faces，用于在对话框中设置只选择面的遮罩。
> 6. 点选择函数：定义了一个点选择函数select_point_on_face，用于让用户在面上选择一个点，并返回该面的tag。
> 7. 获取面方向函数：定义了一个获取面方向函数ask_face_dirs，用于根据面上某点的参数获取面的法向和UV方向。
> 8. 线选择遮罩函数：定义了一个线选择遮罩函数mask_for_linear_edges，用于在对话框中设置只选择直线的遮罩。
> 9. 直线选择函数：定义了一个直线选择函数select_a_linear_edge，用于让用户选择一条直线，并返回该直线的tag。
> 10. RPO约束创建函数：定义了一个RPO约束创建函数rpo_pad，用于在矩形垫片特征上创建自定义的RPO约束。
> 11. 主函数：定义了一个主函数do_it，用于实现创建矩形垫片的主要流程，包括注册自定义RPO约束函数、循环选择面创建矩形垫片、注销自定义RPO约束函数等。
> 12. NX初始化和终止函数：定义了ufusr函数，用于在NX启动时初始化并调用主函数，在NX关闭时终止。
> 13. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于返回该库不能被立即卸载，必须在UG终止时卸载。
>
> 以上是该代码的主要功能，通过这些函数和流程，实现了在NX中创建矩形垫片并使用自定义的RPO约束优化。
>
