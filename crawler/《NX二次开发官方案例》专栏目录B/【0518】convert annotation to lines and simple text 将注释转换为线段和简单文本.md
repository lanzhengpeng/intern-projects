### 【0518】convert annotation to lines and simple text 将注释转换为线段和简单文本

#### 代码

```cpp
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
            csys,                       /* Csys object  译:Csys 对象的翻译是坐标系对象。 */  
            matobj,                     /* Matrix object 译:Matrix object */  
            wv;  
        double  
            bounds[4],  
            origin[3],                  /* View's center 译:View's center的翻译是视图的中心。 */  
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
    /* This is helpful in obtaining WYSIWYG text and/or leader line info 译:这有助于获取所见即所得文本和/或引导线信息。 */  
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

```

#### 代码解析

> 这段代码是用于将NX中的注释(Annotation)转换为线条和简单文本的二次开发代码。其主要功能包括：
>
> 1. 注释处理：首先，代码将指定的注释对象进行隐藏，并导出当前视图的CGM文件，然后导入该CGM文件以获取注释的线条和文本信息。
> 2. 工作视图处理：代码会检查当前视图是否为图纸视图，如果是，则创建一个临时的文本注释，然后将其转换为线条和文本。
> 3. CGM文件处理：对于非图纸视图，代码会导出当前视图的CGM文件，然后导入该文件，接着删除原注释对象，并导出当前视图的prt文件。
> 4. 导入处理：最后，代码导入之前导出的prt文件，得到转换后的线条和文本，并删除临时文件和注释对象。
> 5. 函数定义：代码还定义了多个辅助函数，用于处理视图、图层、坐标系等。
> 6. 注释转换：整体上，这段代码实现了将复杂的注释对象转换为简单的线条和文本的功能，有助于后续的二次开发处理。
>
