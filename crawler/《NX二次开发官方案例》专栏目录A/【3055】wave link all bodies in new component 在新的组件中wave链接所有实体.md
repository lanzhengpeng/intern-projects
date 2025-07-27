### 【3055】wave link all bodies in new component 在新的组件中wave链接所有实体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_csys.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
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
    static void create_wave_linked_body(tag_t body)  
    {  
        tag_t  
            feat,  
            part_occ,  
            proto,  
            xform;  
        if (UF_ASSEM_is_occurrence(body))  
            proto = UF_ASSEM_ask_prototype_of_occ(body);  
        else  
            proto = body;  
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
        UF_CALL(UF_MODL_update());  
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
    static void recursively_dig_for_bodies(tag_t component, uf_list_p_t body_list)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            obj = NULL_TAG;  
        do {  
            obj = UF_ASSEM_cycle_ents_in_part_occ(component, obj);  
            if (obj == NULL_TAG) break;  
            UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                UF_CALL(UF_MODL_put_list_item(body_list, obj));  
            if ((type == UF_component_type) && (subtype == UF_part_occurrence_subtype))  
                recursively_dig_for_bodies(obj, body_list);  
        } while (true);  
    }  
    static int ask_all_bodies_in_component(tag_t component, tag_t **solids)  
    {  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        recursively_dig_for_bodies(component, body_list);  
        return (make_an_array(&body_list, solids));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_bodies;  
        tag_t  
            *bodies,  
            disp_part = UF_PART_ask_display_part(),  
            instance,  
            part_occ,  
            work_part = UF_ASSEM_ask_work_part();  
        char  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        double  
            dest_csys[9],  
            dest_pnt[3];  
        UF_PART_load_status_t  
            error_status;  
        ask_wcs_info(dest_pnt, dest_csys);  
        if (work_part != disp_part)  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, dest_pnt,  
            UF_CSYS_WORK_COORDS, dest_pnt));  
        if (prompt_for_existing_part_name("Add Component", fspec))  
        {  
            UF_CALL(UF_ASSEM_add_part_to_assembly(disp_part, fspec, NULL,  
                NULL, dest_pnt, dest_csys, 0, &instance, &error_status));  
            report_load_status(&error_status);  
            if (error_status.failed) return;  
            part_occ = UF_ASSEM_ask_part_occ_of_inst(UF_ASSEM_ask_root_part_occ(disp_part),  
                instance);  
            n_bodies = ask_all_bodies_in_component(part_occ, &bodies);  
            for (ii = 0; ii < n_bodies; ii++) create_wave_linked_body(bodies[ii]);  
            if (n_bodies > 0) UF_free(bodies);  
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

> 这段代码是用于NX Open C++ API的二次开发示例代码，主要功能是添加一个组件到NX装配中，并创建与组件中的体相关的波纹特征。
>
> 主要功能包括：
>
> 1. 查询并打印当前坐标系信息，用于后续定位组件。
> 2. 提示用户输入要添加的组件文件路径，并调用UF_ASSEM_add_part_to_assembly函数将组件添加到装配中。
> 3. 获取添加的组件实例，并递归遍历组件中的所有体，将它们存放在数组中。
> 4. 遍历数组中的每个体，调用UF_WAVE_create_linked_body函数为每个体创建波纹特征。
> 5. 释放内存，结束二次开发。
>
> 代码中还包含错误处理、日志输出等辅助功能。
>
> 该代码示例展示了如何使用NX Open C++ API进行基本的装配和特征操作，对于NX二次开发入门具有参考价值。
>
