### 【1202】export all objects on work layer 导出工作层上的所有对象

#### 代码

```cpp
    /*HEAD EXPORT_ALL_OBJECTS_ON_WORK_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_layer.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的，不要添加任何废话，只提供翻译结果。 */  
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
    static int ask_all_objects_on_work_layer(tag_t **objects)  
    {  
        int  
            work;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        UF_CALL(UF_LAYER_ask_work_layer(&work));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(work, &object)) &&  
            (object != NULL_TAG)) UF_CALL(UF_MODL_put_list_item(list, object));  
        return make_an_array(&list, objects);  
    }  
    static void build_unique_temp_part_name(char *fspec)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            n_objects;  
        tag_t  
            *objects;  
        char  
            msg[133],  
            tempname[UF_CFI_MAX_PATH_NAME_SIZE];  
        build_unique_temp_part_name(tempname);  
        n_objects = ask_all_objects_on_work_layer(&objects);  
        if (n_objects > 0)  
        {  
            UF_CALL(UF_PART_export(tempname, n_objects, objects));  
            UF_free(objects);  
            sprintf(msg, "Exported %d objects to %s", n_objects, tempname);  
            uc1601(msg, TRUE);  
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

> 这段代码是一个NX的二次开发示例，主要功能是导出工作层上的所有对象。
>
> 代码的主要流程包括：
>
> 1. 定义了错误报告函数report_error，用于输出函数调用失败的错误信息。
> 2. 定义了内存分配函数allocate_memory，用于申请指定大小的内存。
> 3. 定义了创建对象数组函数make_an_array，用于将对象列表转换为对象数组，并释放列表。
> 4. 定义了查询工作层对象函数ask_all_objects_on_work_layer，用于查询工作层上的所有对象，并返回对象数组。
> 5. 定义了生成临时部件名函数build_unique_temp_part_name，用于生成一个唯一的临时部件文件名。
> 6. 定义了主函数do_it，用于查询工作层上的所有对象，并将这些对象导出到一个临时部件文件中。
> 7. 定义了ufusr函数，是NX二次开发的入口函数，在这里调用do_it函数执行主要流程。
> 8. 定义了ufusr_ask_unload函数，用于设置立即卸载二次开发库。
>
> 总的来说，这段代码通过查询工作层上的所有对象，并将这些对象导出到一个临时部件文件中，实现了NX部件的导出功能。
>
