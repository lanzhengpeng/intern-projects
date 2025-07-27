### 【1417】import and rpo a udf v14 在 NX 14 中导入并应用 UDF

#### 代码

```cpp
    /*HEAD IMPORT_AND_RPO_A_UDF CCC UFUN */  
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
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_mtx.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数，主要用于打印系统日志。 */  
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
    static tag_t select_placement_face(char *prompt, double *loc, double *csys)  
    {  
        int  
            subtype,  
            type,  
            resp;  
        tag_t  
            face,  
            feat,  
            line,  
            view;  
        UF_CURVE_line_t  
            lp;  
        UF_UI_mask_t  
            mask[2];  
        UF_UI_selection_options_t  
            opts;  
        double  
            dirU2[3],  
            dirV2[3],  
            dist,  
            parm[2],  
            pnt[3],  
            radii[2],  
            z[3];  
        char  
            *angle,  
            *offset;  
        opts.num_mask_triples = 2;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask[0].object_type = UF_solid_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = UF_UI_SEL_FEATURE_PLANAR_FACE;  
        mask[1].object_type = UF_datum_plane_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &face, loc, &view));  
        if (resp == 4 || resp == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
            lp.start_point[0] = loc[0];  
            lp.start_point[1] = loc[1];  
            lp.start_point[2] = loc[2] + 1000;  
            lp.end_point[0] = loc[0];  
            lp.end_point[1] = loc[1];  
            lp.end_point[2] = loc[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_CALL(UF_MODL_ask_minimum_dist(face,line,0,loc,0,loc,&dist,loc,loc));  
            UF_CALL(UF_OBJ_delete_object(line));  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if (type == UF_solid_type)  
            {  
                UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
                UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, &csys[0], &csys[3],  
                    dirU2, dirV2, &csys[6], radii));  
            }  
            else  
            {  
                UF_CALL(UF_MODL_ask_object_feat(face, &feat));  
                UF_CALL(UF_MODL_ask_datum_plane_parms(feat,loc,z,&offset,&angle));  
                UF_CALL(UF_MTX3_initialize_z(z, csys));  
                UF_free(angle);  
                UF_free(offset);  
            }  
            UF_VEC3_negate(&csys[6], &csys[6]);  
            return face;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_linear_edge_or_dplane(char *prompt)  
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
        mask[0].solid_type = UF_UI_SEL_FEATURE_LINEAR_EDGE;  
        mask[1].object_type = UF_datum_plane_type;  
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
    static int rpo_udf(tag_t feat)  
    {  
        tag_t  
            target[2],  
            tool[2];  
        int  
            center[2] = { 0, 0 };  
        char  
            *constraints_array[2] = { "PERP_DIST_PARMS", "PARA_DIST_PARMS" },  
            *constraints_value[2] = { "0.0", "0.0" };  
        if ((target[0] = select_linear_edge_or_dplane("Select 1st target"))  
         && (tool[0] = select_linear_edge_or_dplane("Select corresponding tool"))  
         && (target[1] = select_linear_edge_or_dplane("Select 2nd target"))  
         && (tool[1] = select_linear_edge_or_dplane("Select corresponding tool")))  
            return(UF_MODL_create_rpo_constraints(feat, NULL_TAG, NULL_TAG,  
                target, center, tool, center, constraints_value,  
                constraints_array, 2));  
        else return (0);  
    }  
    static void do_it(void)  
    {  
        int  
            num_values,  
            mark;  
        tag_t  
            face,  
            meta_id,  
            tool_face = NULL_TAG,  
            udf;  
        double  
            loc[3],  
            dest_csys[9];  
        char  
            **udf_prompt,  
            **udf_values,  
            *udf_name;  
        UF_CALL(UF_MODL_register_rpo_routine(rpo_udf));  
        while ((face = select_placement_face("Select placement face", loc,  
            dest_csys)) != NULL_TAG)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
            UF_CALL(UF_MODL_import_udf("parts/udf.udf", dest_csys, loc, &udf));  
            UF_CALL(UF_MODL_ask_udf_parms(udf, &udf_name, &udf_prompt, &udf_values,  
                &num_values));  
            UF_CALL(UF_MODL_set_udf_parms(udf, udf_prompt, udf_values, num_values));  
            UF_free_string_array(num_values,udf_values);  
            UF_free_string_array(num_values,udf_prompt);  
            UF_free(udf_name);  
            UF_CALL(UF_OBJ_cycle_by_name("TOOL_FACE", &tool_face));  
            if (UF_CALL(UF_MODL_boolean_udf(udf, face, tool_face, dest_csys,  
                    0, NULL, 0, NULL, FALSE, &meta_id)))  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
            else  
                UF_CALL(UF_MODL_update());  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出错误信息到系统日志和窗口。
> 2. 视图坐标系转换函数：定义了一个视图坐标系到绝对坐标系的转换函数map_view2abs，用于将用户在视图坐标系下选择的点转换为绝对坐标系下的点。
> 3. 选择放置面函数：定义了一个选择放置面函数select_placement_face，用于选择一个放置面，并获取其坐标系。
> 4. 选择线性边缘或基准平面函数：定义了一个选择线性边缘或基准平面的函数select_linear_edge_or_dplane。
> 5. RPO约束函数：定义了一个RPO约束函数rpo_udf，用于创建RPO约束。
> 6. 主体函数：定义了主体函数do_it，用于遍历选择放置面，导入UDF，设置UDF参数，创建布尔操作，并更新模型。
> 7. 入口函数：定义了入口函数ufusr，用于初始化NX系统，调用主体函数，然后终止NX系统。
> 8. 卸载询问函数：定义了卸载询问函数ufusr_ask_unload，用于询问是否可以卸载该程序，由于使用了NX内部函数，因此该程序不能被卸载。
>
> 总体而言，该代码实现了UDF的导入、参数设置、布尔操作等功能，并包含了错误处理和坐标系转换等辅助功能。
>
