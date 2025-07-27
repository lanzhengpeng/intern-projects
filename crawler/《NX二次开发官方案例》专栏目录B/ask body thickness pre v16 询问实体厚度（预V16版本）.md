### ask body thickness pre v16 询问实体厚度（预V16版本）

#### 代码

```cpp
    /*HEAD ASK_BODY_THICKNESS_PRE_V16 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的函数。 */  
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
    static logical work_around_2(tag_t *object)  
    {  
        int  
            d_units,  
            p_units;  
        tag_t  
            comp,  
            feat,  
            part;  
        if (UF_ASSEM_is_occurrence(*object))  
        {  
            comp = UF_ASSEM_ask_part_occurrence(*object);  
            part = UF_ASSEM_ask_prototype_of_occ(comp);  
            UF_CALL(UF_PART_ask_units(part, &p_units));  
            UF_CALL(UF_PART_ask_units(UF_PART_ask_display_part(), &d_units));  
            if (p_units != d_units)  
            {  
                if (!UF_MODL_create_promotion(*object, &feat))  
                {  
                    UF_CALL(UF_MODL_ask_feat_body(feat, object));  
                    return TRUE;  
                }  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
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
    static void report_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%f", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            fix;  
        int  
            irc,  
            n,  
            one = 1,  
            two = 2,  
            which[1];  
        tag_t  
            body[1],  
            face;  
        double  
            ipc[6],  
            junk[6],  
            mx[9] = { 1,0,0, 0,1,0, 0,0,1 },  
            normal[3],  
            parm[2],  
            pos[3],  
            ray[3],  
            thickness,  
            vec[3] = { 0,0,0 };  
        char  
            messg[300];  
        while ((face=select_point_on_face("Select point on body", pos)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_face_body(face, &body[0]));  
            UF_CALL(UF_MODL_ask_face_parm(face, pos, parm, junk));  
            UF_CALL(UF_MODL_ask_face_props(face, parm, junk, junk, junk, junk,  
                junk, normal, junk));  
            UF_VEC3_negate(normal, ray);  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, pos, ray, 0);  
            fix = work_around_2(&body[0]);  
            FTN(uf6545)(body, &one, mx, vec, &two, pos, ray, &irc, &n, ipc,  
                junk, which);  
            if (fix) UF_CALL(UF_OBJ_delete_object(body[0]));  
            UF_CALL(irc);  
            if ((irc == 0) && (n > 1))  
            {  
                report_double_array("ipc", ipc, 2, 3);  
                UF_VEC3_distance(&ipc[0], &ipc[3], &thickness);  
                sprintf(messg, "Thickness = %f", thickness);  
                uc1601(messg, TRUE);  
            }  
        }  
    }  
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

> 这段代码是用于NX的二次开发，主要功能是计算并显示选中面的厚度。
>
> 主要步骤如下：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了坐标系转换函数map_abs2view和map_view2abs。
> 4. 定义了两个work_around函数，用于处理单位不统一的情况。
> 5. 定义了mask_for_faces函数，用于设置选择过滤条件，只选择实体类型和任意特征面。
> 6. 定义了select_point_on_face函数，用于提示用户选择一个面上的点。
> 7. 定义了report_double_array函数，用于打印数组。
> 8. 定义了do_it函数，是主函数，循环提示用户选择面，计算厚度并显示。
> 9. 定义了ufusr函数，是NX二次开发的入口函数。
> 10. 定义了ufusr_ask_unload函数，返回立即卸载。
>
> 整体来看，这段代码实现了选择面并计算显示厚度的功能。
>
