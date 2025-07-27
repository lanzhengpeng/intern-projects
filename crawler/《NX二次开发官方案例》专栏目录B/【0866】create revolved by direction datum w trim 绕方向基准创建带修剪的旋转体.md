### 【0866】create revolved by direction datum w trim 绕方向基准创建带修剪的旋转体

#### 代码

```cpp
    /*HEAD CREATE_REVOLVED_BY_DIRECTION_DATUM_W_TRIM CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_vec.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能。

UF_print_syslog是V18版本新增的功能。 */  
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
    static void ask_curve_ends(tag_t curve, double ends[6])  
    {  
        double  
            junk[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, &ends[0], junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, &ends[3], junk, junk, junk,  
            junk, junk));  
    }  
    static void sort_end_to_end(tag_t *curves, int n_curves)  
    {  
        int  
            ii,  
            jj,  
            kk,  
            sofar = 1;  
        double  
            free_ends[6],  
            gap,  
            this_ends[6],  
            tol;  
        tag_t  
            temp;  
        UF_MODL_ask_distance_tolerance(&tol);  
        ask_curve_ends(curves[0], free_ends);  
        for (ii = 1; ii < n_curves; ii++)  
        {  
            ask_curve_ends(curves[ii], this_ends);  
            for (jj = 0; jj < 4; jj = jj + 3)  
            {  
                UF_VEC3_distance(&this_ends[jj], &free_ends[0], &gap);  
                if (gap <= tol)  
                {  
                    UF_VEC3_copy(&this_ends[abs(jj-3)], &free_ends[0]);  
                    temp = curves[ii];  
                    for (kk = ii; kk > 0; kk--)  
                        curves[kk] = curves[kk-1];  
                    curves[0] = temp;  
                    ii = sofar;  
                    sofar++;  
                    break;  
                }  
                UF_VEC3_distance(&this_ends[jj], &free_ends[3], &gap);  
                if (gap <= tol)  
                {  
                    UF_VEC3_copy(&this_ends[abs(jj-3)], &free_ends[3]);  
                    temp = curves[sofar];  
                    curves[sofar] = curves[ii];  
                    curves[ii] = temp;  
                    ii = sofar;  
                    sofar++;  
                    break;  
                }  
            }  
        }  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            jj,  
            resp,  
            subtype,  
            type;  
        tag_t  
            *objs;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask[5];  
        opts.num_mask_triples = 5;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask[0].object_type = UF_line_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = 0;  
        mask[1].object_type = UF_circle_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        mask[2].object_type = UF_conic_type;  
        mask[2].object_subtype = UF_all_subtype;  
        mask[2].solid_type = 0;  
        mask[3].object_type = UF_spline_type;  
        mask[3].object_subtype = 0;  
        mask[3].solid_type = 0;  
        mask[4].object_type = UF_group_type;  
        mask[4].object_subtype = 0;  
        mask[4].solid_type = 0;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
        {  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
            /* Don't return any groups - just the curves 译:不要返回任何组 - 只返回曲线。 */  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objs[ii], &type, &subtype));  
            if (type == UF_group_type)  
            {  
                cnt--;  
                for (jj = ii; jj < cnt; jj++) objs[jj] = objs[jj+1];  
                ii--;  
            }  
        }  
        return cnt;  
    }  
    /* 里海 */  
    static int mask_for_datum_axes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_axis_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_axes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_planar_face_datum(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face_or_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face_datum, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            n_feats;  
        tag_t  
            *curves,  
            datum_tag,  
            fpd_tag,   
            *feats;  
        uf_list_p_t  
            curve_list;  
        char  
            *offsets[2] = { "0", "0" };  
        double  
            junk[3] = { 0,0,0 };  
        UF_MODL_SWEEP_TRIM_object_t  
            trim_data = { NULL, 1, UF_MODL_SWEEP_TRIM_TO_FACE, NULL, 0 };  
        trim_data.trim_objects = &fpd_tag;  
        while (((n_curves = select_curves("Select curves to revolve", &curves)) > 0)  
            && ((datum_tag = select_a_datum_axis("Select datum axis to revolve around")) != NULL_TAG)  
            && ((fpd_tag = select_planar_face_or_datum("Select face or datum")) != NULL_TAG))  
        {  
            sort_end_to_end(curves, n_curves); /* Work around for PR 679402 译:Work around for PR 679402的中文翻译为：

针对PR 679402的解决方法。 */  
            UF_CALL(UF_MODL_create_list(&curve_list));  
            for (ii = 0; ii < n_curves; ii++)  
                UF_CALL(UF_MODL_put_list_item(curve_list, curves[ii]));  
            UF_MODL_create_revolution_dir(curves, n_curves, &trim_data, NULL, offsets, junk,   
                FALSE, TRUE, datum_tag, UF_POSITIVE, &feats, &n_feats);  
            UF_free(feats);  
            UF_free(curves);  
            UF_CALL(UF_MODL_delete_list(&curve_list));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是NX Open C++的二次开发示例代码，主要功能是创建绕基准轴旋转的扫掠特征。具体步骤包括：
>
> 1. 选择曲线：使用UF_UI_select_by_class函数让用户选择曲线，并去除组。
> 2. 选择基准轴：使用UF_UI_select_with_single_dialog函数让用户选择基准轴。
> 3. 选择平面或基准平面：使用UF_UI_select_with_single_dialog函数让用户选择平面或基准平面。
> 4. 对曲线排序：确保曲线按端点顺序排列。
> 5. 创建扫掠特征：使用UF_MODL_create_revolution_dir函数创建绕基准轴旋转的扫掠特征。
> 6. 处理函数：使用UF_initialize、UF_terminate等函数进行初始化和终止处理。
> 7. 错误处理：使用自定义函数report_error进行错误报告。
> 8. 回调函数：使用ufusr、ufusr_ask_unload等回调函数进行交互。
>
> 通过以上步骤，这段代码实现了NX的绕基准轴旋转扫掠特征的二次开发。
>
