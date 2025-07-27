### 【1215】export flattened assembly “导出展平装配体”

#### 代码

```cpp
    /*HEAD EXPORT_FLATTENED_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。在回答问题时，只需简洁地回复“UF_print_syslog是V18版本中新增的函数”，无需添加任何其他无关内容。 */  
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
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void fully_load_assembly_tree(tag_t component)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components,  
            old = UF_ASSEM_ask_work_part();  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        n_children = UF_ASSEM_ask_part_occ_children(component, &child_components);  
        if (n_children > 0)  
        {  
            for (ii = 0; ii < n_children; ii++)  
            {  
                UF_ASSEM_ask_part_name_of_child(  
                    UF_ASSEM_ask_inst_of_part_occ(child_components[ii]), partname);  
                if ((UF_PART_is_loaded(partname)) != 1)  
                {  
                    UF_DISP_set_display(FALSE);  
                    UF_CALL(UF_ASSEM_set_work_occurrence(child_components[ii]));  
                    UF_CALL(UF_ASSEM_set_work_part(old));  
                    UF_DISP_set_display(TRUE);  
                }  
                fully_load_assembly_tree(child_components[ii]);  
            }  
            UF_free(child_components);  
        }  
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
    static int ask_all_geometry(tag_t **geometry)  
    {  
        logical  
            ok;  
        int  
            subtype,  
            type;  
        tag_t  
            item = NULL_TAG;  
        uf_list_p_t  
            geo_list;  
        UF_CALL(UF_MODL_create_list(&geo_list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &item)) && (item != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(item, &type, &subtype));  
            // UF_CALL(UF_OBJ_is_type_transferable(type, &ok));  
            // if (ok) UF_CALL(UF_OBJ_is_transferable(item, &ok));  
            UF_CALL(UF_OBJ_is_transferable(item, &ok));  
            if (ok && (type != UF_datum_axis_type) &&  
                (type != UF_datum_plane_type) && (type != UF_sketch_type))  
                    UF_CALL(UF_MODL_put_list_item(geo_list, item));  
            // if (ok) UF_CALL(UF_MODL_put_list_item(geo_list, item));  
        }  
        return make_an_array(&geo_list, geometry);  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *geometry,  
            part = UF_PART_ask_display_part(),  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            opts = { TRUE, UF_PART_remove_params, UF_PART_copy_exp_deeply };  
        if (prompt_for_part_name("Export Flat Assembly", fspec))  
        {  
            if (root_part_occ != NULL_TAG) fully_load_assembly_tree(root_part_occ);  
            if ((n = ask_all_geometry(&geometry)) > 0)  
            {  
                UF_CALL(UF_PART_export_with_options(fspec, n, geometry, &opts));  
                UF_free(geometry);  
            }  
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

> 这段代码是一个用于NX CAD系统的二次开发程序，主要实现了导出当前打开的装配体为平坦装配体的功能。
>
> 主要功能包括：
>
> 1. 弹出对话框，提示用户输入要导出的平坦装配体的文件名。
> 2. 完全加载当前打开的装配体的装配树，确保所有子组件都被加载。
> 3. 遍历所有图层的几何体，创建一个几何体列表。
> 4. 调用NX提供的函数，根据用户指定的文件名和几何体列表，导出当前的装配体为平坦装配体。
> 5. 在导出过程中，可以选择是否移除参数，是否进行深度复制等选项。
> 6. 提供错误处理机制，如果函数调用失败，会弹出错误信息。
> 7. 在程序开始和结束时，调用NX的初始化和终止函数。
> 8. 如果用户要求卸载该程序，可以选择立即卸载。
>
> 总体来说，该程序实现了NX CAD系统的一个常见需求，即导出装配体为平坦装配体，并提供了友好的用户交互和错误处理机制。
>
