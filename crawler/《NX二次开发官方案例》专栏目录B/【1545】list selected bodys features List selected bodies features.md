### 【1545】list selected bodys features List selected bodies features

#### 代码

```cpp
    /*HEAD LIST_SELECTED_BODYS_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

UF_print_syslog是在V18版本中新增的函数。 */  
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
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void report_body_feats(tag_t body)  
    {  
        tag_t  
            base_body;  
        uf_list_p_t  
            feat_list,  
            temp;  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        UF_CALL(UF_UI_open_listing_window());  
        if (UF_ASSEM_is_occurrence(body))  
            body = UF_ASSEM_ask_prototype_of_occ(body);  
        ensure_fully_loaded(body);  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        for (temp = feat_list; temp; temp = temp->next)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(temp->eid, &type));  
            if (!strcmp(type, "PROMOTION"))  
            {  
                UF_CALL(UF_MODL_prom_map_object_down(body, &base_body));  
                report_body_feats(base_body);  
            }  
            else  
            {  
                UF_CALL(UF_MODL_ask_feat_name(temp->eid, &name));  
                sprintf(msg, "tag %d = %s (%s)\n", temp->eid, name, type);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_free(name);  
            }  
            UF_free(type);  
        }  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body;  
        while ((body = select_a_solid("List features")) != NULL_TAG)  
            report_body_feats(body);  
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

> 这段NX Open C++代码实现了以下功能：
>
> 1. 错误报告函数：定义了一个名为report_error的函数，用于报告函数调用失败的错误信息，包括文件名、行号、错误码和错误消息。
> 2. 选择过滤函数：定义了一个名为filter_body_type的函数，用于过滤选择对话框中不符合指定类型的体。
> 3. 选择器配置函数：定义了一个名为mask_body_type的函数，用于配置选择器的选择掩码和过滤函数。
> 4. 选择一个实体函数：定义了一个名为select_a_solid的函数，用于弹出一个选择对话框，让用户选择一个实体，并返回选中的实体标签。
> 5. 报告加载状态函数：定义了一个名为report_load_status的函数，用于报告打开零件时出现的错误。
> 6. 确保完全加载函数：定义了一个名为ensure_fully_loaded的函数，用于确保指定的零件被完全加载。
> 7. 报告体特征函数：定义了一个名为report_body_feats的函数，用于报告指定体的所有特征。
> 8. 执行函数：定义了一个名为do_it的函数，用于循环调用select_a_solid和report_body_feats，实现选择一个实体并报告其特征的功能。
> 9. UF初始化函数：定义了一个名为ufusr的函数，用于初始化UF，调用do_it函数，然后终止UF。
> 10. 卸载询问函数：定义了一个名为ufusr_ask_unload的函数，用于询问是否立即卸载该库。
>
> 总体来说，这段代码实现了在NX中选择一个实体，并报告该实体所有特征的功能。
>
