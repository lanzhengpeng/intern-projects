### 【0850】create quilt using curve mesh along driver normals 创建沿驱动法线方向的曲线网格贴图

#### 代码

```cpp
    /*HEAD CREATE_QUILT_USING_CURVE_MESH_ALONG_DRIVER_NORMALS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog是V18版本新增的函数，其主要作用是打印系统日志。需要注意的是，该函数是V18版本新增的，因此在V18之前的版本中不可用。在V18及之后的版本中，UF_print_syslog可用于打印系统日志，帮助调试系统问题。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[5];  
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
        mask[4].object_type = UF_solid_type;  
        mask[4].object_subtype = 0;  
        mask[4].solid_type = UF_UI_SEL_FEATURE_ANY_EDGE;  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    static tag_t select_single_curve(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            curve,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_curves, NULL, &resp, &curve, cp,  
            &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, 0));  
            return curve;  
        }  
        else return NULL_TAG;  
    }  
    static void specify_string(char *which, UF_STRING_t *string)  
    {  
        int  
            ii = 0,  
            jj = 0,  
            kk = -1,  
            n,  
            n_segs[10];  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        do  
        {  
            if ((kk > -1) && (curves[kk] == NULL_TAG))  
            {  
                n_segs[ii] = jj-1;  
                ii++;  
                jj = 0;  
                kk--;  
            }  
            if (jj > 0)  
            {  
                sprintf(prompt, "OK to finish selecting string #%d", ii+1);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else if (ii > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s strings", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            sprintf(prompt, "Select %s String #%d - curve #%d", which, ii+1, ++jj);  
            curves[++kk] = select_single_curve(prompt);  
        } while ((curves[kk] != NULL_TAG) || jj > 1);  
        UF_CALL(UF_UI_set_status(""));  
        n_segs[ii] = jj;  
        n = ii;  
        UF_MODL_init_string_list(string);  
        UF_MODL_create_string_list(n, kk, string);  
        for (ii = 0; ii < n; ii++)  
        {  
            string->string[ii] = n_segs[ii];  
            string->dir[ii] = 1;  
        }  
        for (ii = 0; ii < kk; ii++)  
            string->id[ii] = curves[ii];  
    }  
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            ii,  
            irc,  
            n;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_FACE;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &irc, &n, faces));  
        for (ii=0; ii<n; ii++)  
            UF_CALL(UF_DISP_set_highlight(*(*faces + ii), FALSE));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            tol_achieved;  
        tag_t  
            quilt;  
        UF_STRING_t  
            cross,  
            primary;  
        UF_MODL_quilt_data_structures_u  
            quilt_struct;  
        UF_MODL_curve_mesh_along_driver_normals_data_t  
            data;  
        quilt_struct.quilt_type2 = &data;  
        data.quilt_data.check_overlap = UF_MODL_OVERLAP_CHECK_OFF;  
        data.quilt_data.inside_distance = "0.01";  
        data.quilt_data.inside_angle = "90.0";  
        data.quilt_data.edge_distance = "0.01";  
        data.quilt_data.edge_angle = "90.0";  
        data.projection_limit = "10.5";  
        data.primary_curves = &primary;  
        data.cross_curves = &cross;  
        specify_string("Primary", data.primary_curves);  
        specify_string("Cross", data.cross_curves);  
        data.quilt_data.num_target_faces =  
            select_faces("Select faces for quilt", &data.quilt_data.target_faces);  
        UF_CALL(UF_MODL_create_quilt(UF_MODL_CURVE_MESH_ALONG_DRIVER_NORMALS,  
            &quilt_struct, &quilt, &tol_achieved));  
        UF_MODL_free_string_list(data.primary_curves);  
        UF_MODL_free_string_list(data.cross_curves);  
        UF_free(data.quilt_data.target_faces);  
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

> 这段NX二次开发代码的主要功能是创建沿着驱动法线方向的曲线网格的曲面。具体来说，代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 2. 定义了mask_for_curves函数，用于设置选择曲线时的过滤条件，只允许选择线、圆、圆锥、样条线和实体边线。
> 3. 定义了select_single_curve函数，用于弹出一个对话框让用户选择单个曲线。
> 4. 定义了specify_string函数，用于让用户选择一系列曲线，并存储为字符串列表。
> 5. 定义了select_faces函数，用于让用户选择一系列面，并存储为面列表。
> 6. 在do_it函数中，首先初始化曲线网格曲面的参数，包括重叠检查、距离和角度公差、投影限制等。然后调用specify_string函数选择主曲线和交叉曲线，接着选择目标面。最后调用UF_MODL_create_quilt函数创建曲线网格曲面。
> 7. ufusr函数是主函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数用于卸载用户自定义程序。
>
> 总的来说，这段代码实现了创建沿着驱动法线方向的曲线网格曲面的功能，用户可以通过选择曲线和面来指定网格的形状。
>
