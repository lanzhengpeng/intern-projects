### 【2277】report parts features expressions 报告零件特征表达式

#### 代码

```cpp
    /*HEAD REPORT_PARTS_FEATURES_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
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
    static int ask_all_features(tag_t part, tag_p_t *feats)  
    {  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
        return (make_an_array(&feat_list, feats));  
    }  
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void report_feature_expressions(tag_t feat)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            master;  
        char  
            **descs,  
            msg[MAX_LINE_SIZE+1],  
            *string,  
            *type;  
        if(!UF_CALL(UF_MODL_ask_exp_desc_of_feat(feat, &n_exps, &descs, &exps)))  
        {  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                if (!UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string)))  
                {  
                    sprintf(msg, "  %d.  %s (%s)\n", ii+1, string, descs[ii]);  
                    UF_free(string);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_free(exps);  
            UF_free_string_array(n_exps, descs);  
        }  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if (!strcmp(type, "INSTANCE"))  
        {  
            UF_CALL(UF_MODL_ask_master(feat, &master));  
            report_feature_name_and_type("  from master of instance", master);  
            report_feature_expressions(master);  
        }  
        UF_free(type);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_features;  
        tag_t  
            *features,  
            part = UF_PART_ask_display_part();  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        n_features = ask_all_features(part, &features);  
        WRITE_D(n_features);  
        for (ii = 0; ii < n_features; ii++)  
        {  
            sprintf(name, "features[%d]", ii);  
            report_feature_name_and_type(name, features[ii]);  
            report_feature_expressions(features[ii]);  
        }  
        if (n_features > 0) UF_free(features);  
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

> 这段代码是NX的二次开发代码，其主要功能是遍历当前部件的所有特征，并报告每个特征的名字、类型以及相关的表达式。具体来说，该代码的主要内容和功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时输出错误信息。
> 2. 定义了一个函数write_integer_to_listing_window，用于在列表窗口中写入整数值。
> 3. 定义了一个函数ask_next_feature，用于遍历部件的所有特征。
> 4. 定义了一个函数allocate_memory，用于申请内存。
> 5. 定义了一个函数make_an_array，用于将UF列表转换为tag数组。
> 6. 定义了一个函数ask_all_features，用于获取部件的所有特征。
> 7. 定义了一个函数report_feature_name_and_type，用于报告特征的名字和类型。
> 8. 定义了一个函数report_feature_expressions，用于报告特征的相关表达式。
> 9. 定义了一个函数do_it，用于执行遍历部件特征并报告信息的操作。
> 10. 定义了ufusr函数，作为NX的二次开发入口函数，初始化UF模块，调用do_it函数，然后终止UF模块。
> 11. 定义了ufusr_ask_unload函数，表示立即卸载该UF插件。
>
> 总的来说，这段代码实现了对NX部件特征信息的遍历和报告功能，对了解NX部件特征结构非常有帮助。
>
