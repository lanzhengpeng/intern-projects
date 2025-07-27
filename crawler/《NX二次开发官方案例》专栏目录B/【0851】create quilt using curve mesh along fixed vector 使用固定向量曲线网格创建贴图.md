### 【0851】create quilt using curve mesh along fixed vector 使用固定向量曲线网格创建贴图

#### 代码

```cpp
    /*HEAD CREATE_QUILT_USING_CURVE_MESH_ALONG_FIXED_VECTOR CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 新增的。 */  
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
        UF_MODL_curve_mesh_along_fixed_vector_data_t  
            data;  
        quilt_struct.quilt_type1 = &data;  
        data.quilt_data.check_overlap = UF_MODL_OVERLAP_CHECK_OFF;  
        data.quilt_data.inside_distance = "0.01";  
        data.quilt_data.inside_angle = "90.0";  
        data.quilt_data.edge_distance = "0.01";  
        data.quilt_data.edge_angle = "90.0";  
        data.projection_vector[0] = 0.0;  
        data.projection_vector[1] = 0.0;  
        data.projection_vector[2] = 1.0;  
        data.primary_curves = &primary;  
        data.cross_curves = &cross;  
        specify_string("Primary", data.primary_curves);  
        specify_string("Cross", data.cross_curves);  
        data.quilt_data.num_target_faces =  
            select_faces("Select faces for quilt", &data.quilt_data.target_faces);  
        UF_CALL(UF_MODL_create_quilt(UF_MODL_CURVE_MESH_ALONG_FIXED_VECTOR,  
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

> 这段代码是用于在NX中创建曲线网格面(Quilt)的二次开发代码，其主要功能如下：
>
> 1. 头文件包含：代码包含了必要的NX头文件，如uf.h, uf_ui.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于打印出错误信息。
> 3. 选择曲线的掩码：定义了一个函数mask_for_curves，用于设置选择曲线时的掩码。
> 4. 选择单条曲线：定义了一个函数select_single_curve，用于选择单条曲线。
> 5. 选择字符串：定义了一个函数specify_string，用于选择一组曲线字符串。
> 6. 选择面：定义了一个函数select_faces，用于选择一组面。
> 7. 创建Quilt：定义了一个函数do_it，用于执行创建Quilt的主要逻辑，包括选择主曲线、交叉曲线、目标面，并调用UF_MODL_create_quilt接口创建Quilt。
> 8. ufusr函数：这是NX二次开发的入口函数，用于初始化和调用do_it函数。
> 9. 卸载函数：定义了一个卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码实现了选择曲线、面，并创建曲线网格面的功能，通过设置不同的参数，可以创建不同类型的Quilt。
>
