### 【2278】report parts features relatives 报告部件特征关系

#### 代码

```cpp
    /*HEAD REPORT_PARTS_FEATURES_RELATIVES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的功能。

翻译如下：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
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
    static void do_it(void)  
    {  
        int  
            ii,jj,kk,  
            n_children,  
            n_features,  
            n_parents;  
        tag_t  
            *children,  
            *features,  
            part = UF_PART_ask_display_part(),  
            *parents;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        n_features = ask_part_feats(part, &features);  
        WRITE_D(n_features);  
        for (ii = 0; ii < n_features; ii++)  
        {  
            sprintf(name, "features[%d]", ii);  
            report_feature_name_and_type(name, features[ii]);  
            UF_CALL(UF_MODL_ask_feat_relatives(features[ii], &n_parents, &parents,  
                &n_children, &children));  
            for (jj = 0; jj < n_parents; jj++)  
            {  
            /*  Begin work around to PR 1089565 里海译:开始围绕PR 1089565进行工作。 */  
                for (kk = 0; kk < n_features; kk++)  
                    if (parents[jj] == features[kk]) break;  
                if (kk == n_features) continue;  
            /*  End work around to PR 1089565 里海译:End work around to PR 1089565 */  
                sprintf(name, "  parents[%d]", jj);  
                report_feature_name_and_type(name, parents[jj]);  
            }  
            if (n_parents > 0) UF_free(parents);  
            for (jj = 0; jj < n_children; jj++)  
            {  
            /*  Begin work around to PR 1089565 里海译:翻译如下：

开始围绕PR 1089565进行工作。 */  
                for (kk = 0; kk < n_features; kk++)  
                    if (children[jj] == features[kk]) break;  
                if (kk == n_features) continue;  
            /*  End work around to PR 1089565 里海译:翻译 End work around to PR 1089565 的意思是结束对编号为 1089565 的公关工作的回避。 */  
                sprintf(name, "    children[%d]", jj);  
                report_feature_name_and_type(name, children[jj]);  
            }  
            if (n_children > 0) UF_free(children);  
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

> 这段代码是一个NX二次开发示例，主要功能是遍历当前显示零件中的所有特征，并输出特征名称、类型、父特征和子特征信息。主要步骤包括：
>
> 1. 初始化：调用UF_initialize()函数进行NX的初始化。
> 2. 获取当前显示零件：使用UF_PART_ask_display_part()获取当前显示的零件对象。
> 3. 遍历零件中的所有体：通过UF_OBJ_cycle_objs_in_part()函数循环遍历零件中的所有体对象。
> 4. 获取每个体中的特征：使用UF_MODL_ask_body_feats()获取每个体中的特征列表。
> 5. 输出特征信息：调用UF_MODL_ask_feat_name()和UF_MODL_ask_feat_type()输出特征名称和类型。
> 6. 获取特征关系：通过UF_MODL_ask_feat_relatives()获取特征的父特征和子特征列表。
> 7. 输出父特征和子特征信息：遍历父特征和子特征列表，输出每个特征的信息。
> 8. 释放内存：释放分配的内存。
> 9. 终止NX：调用UF_terminate()函数终止NX。
> 10. 询问卸载：在用户请求卸载时，返回UF_UNLOAD_IMMEDIATELY。
>
> 这段代码主要用于展示NX的二次开发能力，通过遍历特征并输出信息，可以帮助开发者熟悉NX的二次开发接口。
>
