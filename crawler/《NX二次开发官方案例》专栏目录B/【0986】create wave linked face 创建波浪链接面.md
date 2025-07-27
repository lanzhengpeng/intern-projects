### 【0986】create wave linked face 创建波浪链接面

#### 代码

```cpp
    /*HEAD CREATE_WAVE_LINKED_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18版本中新增的函数。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void show_intersection_points(int n, UF_MODL_intersect_info_p_t *ints)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n; ii++)  
        {  
            if (ints[ii]->intersect_type == UF_MODL_INTERSECT_POINT)  
                display_temporary_point(ints[ii]->intersect.point.coords);  
            UF_free(ints[ii]);  
        }  
        if (n > 0) UF_free(ints);  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            curve,  
            face,  
            link_face,  
            feat,  
            proto,  
            xform;  
        double  
            tol;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while ((face = select_a_face("Select face to link")) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(face))  
            {  
                proto = UF_ASSEM_ask_prototype_of_occ(face);  
                UF_CALL(UF_SO_create_xform_assy_ctxt(face,  
                    UF_ASSEM_ask_part_occurrence(face), NULL_TAG, &xform));  
                UF_CALL(UF_WAVE_create_linked_face(proto,xform,face,TRUE,&feat));  
            }  
            else  
                UF_CALL(UF_WAVE_create_linked_face(face,NULL_TAG,face,TRUE,&feat));  
            UF_CALL(UF_WAVE_ask_linked_feature_geom(feat, &link_face));  
            curve = select_a_curve("Select curve to intersect");  
            if (curve != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_intersect_objects(curve, face, tol, &n, &ints));  
                show_intersection_points(n, ints);  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 提示用户选择一个面，并将其链接到当前部件中，以创建一个链接面特征。
> 2. 提示用户选择一个曲线，并计算该曲线与链接面特征的交点，并高亮显示这些交点。
>
> 主要步骤包括：
>
> 总的来说，这段代码通过用户交互，实现了创建链接面特征和显示交点的功能。
>
