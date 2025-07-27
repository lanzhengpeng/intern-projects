### 【1389】highlight mirror set feature pairs 高亮显示镜像特征集特征对

#### 代码

```cpp
    /*HEAD HIGHLIGHT_MIRROR_SET_FEATURE_PAIRS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能。 */  
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
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static void explode_feature_set(tag_t set)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats;  
        uf_list_p_t  
            list;  
        if (!UF_MODL_ask_all_members_of_set(set, &feats, &n_feats))  
        {  
            UF_CALL(UF_MODL_edit_set_members(set, NULL, 0));  
            UF_CALL(UF_MODL_create_list(&list));  
            UF_CALL(UF_MODL_put_list_item(list, set));  
            UF_CALL(UF_MODL_delete_feature(list));  
            UF_CALL(UF_MODL_delete_list(&list));  
            for (ii = 0; ii < n_feats; ii++) explode_feature_set(feats[ii]);  
            if (n_feats > 0) UF_free(feats);  
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
    static int ask_whats_missing(tag_t *old, int n_old, tag_t *now, int n_now,  
        tag_t **missing)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_old; ii++)  
        {  
            for (jj = 0; jj < n_now; jj++)  
                if (old[ii] == now[jj]) break;  
            if (jj == n_now) UF_CALL(UF_MODL_put_list_item(list, old[ii]));  
        }  
        return (make_an_array(&list, missing));  
    }  
    static void highlight_mirror_set_feature_pairs(tag_t mirror_set)  
    {  
        int  
            ii,  
            jj,  
            nif,  
            nnf,  
            nof;  
        tag_t  
            *g_feats,  
            *i_feats,  
            *n_feats,  
            *o_feats,  
            out,  
            plane;  
        char  
            *iname,  
            *oname,  
            msg[133];  
        UF_UNDO_mark_id_t  
            mark_1,  
            mark;  
        UF_MODL_update_option_t  
            old_option;  
        UF_CALL(UF_MODL_ask_update_fail_option(&old_option));  
        if (old_option != UF_MODL_UPDATE_SUPPRESS)  
            UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_SUPPRESS));  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        UF_CALL(UF_MODL_ask_features_of_mirror_set(mirror_set, &i_feats, &nif));  
        for (ii = 0; ii < nif; ii++) explode_feature_set(i_feats[ii]);  
        UF_CALL(UF_MODL_ask_features_of_mirror_set(mirror_set, &i_feats, &nif));  
        UF_CALL(UF_MODL_ask_plane_of_mirror_set(mirror_set, &plane));  
        UF_CALL(UF_MODL_ask_all_members_of_set(mirror_set, &o_feats, &nof));  
        for (ii = 0; ii < nif; ii++)  
        {  
            out = i_feats[ii];  
            for (jj = ii; jj < nif - 1; jj++) i_feats[jj] = i_feats[jj+1];  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_1));  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            UF_CALL(UF_MODL_edit_mirror_set(mirror_set, i_feats, nif - 1, plane));  
            UF_CALL(UF_MODL_update());  
            UF_CALL(UF_MODL_ask_all_members_of_set(mirror_set, &n_feats, &nnf));  
            UF_CALL(UF_UNDO_undo_to_mark(mark_1, NULL));  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            UF_CALL(UF_DISP_regenerate_display());  
            for (jj = nif - 1; jj > ii; jj--) i_feats[jj] = i_feats[jj-1];  
            i_feats[ii] = out;  
            if (ask_whats_missing(o_feats, nof, n_feats, nnf, &g_feats) > 0)  
            {  
                UF_DISP_set_highlight(i_feats[ii], TRUE);  
                UF_DISP_set_highlight(g_feats[0], TRUE);  
                UF_CALL(UF_MODL_ask_feat_name(i_feats[ii], &iname));  
                UF_CALL(UF_MODL_ask_feat_name(g_feats[0], &oname));  
                sprintf(msg, "%s <-> %s", iname, oname);  
                uc1601(msg, TRUE);  
                UF_free(iname);  
                UF_free(oname);  
                UF_DISP_set_highlight(i_feats[ii], FALSE);  
                UF_DISP_set_highlight(g_feats[0], FALSE);  
                UF_free(g_feats);  
            }  
            if (nnf > 0) UF_free(n_feats);  
        }  
        if (nif > 0) UF_free(i_feats);  
        if (nof > 0) UF_free(o_feats);  
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
        if (old_option != UF_MODL_UPDATE_SUPPRESS)  
            UF_CALL(UF_MODL_set_update_fail_option(old_option));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((feat = ask_next_feature_of_type(part, "MIRROR_SET", feat))  
            != NULL_TAG) highlight_mirror_set_feature_pairs(feat);  
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

> 根据代码的内容，这是一段用于NX的二次开发代码，其主要功能是高亮显示镜像特征集中的特征对。以下是代码的主要功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个函数ask_next_feature_of_type，用于遍历零件中的特征并获取下一个指定类型的特征。
> 3. 定义了一个函数explode_feature_set，用于删除特征集中的所有特征。
> 4. 定义了一个函数allocate_memory，用于申请内存。
> 5. 定义了一个函数make_an_array，用于将列表转换为数组。
> 6. 定义了一个函数ask_whats_missing，用于找出一个数组中缺失的元素。
> 7. 定义了一个函数highlight_mirror_set_feature_pairs，这是主要函数，用于高亮显示镜像特征集中的特征对：获取镜像特征集的所有输入特征和输出特征。遍历输入特征，依次隐藏每个输入特征，然后获取新的输出特征。比较隐藏前后的输出特征，找出新增的特征，即输入特征对应的输出特征。高亮显示输入特征和新增的输出特征，并显示它们的名称。
> 8. 获取镜像特征集的所有输入特征和输出特征。
> 9. 遍历输入特征，依次隐藏每个输入特征，然后获取新的输出特征。
> 10. 比较隐藏前后的输出特征，找出新增的特征，即输入特征对应的输出特征。
> 11. 高亮显示输入特征和新增的输出特征，并显示它们的名称。
> 12. 定义了一个函数do_it，用于遍历零件中的所有镜像特征集，并调用highlight_mirror_set_feature_pairs函数。
> 13. 定义了ufusr函数，这是NX的二次开发入口函数，用于调用do_it函数。
> 14. 定义了ufusr_ask_unload函数，用于卸载二次开发模块。
>
> 综上所述，这段代码通过遍历零件中的所有镜像特征集，并高亮显示每个输入特征对应的输出特征，从而帮助用户清晰地查看和理解特征集中的特征关系。
>
