### 【0759】create linked exterior 创建关联外部

#### 代码

```cpp
    /*HEAD CREATE_LINKED_EXTERIOR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
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
    static int ask_all_bodies(tag_t part, tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_groups,  
            n_subfeats,  
            yellow;  
        tag_t  
            *groups,  
            ext_feat,  
            part = UF_PART_ask_display_part(),  
            sheet,  
            *subfeats,  
            wp = UF_ASSEM_ask_work_part(),  
            wp_obj = UF_OBJ_cycle_all(wp, NULL_TAG);  
        double  
            chordal_tol,  
            directions[6][3] =  
                { {1,0,0}, {0,1,0}, {0,0,1}, {-1,0,0}, {0,-1,0}, {0,0,-1}},  
            mass_props[47];  
        char  
            sht_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_MODL_linked_ext_t  
            ext_data = {0};  
        if ((ext_data.num_bodies = ask_all_bodies(part, &ext_data.bodies)) == 0)  
            return;  
        set_undo_mark("Create linked exterior");  
        UF_MODL_ask_distance_tolerance(&chordal_tol);  
        UF_CALL(allocate_memory(ext_data.num_bodies * sizeof(tag_t),  
            (void **)&ext_data.xforms));  
        for (ii = 0; ii < ext_data.num_bodies; ii++)  
        {  
            get_wave_source_and_xform(ext_data.bodies[ii], &ext_data.bodies[ii],  
                &ext_data.xforms[ii]);  
        }  
        UF_CALL(UF_MODL_identify_exterior_using_hl(ext_data.num_bodies,  
            ext_data.bodies, ext_data.xforms, 6, directions, chordal_tol,  
            UF_LINKED_HL_RES_COARSE, &ext_data.num_faces,  
            &ext_data.faces, &ext_data.xform_index));  
        ext_data.group_results = UF_LINKED_EXT_GROUP_SINGLE;  
        ext_data.mass_props = TRUE;  
        ext_data.delete_openings = TRUE;  
        ext_data.at_timestamp = FALSE;  
        UF_CALL(UF_MODL_create_linked_exterior(&ext_data, &ext_feat));  
        free_linked_exterior_data(&ext_data);  
        UF_CALL(UF_MODL_ask_linked_exterior(ext_feat, &ext_data, &n_groups,  
            &groups, &n_subfeats, &subfeats, mass_props));  
        free_linked_exterior_data(&ext_data);  
        UF_CALL(UF_DISP_ask_closest_color_in_part(wp_obj, UF_DISP_YELLOW_NAME,  
            &yellow));  
        for (ii = 0; ii < n_subfeats; ii++)  
        {  
            UF_CALL(UF_MODL_ask_feat_body(subfeats[ii], &sheet));  
            UF_CALL(UF_OBJ_set_color(sheet, yellow));  
            sprintf(sht_name, "LE_SHT_%d", ii);  
            UF_CALL(UF_OBJ_set_name(sheet, sht_name));  
        }  
        if (n_subfeats > 0) UF_free(subfeats);  
        if (n_groups > 0) UF_free(groups);  
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

> 这段代码是一个NX Open C++ API二次开发示例，用于创建一个零件的联动外表面特征。
>
> 主要步骤包括：
>
> 1. 遍历零件，获取所有实体体，存储在数组中。
> 2. 对每个实体体，获取其在装配中的源和变换矩阵。
> 3. 使用高亮线识别方法，识别出所有实体的外表面，并存储面和变换矩阵索引。
> 4. 设置联动外表面的参数，如分组结果、是否计算质量属性、是否删除开口等。
> 5. 调用UF_MODL_create_linked_exterior接口，根据参数创建联动外表面对象。
> 6. 获取联动外表面包含的子特征和组信息。
> 7. 将所有联动外表面的子特征设置成黄色，并重命名。
> 8. 释放所有分配的内存。
> 9. 调用UF_initialize和UF_terminate来初始化和终止NX。
> 10. 提供ufusr和ufusr_ask_unload函数作为二次开发程序的入口。
>
> 这个示例展示了如何使用NX Open C++ API来开发联动外表面的创建功能，包括实体遍历、面识别、联动外表面创建以及后续处理等。
>
