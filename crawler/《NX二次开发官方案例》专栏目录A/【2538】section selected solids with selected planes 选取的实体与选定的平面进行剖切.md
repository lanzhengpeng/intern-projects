### 【2538】section selected solids with selected planes 选取的实体与选定的平面进行剖切

#### 代码

```cpp
    /*HEAD SECTION_SELECTED_SOLIDS_WITH_SELECTED_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_undo.h>  
    #include <uf_assem.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能，用于打印系统日志。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_solids(char *prompt, tag_t **solids)  
    {  
        int  
            ii,  
            n,  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select solids", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_body_type, &solid, &resp, &n,  
            solids));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*solids)[ii], FALSE));  
        return n;  
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
    static tag_t create_dumb_body_copy(tag_t body)  
    {  
        tag_t  
            feat,  
            part_occ,  
            proto,  
            xform,  
            dumb_body;  
        uf_list_p_t  
            list;  
        proto = UF_ASSEM_ask_prototype_of_occ(body);  
        if (UF_OBJ_is_object_a_promotion(proto))  
            part_occ = ask_part_occ_of_prom_occ(body);  
        else  
            part_occ = UF_ASSEM_ask_part_occurrence(body);  
        if (part_occ == NULL_TAG)  
            UF_CALL(UF_WAVE_create_linked_body(proto,NULL_TAG,body,FALSE,&feat));  
        else  
        {  
            UF_CALL(UF_SO_create_xform_assy_ctxt(body, part_occ, NULL_TAG, &xform));  
            UF_CALL(UF_WAVE_create_linked_body(proto, xform, body, FALSE, &feat));  
        }  
        UF_CALL(UF_MODL_ask_feat_body(feat, &dumb_body));  
        if (dumb_body != NULL_TAG)  
        {  
            list = make_a_list(1, &dumb_body);  
            UF_CALL(UF_MODL_delete_object_parms(list));  
            UF_CALL(UF_MODL_delete_list(&list));  
        }  
        return dumb_body;  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            *solids,  
            result;  
        UF_CURVE_section_general_data_t  
            general = { NULL, 0, 0, 0, 0, 0 };  
        UF_CURVE_section_planes_data_t  
            planes;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_MODL_ask_distance_tolerance(&general.tolerance);  
        while (((general.num_objects = select_solids("Select solids to section",  
                    &solids)) > 0) &&  
                ((planes.num_planes = select_planes("Select section planes",  
                    &planes.planes)) > 0))  
        {  
            UF_CALL(allocate_memory(general.num_objects*sizeof(tag_t),  
                (void **)&general.objects));  
            for (ii = 0; ii < general.num_objects; ii++)  
                if (UF_ASSEM_is_occurrence(solids[ii]))  
                    general.objects[ii] = create_dumb_body_copy(solids[ii]);  
                else  
                    general.objects[ii] = solids[ii];  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible,"Section From Planes",&mark));  
            if (UF_CALL(UF_CURVE_section_from_planes(&general, &planes, &result)))  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
            for (ii = 0; ii < general.num_objects; ii++)  
                if (UF_ASSEM_is_occurrence(solids[ii]))  
                    UF_CALL(UF_OBJ_delete_object(general.objects[ii]));  
            UF_free(general.objects);  
            UF_free(solids);  
            UF_free(planes.planes);  
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

> 这段代码是NX的二次开发代码，其主要功能是实现选择实体和剖面平面的功能，并将实体沿着剖面平面进行切片。具体来说，代码的主要功能包括：
>
> 1. 选择实体：通过自定义选择过滤器，允许用户选择实体，并将其存储在数组中。
> 2. 选择剖面平面：使用自定义选择过滤器，允许用户选择剖面平面，并将其存储在数组中。
> 3. 实体切片：遍历用户选择的实体，如果实体是组合体中的实例，则先复制出一个哑实体，然后调用NX提供的函数，沿着用户选择的剖面平面对该实体进行切片。
> 4. 撤销操作：在切片前后设置撤销标记点，方便用户撤销切片操作。
> 5. 内存管理：动态分配内存，并在使用后释放，避免内存泄漏。
> 6. 错误处理：在函数调用后检查错误码，并输出错误信息。
> 7. 初始化和终止：在进入和退出函数时，分别调用NX的初始化和终止函数。
> 8. 卸载函数：定义了一个询问卸载函数，返回立即卸载的标志。
>
> 综上所述，这段代码通过二次开发扩展了NX的建模功能，实现了选择实体、剖面平面和切片的完整流程，并进行了错误处理和内存管理，是一个典型的NX二次开发示例。
>
