### 【0848】create proj curves alternate 创建投影曲线的备选方案

#### 代码

```cpp
    /*HEAD CREATE_PROJ_CURVES_ALTERNATE CCC UFUN */  
    /*  
        This uses UF_MODL_create_proj_curves(), an alternative to  
        UF_CURVE_create_proj_curves().  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能。 */  
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
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    /* 里海 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_face_plane_datums(char *prompt, tag_t **items)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select face/plane/datum", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_face_plane_datums, NULL, &resp, &cnt, items));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*items)[ii], FALSE));  
        return cnt;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            n_faces,  
            inx = 0;  
        tag_t  
            *curves,  
            *faces,  
            proj_curve_feature = NULL_TAG;  
        uf_list_p_t  
            face_refs,  
            curve_refs;  
        UF_UNDO_mark_id_t  
            mark;  
        while (((n_curves = select_curves("Curves to project", &curves )) > 0) &&  
            ((n_faces = select_face_plane_datums("Project onto", &faces )) > 0))  
        {  
            UF_CALL(UF_MODL_create_list(&curve_refs));  
            for( inx = 0; inx < n_curves; inx++ )  
            {  
                UF_CALL(UF_MODL_put_list_item( curve_refs, curves[inx] ));  
            }  
            UF_CALL(UF_MODL_create_list(&face_refs));  
            for( inx = 0; inx < n_faces; inx++ )  
            {  
                UF_CALL(UF_MODL_put_list_item( face_refs, faces[inx] ));  
            }  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Project Curves", &mark));  
            UF_CALL(UF_MODL_create_proj_curves( curve_refs, face_refs, 1,  
                NULL, &proj_curve_feature ));  
            UF_CALL(UF_MODL_delete_list(&face_refs));  
            UF_CALL(UF_MODL_delete_list(&curve_refs));  
            UF_free(curves);  
            UF_free(faces);  
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

> 这段代码是NX的二次开发代码，主要实现了曲线投影的功能。以下是主要步骤和功能的介绍：
>
> 1. 错误报告函数：定义了report_error函数，用于在函数调用失败时报告错误信息。
> 2. 曲线选择：定义了mask_for_curves和select_curves函数，用于设置选择掩码并选择要投影的曲线。
> 3. 平面选择：定义了mask_for_face_plane_datums和select_face_plane_datums函数，用于设置选择掩码并选择投影的平面。
> 4. 向量输入：定义了specify_vector函数，用于用户指定一个向量。
> 5. 曲线投影：do_it函数中实现了主要功能。首先，调用select_curves和select_face_plane_datums选择曲线和平面。然后，创建曲线和平面的列表，并调用UF_MODL_create_proj_curves函数进行投影。最后，删除列表并释放内存。
> 6. 主函数：ufusr函数首先调用UF_initialize初始化NX环境，然后调用do_it执行曲线投影功能，最后调用UF_terminate终止NX环境。
> 7. 卸载函数：ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示该库可以被立即卸载。
>
> 总体来看，该代码通过二次开发实现了NX中的曲线投影功能。
>
