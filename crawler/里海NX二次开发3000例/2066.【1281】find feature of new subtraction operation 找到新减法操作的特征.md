### 【1281】find feature of new subtraction operation 找到新减法操作的特征

#### 代码

```cpp
    /*HEAD FIND_FEATURE_OF_NEW_SUBTRACTION_OPERATION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能，用于在用户框架中打印系统日志。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static int ask_part_feats(tag_t part, tag_p_t *feats)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            body = NULL_TAG,  
            feat;  
        uf_list_p_t  
            feat_list,  
            part_list;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_body_feats(body, &feat_list)))  
            {  
                UF_CALL(UF_MODL_ask_list_count(feat_list, &n));  
                for (ii = 0; ii < n; ii++)  
                {  
                    UF_CALL(UF_MODL_ask_list_item(feat_list, ii, &feat));  
                    UF_CALL(UF_MODL_put_list_item(part_list, feat));  
                }  
                UF_CALL(UF_MODL_delete_list(&feat_list));  
            }  
        }  
        n = make_an_array(&part_list, feats);  
        return n;  
    }  
    static int whats_new(tag_t *old, int n_old, tag_t *now, int n_now, tag_t **new)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_now; ii++)  
        {  
            for (jj = 0; jj < n_old; jj++)  
                if (old[jj] == now[ii]) break;  
            if (jj == n_old) UF_CALL(UF_MODL_put_list_item(list, now[ii]));  
        }  
        return (make_an_array(&list, new));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *feature;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &feature)))  
        {  
            WRITE_S(feature);  
            UF_free(feature);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_after,  
            n_before,  
            n_new;  
        tag_t  
            *after,  
            *before,  
            *new,  
            part = UF_PART_ask_display_part(),  
            target,  
            tool;  
        while (((target = select_a_solid("Select target solid")) != NULL_TAG) &&  
               ((tool = select_a_solid("Select tool solid")) != NULL_TAG))  
        {  
            n_before = ask_part_feats(part, &before);  
            if (!UF_CALL(UF_MODL_operations(target, tool, UF_NEGATIVE)))  
            {  
                n_after = ask_part_feats(part, &after);  
                n_new = whats_new(before, n_before, after, n_after, &new);  
                for (ii = 0; ii < n_new; ii++)  
                    report_feature_name(new[ii]);  
                UF_free(after);  
                UF_free(new);  
            }  
            UF_free(before);  
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

> 这段代码是NX的二次开发代码，主要功能是检测并报告NX部件中新增的减法特征。
>
> 代码的主要步骤如下：
>
> 1. 初始化和错误处理：首先定义了UF_CALL宏，用于调用NX的UF函数，并报告错误信息。
> 2. 选择体：实现了select_a_solid函数，用于选择一个实体。
> 3. 遍历体和特征：实现了ask_next_body函数，用于遍历部件中的所有实体，并ask_part_feats函数，用于获取实体的所有特征。
> 4. 比较新旧特征：实现了whats_new函数，用于比较新旧特征列表，找出新增的特征。
> 5. 报告新增特征：实现了report_feature_name函数，用于报告新增特征的名字。
> 6. 执行主体逻辑：在do_it函数中，循环让用户选择目标体和工具体，然后执行减法操作，并报告新增特征。
> 7. ufusr函数：这是NX二次开发程序的入口函数，在这里调用do_it函数执行主体逻辑。
> 8. 卸载函数：定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体来说，这段代码实现了检测并报告NX部件中新增减法特征的功能，展示了NX二次开发中实体遍历、特征查询、内存管理等常见技巧。
>
