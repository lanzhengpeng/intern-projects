### 【2728】set all taper angle expressions to zero 将所有锥度角表达式设为零

#### 代码

```cpp
    /*HEAD SET_ALL_TAPER_ANGLE_EXPRESSIONS_TO_ZERO CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您的要求，翻译如下：

UF_print_syslog 是 V18 版本中新增的函数。

希望这个翻译符合您的要求。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
            msg[MAX_LINE_SIZE+1],  
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
                    if (!strcmp(descs[jj], "Taper Angle"))  
                        UF_CALL(UF_MODL_put_list_item(exp_list, exps[jj]));  
                }  
                UF_free(exps);  
                UF_free_string_array(n_exps, descs);  
            }  
        }  
        if (n_features > 0) UF_free(features);  
        n_exps = make_an_array(&exp_list, &exps);  
        UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_ACCEPT_ALL));  
        for (ii = n_exps; ii > 0; ii--)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii-1], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exps[ii-1]));  
            sprintf(zero, "%s=0", lhs);  
            sprintf(msg, "%d.  %s\n", ii, zero);  
            WRITE(msg);  
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

> 这段代码的主要功能是将NX中所有特征的锥度角度表达式设置为0。具体步骤如下：
>
> 1. 首先调用UF_MODL_create_list创建一个表达式列表，用于存储找到的锥度角度表达式。
> 2. 通过遍历所有特征，并调用UF_MODL_ask_exp_desc_of_feat获取每个特征的表达式描述和表达式标签数组。如果表达式描述为"Taper Angle"，则将该表达式添加到表达式列表中。
> 3. 将表达式列表转换为表达式标签数组，并遍历该数组。
> 4. 对于每个锥度角度表达式，先调用UF_MODL_dissect_exp_string分解表达式字符串，得到左边表达式和右边表达式。
> 5. 然后将左边表达式和"=0"拼接成新的表达式字符串。
> 6. 调用UF_MODL_edit_exp将新的表达式字符串编辑到NX中，并调用UF_MODL_update更新NX。
> 7. 重复步骤4-6，直到所有锥度角度表达式都被设置为0。
> 8. 释放相关内存，结束NX初始化。
>
> 总的来说，这段代码通过遍历所有特征，找到锥度角度表达式，并将其修改为0，实现了将NX中所有特征的锥度角度表达式设置为0的功能。
>
