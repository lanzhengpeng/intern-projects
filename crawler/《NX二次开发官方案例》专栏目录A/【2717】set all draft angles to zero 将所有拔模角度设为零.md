### 【2717】set all draft angles to zero 将所有拔模角度设为零

#### 代码

```cpp
    /*HEAD SET_ALL_DRAFT_ANGLES_TO_ZERO CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_exps,  
            n_features;  
        tag_t  
            *features,  
            *exps,  
            part = UF_PART_ask_display_part();  
        char  
            **descs,  
            *lhs,  
            *rhs,  
            *string,  
            zero[UF_MAX_EXP_LENGTH+1];  
        uf_list_p_t  
            exp_list;  
        UF_CALL(UF_MODL_create_list(&exp_list));  
        n_features = ask_all_features(part, &features);  
        for (ii = 0; ii < n_features; ii++)  
        {  
            if(!UF_CALL(UF_MODL_ask_exp_desc_of_feat(features[ii], &n_exps, &descs, &exps)) &&  
                (n_exps > 0))  
            {  
                for (jj = 0; jj < n_exps; jj++)  
                {  
                    if (!strcmp(descs[jj], "Draft Angle"))  
                        UF_CALL(UF_MODL_put_list_item(exp_list, exps[jj]));  
                }  
                UF_free(exps);  
                UF_free_string_array(n_exps, descs);  
            }  
        }  
        if (n_features > 0) UF_free(features);  
        n_exps = make_an_array(&exp_list, &exps);  
        for (ii = n_exps; ii > 0; ii--)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii-1], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exps[ii-1]));  
            sprintf(zero, "%s=0", lhs);  
            UF_CALL(UF_MODL_edit_exp(zero));  
            UF_CALL(UF_MODL_update());  
            UF_free(string);  
            UF_free(lhs);  
            UF_free(rhs);  
        }  
        if (n_exps > 0) UF_free(exps);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发用户函数，其主要功能是遍历当前显示部件的所有特征，并找出其中的拔模角特征，将它们的表达式值设置为0，从而实现将所有拔模角设置为零的功能。
>
> 主要步骤包括：
>
> 1. 初始化UF库
> 2. 遍历当前显示部件的所有特征，并找出拔模角特征
> 3. 获取拔模角特征的表达式，并将表达式值改为0
> 4. 更新模型
> 5. 终止UF库
>
> 代码中定义了多个辅助函数，用于获取部件所有特征、获取特征的所有表达式、解析表达式字符串等。同时，使用了UF库的通用错误处理函数report_error来报告错误。整体来看，代码结构清晰，功能明确，是一个典型的NX二次开发用户函数实现。
>
