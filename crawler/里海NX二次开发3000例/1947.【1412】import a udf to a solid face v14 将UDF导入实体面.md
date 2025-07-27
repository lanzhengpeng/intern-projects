### 【1412】import a udf to a solid face v14 将UDF导入实体面

#### 代码

```cpp
    /*HEAD IMPORT_A_UDF_TO_A_SOLID_FACE CCC UFUN */  
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
    #include <uf_undo.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数。

因此，可以简洁地回答：UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            num_values,  
            mark;  
        tag_t  
            body,  
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
        while (((body = select_a_solid("Select target body")) != NULL_TAG) &&  
            ((face = select_placement_face("Select face for udf", loc, dest_csys))  
            != NULL_TAG))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
            if (!UF_CALL(UF_MODL_import_udf("parts\\subtract.udf", dest_csys, loc,  
                &udf)))  
            {  
                UF_CALL(UF_MODL_ask_udf_parms(udf, &udf_name, &udf_prompt,  
                    &udf_values, &num_values));  
                UF_CALL(UF_MODL_set_udf_parms(udf, udf_prompt, udf_values,  
                    num_values));  
                UF_free_string_array(num_values,udf_values);  
                UF_free_string_array(num_values,udf_prompt);  
                UF_free(udf_name);  
                UF_CALL(UF_OBJ_cycle_by_name("TOOL_FACE", &tool_face));  
                if (UF_CALL(UF_MODL_boolean_udf_1(udf, face, tool_face, dest_csys,  
                        0, NULL, 0, NULL, FALSE, body, FALSE, &meta_id)))  
                    UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                else  
                {  
                    UF_CALL(UF_OBJ_set_name(meta_id, "META_ID"));  
                    UF_CALL(UF_MODL_update());  
                }  
            }  
        }  
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
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现以下功能：
>
> 1. 选择一个目标体和一个放置面，将UDF(用户定义特征)导入到目标体的放置面上。
> 2. UDF文件名为"subtract.udf"，位于"parts"目录下。
> 3. 选择目标体时，只接受实体体类型。
> 4. 选择放置面时，可以选择实体面或基准面，并计算放置面的坐标系。
> 5. UDF参数默认为"subtract.udf"文件中的参数，也可以在后续进行修改。
> 6. 在目标体的放置面上布尔减去UDF，生成新的特征。
> 7. 如果布尔运算失败，则撤销操作，否则更新模型。
> 8. UDF的TOOL_FACE面用于布尔减运算。
> 9. 通过UF_UNDO进行撤销管理。
> 10. 提供了卸载函数。
>
> 总体来说，这段代码实现了在NX中通过UDF进行布尔减运算的功能。
>
