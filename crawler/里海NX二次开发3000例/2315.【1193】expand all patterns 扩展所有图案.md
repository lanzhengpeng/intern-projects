### 【1193】expand all patterns 扩展所有图案

#### 代码

```cpp
    /*HEAD EXPAND_ALL_PATTERNS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_patt.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_cfi.h>  
    #include <uf_layer.h>  
    #include <uf_group.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_pattern(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_pattern_type, &object));  
        return (object);  
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
    static int ask_all_patterns(tag_t part, tag_t **patterns)  
    {  
        tag_t  
            pattern = NULL_TAG;  
        uf_list_p_t  
            patt_list;  
        UF_CALL(UF_MODL_create_list(&patt_list));  
        while ((pattern = ask_next_pattern(part, pattern)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(patt_list, pattern));  
        return (make_an_array(&patt_list, patterns));  
    }  
    static logical is_file_there(char *file_spec)  
    {  
        int missing = TRUE;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        if (missing)  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static void expand_pattern(tag_t pattern)  
    {  
        int  
            ii,  
            layer,  
            n_members,  
            old_layer,  
            status,  
            view_dep;  
        tag_t  
            group,  
            *members,  
            view_tag;  
        double  
            data[13];  
        char  
            ffn[UF_CFI_MAX_PATH_NAME_SIZE],  
            name[MAX_ENTITY_NAME_SIZE+1],  
            *pdir,  
            sfn[UF_CFI_MAX_PATH_NAME_SIZE],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_import_part_modes_t  
            modes = { 0, 1, 0, 0, 0, FALSE, FALSE };  
        uc5822(pattern, name, sfn, &view_dep, &layer, &status, data);  
    /*  Try to find the required pattern part 里海译:尝试找到所需的模式部分。 */  
        strcpy(ffn, sfn);  
        if (!is_file_there(ffn))  
        {  
            UF_CALL(UF_translate_variable("UGII_PATDIR", &pdir));  
            UF_CALL(uc4575(pdir, 2, sfn, ffn));  
        }  
        if (!is_file_there(ffn))  
        {  
            UF_CALL(UF_translate_variable("UGII_UTIL", &pdir));  
            UF_CALL(uc4575(pdir, 2, sfn, ffn));  
        }  
        if (!is_file_there(ffn)) return;  
    /*  Set the work layer 里海译:Set the work layer */  
        UF_CALL(UF_LAYER_ask_work_layer(&old_layer));  
        UF_CALL(UF_LAYER_set_status(layer, UF_LAYER_WORK_LAYER));  
    /*  Merge in the pattern part 里海译:Merge in the pattern part. */  
        UF_CALL(UF_PART_import(ffn, &modes, data, &data[9], data[12], &group));  
    /*  Name the group with the same name the pattern originally had 里海译:请用与原来图案相同名字的组命名 */  
        UF_CALL(UF_OBJ_set_name(group, name));  
    /*  Convert all geometry to view dependent 里海译:将所有几何图形转换为视点依赖的。 */  
        if (view_dep)  
        {  
            uc6409(pattern, &view_dep, view_name);  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
            UF_CALL(UF_GROUP_ask_group_data(group, &members, &n_members));  
            for (ii = 0; ii < n_members; ii++)  
                UF_CALL(UF_VIEW_convert_to_view(view_tag, members[ii]));  
            if (n_members > 0) UF_free(members);  
        }  
    /*  Reset the work layer 里海译:重置工作层 */  
        UF_CALL(UF_LAYER_set_status(old_layer, UF_LAYER_WORK_LAYER));  
    /*  Delete the original pattern object 里海译:删除原始模式对象 */  
        UF_CALL(UF_OBJ_delete_object(pattern));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_patterns;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *patterns,  
            work_view;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        UF_CALL(UF_VIEW_ask_type(work_view, &type, &subtype));  
        if (type == UF_VIEW_MODEL_TYPE)  
        {  
            n_patterns = ask_all_patterns(part, &patterns);  
            for (ii = 0; ii < n_patterns; ii++) expand_pattern(patterns[ii]);  
            if (n_patterns > 0) UF_free(patterns);  
        }  
        else uc1601("Do not run in drawing or member view", TRUE);  
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

> 这段代码是一个NX二次开发程序，其主要功能是展开模型中的所有图案。
>
> 1. 错误处理函数：代码定义了report_error函数，用于输出错误信息，包括文件名、行号、调用函数名和错误码。
> 2. 遍历图案对象：ask_next_pattern函数用于遍历指定部件中的图案对象。
> 3. 内存分配和对象列表处理：allocate_memory和make_an_array函数用于分配内存和处理对象列表。
> 4. 获取所有图案对象：ask_all_patterns函数用于获取部件中的所有图案对象，并返回一个对象数组。
> 5. 检查文件是否存在：is_file_there函数用于检查指定文件是否存在。
> 6. 展开图案：expand_pattern函数用于展开一个图案，包括设置工作层、导入图案文件、转换视图依赖等。
> 7. 执行展开操作：do_it函数遍历所有图案对象并调用expand_pattern函数展开。
> 8. 主函数：ufusr函数是程序的入口，初始化后调用do_it函数，然后终止。
> 9. 卸载请求：ufusr_ask_unload函数返回立即卸载标志。
>
> 总的来说，这段代码实现了在NX中展开模型中所有图案的功能。
>
