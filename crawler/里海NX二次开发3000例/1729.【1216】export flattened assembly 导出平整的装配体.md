### 【1216】export flattened assembly 导出平整的装配体

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的功能。 */  
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

> 根据代码内容，这是一段用于NX的二次开发代码，主要用于导出装配体的所有几何体到一个文件中。代码的主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败的信息。
> 2. 定义了一个组合目录和通配符的函数combine_directory_and_wildcard。
> 3. 定义了一个提示用户输入部件名称的函数prompt_for_part_name。
> 4. 定义了一个递归加载装配体树中所有部件的函数fully_load_assembly_tree。
> 5. 定义了一个申请内存的函数allocate_memory。
> 6. 定义了一个从列表中提取所有对象到一个数组的函数make_an_array。
> 7. 定义了一个获取所有几何体的函数ask_all_geometry。
> 8. 定义了一个主函数do_it，用于提示用户输入文件名，加载所有部件，获取所有几何体，并调用UF_PART_export_with_options函数导出到指定文件。
> 9. 定义了ufusr函数，用于初始化并调用主函数，然后终止。
> 10. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，这段代码实现了NX装配体中所有几何体的导出功能，并且包含了错误处理、内存管理等辅助函数。
>
