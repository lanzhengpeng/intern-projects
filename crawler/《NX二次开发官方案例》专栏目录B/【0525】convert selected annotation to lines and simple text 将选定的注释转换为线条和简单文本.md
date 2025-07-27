### 【0525】convert selected annotation to lines and simple text 将选定的注释转换为线条和简单文本

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_undo.h>  
    #include <uf_csys.h>  
    #include <uf_cgm.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
    #include <uf_drf.h>  
    #include <uf_vec.h>  
    #include <uf_layer.h>  
    #include <uf_modl.h>  
    #include <uf_layout.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_annotations(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 },  
                        { UF_tabular_note_type, UF_all_subtype, 0 },  
                        { UF_tol_feature_instance_type, 0, 0} };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_annotations(char *prompt, tag_t **annotations)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select annotations", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_annotations, NULL, &resp, &cnt,  
            annotations));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*annotations)[ii], FALSE));  
        return cnt;  
    }  
    static void unblank_all_objects_on_selectable_layers(void)  
    {  
        tag_t  
            object = NULL_TAG;  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object)) &&  
            (object != NULL_TAG))  
            UF_CALL(UF_OBJ_set_blank_status(object, UF_OBJ_NOT_BLANKED));  
    }  
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
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
    static int ask_all_visible_objects(tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG,  
            view;  
        uf_list_p_t  
            list;  
        if (UF_CALL(UF_VIEW_ask_work_view(&view))) return 0;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        return make_an_array(&list, objects);  
    }  
    static void map_abs2view(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void set_wcs_to_current_view_lower_left_corner()  
    {  
        tag_t  
            csys,                       /* Csys object  译:根据文档内容，Csys object指的是坐标系统对象。 */  
            matobj,                     /* Matrix object 译:矩阵对象 */  
            wv;  
        double  
            bounds[4],  
            origin[3],                  /* View's center 译:View的中心 */  
            matrix[9],                  /* View's matrix 译:视图的矩阵 */  
            scale;  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        UF_CALL(UF_VIEW_ask_work_view(&wv));  
        UF_CALL(UF_VIEW_ask_current_xy_clip(wv, bounds));  
        map_abs2view(origin, wv);  
        origin[0] = origin[0] + bounds[0];  
        origin[1] = origin[1] + bounds[2];  
        origin[2] = 0;  
        map_view2abs(origin, wv);  
        if (!UF_CALL(UF_CSYS_create_matrix(matrix, &matobj)) &&  
            !UF_CALL(UF_CSYS_create_temp_csys(origin, matobj, &csys)))  
            UF_CALL(UF_CSYS_set_wcs(csys));  
    }  
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static UF_DRF_align_position_t set_drf_align_site(UF_DRF_align_position_t pos)  
    {  
        int  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        UF_DRF_align_position_t  
            old_site;  
        if (!UF_CALL(UF_DRF_ask_preferences(mpi, mpr, rad, dia)))  
        {  
            old_site = (UF_DRF_align_position_t)mpi[30];  
            mpi[30] = pos;  
            UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad, dia));  
        }  
        return old_site;  
    }  
    static int ask_all_visible_objects_of_type(int the_type, tag_t **objects)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG,  
            view;  
        uf_list_p_t  
            list;  
        if (UF_CALL(UF_VIEW_ask_work_view(&view))) return 0;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == the_type)  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }  
    static tag_t find_visible_note_with_text(char *text)  
    {  
        int  
            ii, jj, kk,  
            n,  
            n_text;  
        tag_t  
            a_note = NULL_TAG,  
            *notes;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        n = ask_all_visible_objects_of_type(UF_drafting_entity_type, &notes);  
        for (kk = 0; kk < n; kk++)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(notes[kk], &n_text, &text_info));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                for (jj = 0; jj < text_info[ii].num_lines; jj++)  
                    if (strstr(text_info[ii].text[jj].string, text) != NULL)  
                        break;  
                if (jj < text_info[ii].num_lines) break;  
            }  
            UF_CALL(UF_DRF_free_text(n_text, &text_info));  
            if (ii < n_text)  
            {  
                a_note = notes[kk];  
                break;  
            }  
        }  
        if (n > 0) UF_free(notes);  
        return a_note;  
    }  
    /* This is helpful in obtaining WYSIWYG text and/or leader line info 译:这有助于获取所见即所得文本和/或引线信息。 */  
    static tag_t convert_annotation_to_lines_and_simple_text(tag_t annotation)  
    {  
        int  
            n_views,  
            n_new,  
            status;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        tag_t  
            grp,  
            *new_objects,  
            note,  
            wv;  
        double  
            dest_csys[9] = { 1,0,0, 0,1,0, 0,0,1 },  
            dest_pnt[3] = { 0, 0, 0 },  
            offset[3];  
        char  
            temp_cgm[MAX_FSPEC_SIZE+1],  
            temp_prt[MAX_FSPEC_SIZE+1],  
            *text[1],  
            unique[MAX_ENTITY_NAME_SIZE+1],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CGM_export_options_t  
            defaults;  
        UF_import_part_modes_t  
            modes = { 1, 1, 0, 3, 0, FALSE, 0 };  
        UF_DRF_align_position_t  
            old_align;  
        text[0] = unique;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        unblank_all_objects_on_selectable_layers();  
        UF_CALL(UF_OBJ_set_blank_status(annotation, UF_OBJ_BLANKED));  
        UF_CALL(UF_OBJ_reverse_blank_all());  
        UF_CALL(UF_VIEW_ask_work_view(&wv));  
        UF_CALL(UF_VIEW_ask_type(wv, &type, &subtype));  
        if (type == UF_VIEW_DRAWING_SHEET_TYPE)  
        {  
            UF_CALL(uc4577(unique));  
            old_align = set_drf_align_site(UF_DRF_ALIGN_BOTTOM_LEFT);  
            UF_CALL(UF_DRF_create_note(1, text, dest_pnt, 0, &note));  
            old_align = set_drf_align_site(old_align);  
        }  
        else  
        {  
            uc6472("", &n_views);  
            if (n_views > 1)  
            {  
                uc6409(annotation, &status, view_name);  
                if (status == 0) uc6448(view_name);  
                uc6468("L1", 2, 0);  
                uc6464("", "", view_name);  
            }  
            uc6434("", 3, annotation, NULL);  
            uc6432("", 1);  
            set_wcs_to_current_view_lower_left_corner();  
            ask_wcs_info(dest_pnt, dest_csys);  
        }  
        build_unique_temp_name(temp_cgm, 94);  
        UF_CALL(UF_CGM_ask_default_export_options(&defaults));  
        defaults.text_mode = UF_CGM_TEXT_AS_CHARACTERS;  
        UF_CALL(UF_CGM_export_cgm(NULL_TAG, &defaults, temp_cgm));  
        UF_CALL(UF_CGM_import_cgm(temp_cgm, NULL));  
        if (type == UF_VIEW_DRAWING_SHEET_TYPE)  
        {  
            note = find_visible_note_with_text(unique);  
            UF_CALL(UF_DRF_ask_origin(note, offset));  
            UF_VEC3_negate(offset, dest_pnt);  
            UF_OBJ_delete_object(note);  
        }  
        UF_CALL(UF_OBJ_set_blank_status(annotation, UF_OBJ_BLANKED));  
        n_new = ask_all_visible_objects(&new_objects);  
        build_unique_temp_name(temp_prt, 2);  
        UF_CALL(UF_PART_export(temp_prt, n_new, new_objects));  
        UF_free(new_objects);  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, ""));  
        uc6432("", 2);  
        UF_CALL(UF_PART_import(temp_prt, &modes, dest_csys, dest_pnt, 1.0, &grp));  
        UF_CALL(UF_DISP_regenerate_display());  
        UF_CALL(uc4561(temp_cgm, 0));  
        UF_CALL(uc4561(temp_prt, 0));  
        UF_CALL(UF_OBJ_delete_object(annotation));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, ""));  
        return grp;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *objects;  
        UF_UNDO_mark_id_t  
            mark_id;  
        while ((n = select_annotations("Convert annotations to lines and text",  
                &objects)) > 0)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible,  
                "Convert annotations to lines and text", &mark_id));  
        /*  If an annotation was associated to one that was previously  
            converted it might be deleted so check if its alive first */  
            for (ii = 0; ii < n; ii++)  
                if (UF_OBJ_ask_status(objects[ii]) == UF_OBJ_ALIVE)  
                    convert_annotation_to_lines_and_simple_text(objects[ii]);  
            UF_free(objects);  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是将NX中的注释转换为直线和文字。以下是代码的主要组成部分：
>
> 1. 头文件包含：代码包含了多个NX Open API的头文件，用于调用NX的函数。
> 2. 错误处理宏：定义了UF_CALL宏，用于调用NX API函数，并自动处理错误。
> 3. 注释选择：定义了mask_for_annotations函数，用于设置选择注释时的过滤条件。
> 4. 注释选择对话框：定义了select_annotations函数，用于显示对话框，让用户选择要转换的注释。
> 5. 临时文件命名：定义了build_unique_temp_name函数，用于生成临时文件的唯一名称。
> 6. 内存分配：定义了allocate_memory函数，用于分配内存。
> 7. 对象列表处理：定义了make_an_array函数，用于将对象列表转换为对象数组。
> 8. 获取可见对象：定义了ask_all_visible_objects函数，用于获取当前工作视图中所有可见的对象。
> 9. 坐标转换：定义了map_abs2view和map_view2abs函数，用于视图坐标系和绝对坐标系之间的转换。
> 10. 设置WCS：定义了set_wcs_to_current_view_lower_left_corner函数，用于设置WCS到当前视图左下角。
> 11. 获取WCS信息：定义了ask_wcs_info函数，用于获取当前WCS的信息。
> 12. 设置注释对齐位置：定义了set_drf_align_site函数，用于设置注释的对齐位置。
> 13. 查找可见注释：定义了find_visible_note_with_text函数，用于查找包含指定文本的可见注释。
> 14. 转换注释：定义了convert_annotation_to_lines_and_simple_text函数，用于将注释转换为直线和文字。
> 15. 主函数：定义了do_it函数，用于执行注释转换的主流程。
> 16. NX Open入口函数：定义了ufusr函数，作为NX Open的入口函数，用于初始化和调用主流程。
> 17. 卸载函数：定义了ufusr_ask_unload函数，用于处理插件卸载。
>
> 综上所述，这段代码实现了一个完整的NX Open二次开发示例，通过注释转换展示了NX Open API的典型应用。
>
