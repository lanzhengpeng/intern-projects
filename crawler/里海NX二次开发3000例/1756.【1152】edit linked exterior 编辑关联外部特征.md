### 【1152】edit linked exterior 编辑关联外部特征

#### 代码

```cpp
    /*HEAD EDIT_LINKED_EXTERIOR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_so.h>  
    #include <uf_vec.h>  
    #include <uf_linked_exterior.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    struct preselect_data_s {  
        int     n_items;  
        tag_t   *items;  
    };  
    typedef struct preselect_data_s preselect_data_t, *preselect_data_p_t;  
    /* qq3123197280 */  
    static int preselect_and_mask_for_faces(UF_UI_selection_p_t select,  
        void *user_data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        preselect_data_p_t  
            preselects;  
        preselects = (preselect_data_p_t) user_data;  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
                ((preselects->n_items == 0) ||  
                !UF_CALL(UF_UI_add_to_sel_list(select, preselects->n_items,  
                    preselects->items, TRUE))))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_and_deselect_faces(char *prompt, tag_t *preselects,  
        int n_preselects, tag_t **selections)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        preselect_data_t  
            presets;  
        presets.n_items = n_preselects;  
        presets.items = preselects;  
        UF_CALL(UF_UI_select_with_class_dialog("Select/Deselect faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, preselect_and_mask_for_faces,  
            (void *)&presets, &resp, &cnt, selections));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*selections)[ii], FALSE));  
        return cnt;  
    }  
    /* qq3123197280 */  
    static int preselect_and_mask_for_bodies(UF_UI_selection_p_t select,  
        void *user_data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        preselect_data_p_t  
            preselects;  
        preselects = (preselect_data_p_t) user_data;  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
                ((preselects->n_items == 0) ||  
                !UF_CALL(UF_UI_add_to_sel_list(select, preselects->n_items,  
                    preselects->items, TRUE))))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_and_deselect_bodies(char *prompt, tag_t *preselects,  
        int n_preselects, tag_t **selections)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        preselect_data_t  
            presets;  
        presets.n_items = n_preselects;  
        presets.items = preselects;  
        UF_CALL(UF_UI_select_with_class_dialog("Select/Deselect bodies", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, preselect_and_mask_for_bodies,  
            (void *)&presets, &resp, &cnt, selections));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*selections)[ii], FALSE));  
        return cnt;  
    }  
    static int reallocate_tag_array(int n_old, int n_new, tag_t **tag_array)  
    {  
        int  
            ii,  
            resp;  
        tag_t  
            *old_ptr = *tag_array;  
        *tag_array = UF_allocate_memory(n_new * sizeof(tag_t), &resp);  
        for (ii = 0; ii < n_old; ii++) (*tag_array)[ii] = old_ptr[ii];  
        UF_free(old_ptr);  
        return resp;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_item_array_index(int n_items, tag_t *items, tag_t item)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n_items; ii++)  
            if (items[ii] == item) return ii;  
        return -1;  
    }  
    static logical do_tag_arrays_match(int n_one, tag_t *one, int n_two, tag_t *two)  
    {  
        int  
            ii;  
        if (n_one != n_two) return FALSE;  
        for (ii = 0; ii < n_one; ii++)  
        {  
            if ((ask_item_array_index(n_two, two, one[ii])) > -1) continue;  
            return FALSE;  
        }  
        return TRUE;  
    }  
    static void free_linked_exterior_data(UF_MODL_linked_ext_p_t ext_data)  
    {  
        if (ext_data->num_bodies > 0)  
        {  
            UF_free(ext_data->bodies);  
            UF_free(ext_data->xforms);  
        }  
        if (ext_data->num_faces > 0)  
        {  
            UF_free(ext_data->faces);  
            UF_free(ext_data->xform_index);  
        }  
        ext_data->num_bodies = 0;  
        ext_data->bodies = NULL;  
        ext_data->xforms = NULL;  
        ext_data->num_faces = 0;  
        ext_data->faces = NULL;  
        ext_data->xform_index = NULL;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void suppress_a_feature(tag_t feature)  
    {  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        UF_CALL(UF_MODL_put_list_item(feat_list, feature));  
        UF_CALL(UF_MODL_suppress_feature(feat_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static UF_UNDO_mark_id_t rollback_for_edit(tag_t feat)  
    {  
        tag_t  
            next_feat = feat;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_MODL_ask_previous_feature(feat, &next_feat));  
        if ((next_feat == NULL_TAG) || (next_feat == feat)) /* see PR 2041495 里海译:翻译如下：

请查看 PR 2041495。 */  
        {  
            UF_CALL(UF_MODL_set_current_feature(feat));  
            suppress_a_feature(feat);  
        }  
        else  
            UF_CALL(UF_MODL_set_current_feature(next_feat));  
        return mark_id;  
    }  
    static tag_t ask_part_occ_of_prom_occ(tag_t prom_object)  
    {  
        tag_t  
            owning_part,  
            part_occ = prom_object,  
            proto;  
        proto = UF_ASSEM_ask_prototype_of_occ(prom_object);  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        while (!UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ))  
            && (UF_ASSEM_ask_prototype_of_occ(part_occ) != owning_part));  
        return part_occ;  
    }  
    static tag_t get_work_occurrence()  
    {  
        tag_t  
            work_occ = UF_ASSEM_ask_work_occurrence();  
        if (work_occ == NULL_TAG)  
            work_occ = UF_ASSEM_ask_root_part_occ(UF_ASSEM_ask_work_part());  
        return work_occ;  
    }  
    static void get_wave_source_and_xform(tag_t object, tag_t *source, tag_t *xform)  
    {  
        tag_t  
            disp_occ,  
            work_occ = get_work_occurrence(),  
            wp = UF_ASSEM_ask_work_part(),  
            wp_obj = UF_OBJ_cycle_all(wp, NULL_TAG);  
        if (UF_ASSEM_is_occurrence(object))  
            *source = UF_ASSEM_ask_prototype_of_occ(object);  
        else  
            *source = object;  
        if (UF_OBJ_is_object_a_promotion(*source))  
            disp_occ = ask_part_occ_of_prom_occ(object);  
        else  
            disp_occ = UF_ASSEM_ask_part_occurrence(object);  
        if ((disp_occ == NULL_TAG) || (disp_occ == work_occ))  
            *xform = NULL_TAG;  
        else  
            UF_CALL(UF_SO_create_xform_assy_ctxt(wp_obj, disp_occ, work_occ,  
                xform));  
    }  
    static void map_prototypes_and_xforms_to_occurrences(int *n, tag_t *protos,  
        tag_t *xforms, tag_t *occs)  
    {  
        logical  
            found;  
        int  
            ii, jj;  
        tag_t  
            dp = UF_PART_ask_display_part(),  
            disp_occ,  
            owner,  
            work_occ = get_work_occurrence(),  
            wp = UF_ASSEM_ask_work_part();  
        for (ii = 0; ii < *n; ii++)  
        {  
            found = FALSE;  
            UF_CALL(UF_OBJ_ask_owning_part(protos[ii], &owner));  
            if (owner == wp)  
            {  
                occs[ii] = UF_ASSEM_find_occurrence(work_occ, protos[ii]);  
                if (occs[ii] == NULL_TAG) occs[ii] = protos[ii];  
                xforms[ii] = NULL_TAG;  
                found = TRUE;  
            }  
            if (!found && (owner == dp))  
            {  
                occs[ii] = protos[ii];  
                get_wave_source_and_xform(occs[ii], &protos[ii],  
                    &xforms[ii]);  
                found = TRUE;  
            }  
            if (!found)  
            {  
                if (xforms[ii] == NULL_TAG)  
                {  
                    occs[ii] = UF_ASSEM_find_occurrence(work_occ, protos[ii]);  
                    if (occs[ii] != NULL_TAG) found = TRUE;  
                }  
                else  
                {  
                    UF_CALL(UF_SO_ask_assy_ctxt_part_occ(xforms[ii], work_occ,  
                        &disp_occ));  
                    if (disp_occ != NULL_TAG)  
                    {  
                        occs[ii] = UF_ASSEM_find_occurrence(disp_occ, protos[ii]);  
                        if (occs[ii] != NULL_TAG) found = TRUE;  
                    }  
                }  
            }  
            if (!found)  
            {  
                for (jj = ii; jj < *n-1; jj++)  
                {  
                    protos[jj] = protos[jj+1];  
                    xforms[jj] = xforms[jj+1];  
                }  
                *n = *n - 1;  
                ii--;  
            }  
        }  
    }  
    static logical do_assy_ctxt_xforms_match(tag_t one, tag_t two)  
    {  
        tag_t  
            one_occ,  
            two_occ,  
            work_occ = get_work_occurrence();  
        if (one == two) return TRUE; //  matching tags or NULL_TAGs  
        UF_CALL(UF_SO_ask_assy_ctxt_part_occ(one, work_occ, &one_occ));  
        UF_CALL(UF_SO_ask_assy_ctxt_part_occ(two, work_occ, &two_occ));  
        if (one_occ == two_occ) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_bodies = 0,  
            n_faces = 0,  
            n_groups = 0,  
            n_subfeats = 0;  
        tag_t  
            feat,  
            body,  
            *bodies,  
            *faces,  
            *groups = NULL,  
            face_xform,  
            *old_bodies,  
            *old_faces,  
            *old_xforms,  
            *subfeats = NULL;  
        double  
            chordal_tol,  
            directions[6][3] =  
                 { {1,0,0}, {0,1,0}, {0,0,1}, {-1,0,0}, {0,-1,0}, {0,0,-1}},  
            mass_props[47] = {0};  
        UF_MODL_linked_ext_t  
            ext_data = {0};  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_MODL_ask_distance_tolerance(&chordal_tol);  
        if ((feat = select_a_feature("Select Linked Exterior")) != NULL_TAG)  
        {  
            set_undo_mark("Edit Linked Exterior");  
            if (UF_CALL(UF_MODL_ask_linked_exterior(feat, &ext_data,  
                &n_groups, &groups, &n_subfeats, &subfeats, mass_props))) return;  
            if (n_groups > 0) UF_free(groups);  
            if (n_subfeats > 0) UF_free(subfeats);  
            UF_CALL(allocate_memory(ext_data.num_bodies * sizeof(tag_t),  
                (void **)&old_bodies));  
            map_prototypes_and_xforms_to_occurrences(&ext_data.num_bodies,  
                ext_data.bodies, ext_data.xforms, old_bodies);  
        /*  this prevents selection of bodies which are part of or were created  
            after the linked exterior.  They are invalid for participation in it.  
        */  
            mark_id = rollback_for_edit(feat);  
        /*  See PR 6062377 - promoted bodies not in the work part will not  
            automatically pre-select.  
        */  
            n_bodies = select_and_deselect_bodies(  
                "Select candidate bodies from which to extract faces",  
                old_bodies, ext_data.num_bodies, &bodies);  
            UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
            UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
            if ((n_bodies > 0) &&  
                !do_tag_arrays_match(ext_data.num_bodies, old_bodies, n_bodies,  
                bodies))  
            {  
                free_linked_exterior_data(&ext_data);  
                ext_data.num_bodies = n_bodies;  
                UF_CALL(allocate_memory(n_bodies * sizeof(tag_t),  
                    (void **)&ext_data.bodies));  
                UF_CALL(allocate_memory(n_bodies * sizeof(tag_t),  
                    (void **)&ext_data.xforms));  
                for (ii = 0; ii < n_bodies; ii++)  
                    get_wave_source_and_xform(bodies[ii],  
                        &ext_data.bodies[ii], &ext_data.xforms[ii]);  
            /*  The bodies changed so re-calculate the faces before re-selection 里海译:The bodies have changed, so please recalculate the faces before reselecting. */  
                UF_CALL(UF_MODL_identify_exterior_using_hl(ext_data.num_bodies,  
                    ext_data.bodies, ext_data.xforms, 6, directions, chordal_tol,  
                    UF_LINKED_HL_RES_COARSE, &ext_data.num_faces,  
                    &ext_data.faces, &ext_data.xform_index));  
            }  
            UF_free(old_bodies);  
            UF_CALL(allocate_memory(ext_data.num_faces * sizeof(tag_t),  
                (void **)&old_faces));  
            UF_CALL(allocate_memory(ext_data.num_faces * sizeof(tag_t),  
                (void **)&old_xforms));  
            for (ii = 0; ii < ext_data.num_faces; ii++)  
                old_xforms[ii] = ext_data.xforms[ext_data.xform_index[ii]];  
            map_prototypes_and_xforms_to_occurrences(&ext_data.num_faces,  
                ext_data.faces, old_xforms, old_faces);  
        /*  this prevents selection of faces which are part of or were created  
            after the linked exterior.  They are invalid for participation in it.  
        */  
            mark_id = rollback_for_edit(feat);  
            n_faces = select_and_deselect_faces(  
                "Select exterior faces to extract",  
                old_faces, ext_data.num_faces, &faces);  
            UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
            UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
            if (!do_tag_arrays_match(ext_data.num_faces, old_faces,  
                    n_faces, faces))  
            {  
                UF_free(ext_data.faces);  
                UF_free(ext_data.xform_index);  
                ext_data.num_faces = n_faces;  
                UF_CALL(allocate_memory(n_faces * sizeof(tag_t),  
                    (void **)&ext_data.faces));  
                UF_CALL(allocate_memory(n_faces * sizeof(int),  
                    (void **)&ext_data.xform_index));  
                for (ii = 0; ii < n_faces; ii++)  
                {  
                    get_wave_source_and_xform(faces[ii], &ext_data.faces[ii],  
                        &face_xform);  
                    UF_CALL(UF_MODL_ask_face_body(ext_data.faces[ii], &body));  
                    for (jj = 0; jj < ext_data.num_bodies; jj++)  
                    {  
                        if (body != ext_data.bodies[jj]) continue;  
                        if (do_assy_ctxt_xforms_match(face_xform,  
                            ext_data.xforms[jj]))  
                        {  
                            ext_data.xform_index[ii] = jj;  
                            break;  
                        }  
                    }  
                    if (jj == ext_data.num_bodies)  
                    {  
                        ext_data.num_bodies++;  
                        UF_CALL(reallocate_tag_array(ext_data.num_bodies - 1,  
                            ext_data.num_bodies,  
                            &ext_data.bodies));  
                        UF_CALL(reallocate_tag_array(ext_data.num_bodies - 1,  
                            ext_data.num_bodies,  
                            &ext_data.xforms));  
                        UF_CALL(UF_MODL_ask_face_body(faces[ii], &body));  
                        get_wave_source_and_xform(body,  
                            &ext_data.bodies[ext_data.num_bodies - 1],  
                            &ext_data.xforms[ext_data.num_bodies - 1]);  
                        ext_data.xform_index[ii] = ext_data.num_bodies - 1;  
                    }  
                }  
            }  
            UF_free(old_faces);  
            UF_free(old_xforms);  
            UF_CALL(UF_MODL_edit_linked_exterior(feat, &ext_data));  
            UF_CALL(UF_MODL_update());  
            free_linked_exterior_data(&ext_data);  
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

> 这段代码实现了NX软件中编辑链接外表面特征的功能。主要步骤包括：
>
> 1. 选择一个链接外表面特征。
> 2. 获取该特征当前的体和面信息。
> 3. 允许用户选择新的候选体和面，以替换原始特征中的体和面。
> 4. 比较用户选择的体和面与原始特征中的体和面，如果不同，则重新计算特征的外表面。
> 5. 使用新的体和面更新链接外表面特征。
> 6. 更新NX模型。
>
> 代码的关键点包括：
>
> 总的来说，这段代码实现了NX软件中编辑链接外表面特征的功能，用户可以通过选择新的体和面来更新特征，同时代码中做了错误处理和撤销操作，保证了功能的健壮性。
>
