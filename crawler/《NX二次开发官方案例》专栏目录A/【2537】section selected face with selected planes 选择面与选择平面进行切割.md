### 【2537】section selected face with selected planes 选择面与选择平面进行切割

#### 代码

```cpp
    /*HEAD SECTION_SELECTED_FACE_WITH_SELECTED_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，UF_print_syslog是一个在V18版本中新增的函数。 */  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static int mask_for_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_planes(char *prompt, tag_t **planes)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_planes, NULL, &resp, &cnt, planes));  
        objs = *planes;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int create_intersection_objects(tag_t obj1, tag_t obj2, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            point;  
        double  
            tol;  
        uf_list_p_t  
            object_list;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(UF_MODL_create_list(&object_list));  
        UF_CALL(UF_MODL_intersect_objects(obj1, obj2, tol, &n, &intersections));  
        for (ii = 0; ii < n; ii++)  
        {  
            switch (intersections[ii]->intersect_type)  
            {  
                case UF_MODL_INTERSECT_POINT:  
                    UF_CALL(UF_CURVE_create_point(  
                        intersections[ii]->intersect.point.coords, &point));  
                    UF_CALL(UF_MODL_put_list_item(object_list, point));  
                    break;  
                case UF_MODL_INTERSECT_COINCIDE:  
                    UF_CALL(UF_MODL_put_list_item(object_list,  
                        intersections[ii]->intersect.coincide.identifier));  
                    break;  
                case UF_MODL_INTERSECT_CURVE:  
                    UF_CALL(UF_MODL_put_list_item(object_list,  
                        intersections[ii]->intersect.curve.identifier));  
                    break;  
                default:  
                    break;  
            }  
            UF_free(intersections[ii]);  
        }  
        if (n > 0) UF_free(intersections);  
        return make_an_array(&object_list, objects);  
    }  
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        logical  
            delete = TRUE;  
        int  
            ii,  
            n_curves,  
            n_planes;  
        tag_t  
            *curves,  
            face,  
            *planes;  
        while (((face = select_a_face("Select face to section")) != NULL_TAG) &&  
            ((n_planes = select_planes("Select section planes", &planes)) > 0))  
        {  
            for (ii = 0; ii < n_planes; ii++)  
            {  
                n_curves = create_intersection_objects(face, planes[ii], &curves);  
                if (n_curves > 0)  
                {  
                    set_highlight_object_array(n_curves, curves, TRUE);  
                    UF_free(curves);  
                }  
            }  
            if (ask_yes_or_no("Section objects highlighted, delete planes?",  
                &delete) && (delete == TRUE))  
                UF_CALL(UF_OBJ_delete_array_of_objects(n_planes, planes, NULL));  
            UF_free(planes);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        }  
    }  
    /* qq3123197280 */  
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

> 这段代码是NX Open C++的二次开发代码，实现了以下功能：
>
> 1. 提示用户选择一个面和一个或多个平面。
> 2. 计算面和平面的交线。
> 3. 高亮显示交线。
> 4. 提示用户是否删除平面。
> 5. 如果用户选择删除，则删除选定的平面。
> 6. 重复执行上述步骤，直到用户选择不继续。
>
> 主要函数包括：
>
> 代码首先初始化NX环境，然后进入主循环，不断提示用户选择面和平面，直到用户选择不继续。在选择完面和平面后，计算它们的交线并高亮显示。最后提示用户是否删除平面，如果删除则调用UF_OBJ_delete_array_of_objects删除。在主循环结束后，清理环境并终止NX环境。
>
