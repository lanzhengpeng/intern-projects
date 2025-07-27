### 【0391】assign attribute to all solids and save assembly plaintext为所有实体分配属性并保存装配

#### 代码

```cpp
    /*HEAD ASSIGN_ATTRIBUTE_TO_ALL_SOLIDS_AND_SAVE_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_attr.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中的新增功能，因此我仅提供翻译：

UF_print_syslog是V18版本中的新增功能。 */  
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
    static tag_t ask_next_solid_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
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
    static int ask_all_solids(tag_t part, tag_t **solids)  
    {  
        tag_t  
            solid = NULL_TAG;  
        uf_list_p_t  
            solid_list;  
        UF_CALL(UF_MODL_create_list(&solid_list));  
        while ((solid = ask_next_solid_body(part, solid)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(solid_list, solid));  
        return (make_an_array(&solid_list, solids));  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_fully_loaded(tag_t item)  
    {  
        tag_t  
            part;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            error_status;  
        if (UF_ASSEM_is_occurrence(item))  
            item = UF_ASSEM_ask_prototype_of_occ(item);  
        UF_CALL(UF_OBJ_ask_owning_part(item, &part));  
        UF_PART_ask_part_name(part, partname);  
        if ((UF_PART_is_loaded(partname)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            *errs,  
            ii,  
            n_fail,  
            n_solids;  
        tag_t  
            *fails,  
            part = UF_PART_ask_display_part(),  
            *solids;  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
        value.value.string = "X1";  
        if ((n_solids = ask_all_solids(part, &solids)) == 0) return;  
        for (ii = 0; ii < n_solids; ii++)  
        {  
            if (UF_ASSEM_is_occurrence(solids[ii]))  
                solids[ii] = UF_ASSEM_ask_prototype_of_occ(solids[ii]);  
            ensure_fully_loaded(solids[ii]);  
            UF_CALL(UF_ATTR_assign(solids[ii], "TEST", value));  
        }  
        UF_free(solids);  
        UF_CALL(UF_PART_save_all( &n_fail, &fails, &errs));  
        if (n_fail > 0)  
        {  
            UF_free(errs);  
            UF_free(fails);  
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

> 这段代码是一个NX二次开发代码，主要用于给装配体中的所有实体分配一个属性，并保存装配体。具体介绍如下：
>
> 1. 代码结构：引入所需的头文件。定义UF_CALL宏，用于调用NX API函数，并打印错误信息。定义报告错误函数report_error，用于打印错误信息。定义ask_next_solid_body函数，用于遍历装配体中的实体。定义allocate_memory和make_an_array函数，用于分配内存和创建数组。定义ask_all_solids函数，用于获取装配体中的所有实体。定义report_load_status函数，用于报告加载状态。定义ensure_fully_loaded函数，用于确保装配体完全加载。定义do_it函数，用于执行给实体分配属性和保存装配体的操作。定义ufusr函数，作为NX二次开发的入口函数，调用do_it函数。定义ufusr_ask_unload函数，返回卸载模式。
> 2. 引入所需的头文件。
> 3. 定义UF_CALL宏，用于调用NX API函数，并打印错误信息。
> 4. 定义报告错误函数report_error，用于打印错误信息。
> 5. 定义ask_next_solid_body函数，用于遍历装配体中的实体。
> 6. 定义allocate_memory和make_an_array函数，用于分配内存和创建数组。
> 7. 定义ask_all_solids函数，用于获取装配体中的所有实体。
> 8. 定义report_load_status函数，用于报告加载状态。
> 9. 定义ensure_fully_loaded函数，用于确保装配体完全加载。
> 10. 定义do_it函数，用于执行给实体分配属性和保存装配体的操作。
> 11. 定义ufusr函数，作为NX二次开发的入口函数，调用do_it函数。
> 12. 定义ufusr_ask_unload函数，返回卸载模式。
> 13. 主要功能：遍历装配体中的所有实体。给每个实体分配一个字符串属性"TEST"。保存装配体。
> 14. 遍历装配体中的所有实体。
> 15. 给每个实体分配一个字符串属性"TEST"。
> 16. 保存装配体。
> 17. 关键点：使用UF_CALL宏调用NX API，可打印出错误信息。使用对象循环、列表、数组等数据结构来处理实体。确保装配体完全加载后再操作。分配属性后保存装配体。
> 18. 使用UF_CALL宏调用NX API，可打印出错误信息。
> 19. 使用对象循环、列表、数组等数据结构来处理实体。
> 20. 确保装配体完全加载后再操作。
> 21. 分配属性后保存装配体。
> 22. 注意事项：确保NX版本支持代码中的API。在NX环境中编译和运行代码。根据实际需求修改属性名称等参数。
> 23. 确保NX版本支持代码中的API。
> 24. 在NX环境中编译和运行代码。
> 25. 根据实际需求修改属性名称等参数。
>
> 综上所述，这段代码实现了在NX中遍历装配体中的所有实体，给每个实体分配一个属性，并保存装配体的功能。
>
