### 【1278】find extreme point on selected curve 在选定的曲线上查找极点

#### 代码

```cpp
    /*HEAD FIND_EXTREME_POINT_ON_SELECTED_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog在V18版本中是新增的。因此，翻译为：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
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
    static int ask_feat_faces(tag_t feat, tag_p_t *feat_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        return make_an_array(&face_list, feat_faces);  
    }  
    static void find_curve_extreme_point(tag_t curve, double dir[3],  
        double point[3])  
    {  
        int  
            n_faces,  
            n_feats;  
        tag_t  
            *feats,  
            *faces;  
        double  
            mtx[9];  
        char  
            *limits[2] = { "0.0", "1.0" };  
        UF_UNDO_mark_id_t  
            mark_id;  
        uf_list_p_t  
            curve_list,  
            feat_list;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_MTX3_initialize_x(dir, mtx));  
        curve_list = make_a_list(1, &curve);  
        UF_CALL(UF_MODL_create_extruded(curve_list, "0", limits, point,  
            &mtx[6], UF_NULLSIGN, &feat_list));  
        UF_CALL(UF_MODL_delete_list(&curve_list));  
        if (((n_feats = make_an_array(&feat_list, &feats)) > 0) &&  
            ((n_faces = ask_feat_faces(feats[0], &faces)) > 0))  
        {  
            UF_CALL(UF_MODL_ask_extreme(faces[0], mtx, &mtx[3], &mtx[6],  
                &curve, point));  
            UF_free(feats);  
            UF_free(faces);  
        }  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            dir[3],  
            point[3];  
        while (((curve = select_a_curve("Find extreme point")) != NULL_TAG) &&  
            specify_vector("Extreme direction", dir, point))  
        {  
            find_curve_extreme_point(curve, dir, point);  
            display_temporary_point(point);  
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

> 这段代码是一个NX Open C++用户函数，用于在NX中查找曲线的极值点。主要功能包括：
>
> 1. 选择曲线：使用对话框让用户选择一个曲线对象，并高亮显示。
> 2. 指定方向：使用对话框让用户指定一个方向向量。
> 3. 创建拉伸特征：使用选中的曲线和方向向量，创建一个拉伸特征，并获取拉伸特征的面。
> 4. 查找极值点：在拉伸特征的面中，使用给定的曲线和方向向量，查找曲线的极值点。
> 5. 显示极值点：在NX中显示找到的极值点。
> 6. 重复执行：用户可以重复选择曲线和方向，程序会继续查找极值点并显示。
>
> 主要的数据结构包括：
>
> 通过这个函数，用户可以很方便地在NX中查找曲线的极值点，并进行后续的分析或操作。
>
