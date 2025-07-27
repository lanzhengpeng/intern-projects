### 【1918】report all features link faces 报告所有特征的连接面

#### 代码

```cpp
    /*HEAD REPORT_ALL_FEATURES_LINK_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的语句，翻译如下：

UF_print_syslog是在V18版本中新增的。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_features_link_faces(tag_t feature)  
    {  
        logical  
            is_hor;  
        int  
            ii,  
            n_feats;  
        tag_t  
            dir_ref,  
            *feats,  
            target_face,  
            tool_face;  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (UF_CALL(UF_MODL_ask_feat_type(feature, &type))) return;  
        UF_CALL(UF_MODL_ask_feat_name(feature, &name));  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "feature = %d %s (%s)\n", feature, name, type);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        UF_free(name);  
        if (!strcmp(type, "UDF_INS"))  
        {  
            UF_CALL(UF_MODL_ask_all_members_of_set(feature, &feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
                report_features_link_faces(feats[ii]);  
            if (n_feats > 0) UF_free(feats);  
        }  
        else if (!UF_MODL_ask_link_faces(feature, &target_face, &tool_face,  
            &dir_ref, &is_hor))  
        {  
            WRITE_D(target_face);  
            WRITE_D(tool_face);  
            WRITE_D(dir_ref);  
            WRITE_L(is_hor);  
        }  
        else  
        {  
            WRITE("Feature not linked.\n");  
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
        n_features = ask_all_features(part, &features);  
        for (ii = 0; ii < n_features; ii++)  
            report_features_link_faces(features[ii]);  
        if (n_features > 0) UF_free(features);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容，这是一段基于NX的二次开发代码，其主要功能是遍历当前部件中的所有特征，并输出每个特征的信息，包括特征名、类型、是否与面关联等。具体功能如下：
>
> 1. 定义了错误报告函数report_error，用于输出函数调用错误信息。
> 2. 定义了ask_next_feature函数，用于遍历部件中的特征。
> 3. 定义了allocate_memory和make_an_array函数，用于处理NX内存分配和对象列表转换。
> 4. 定义了ask_all_features函数，用于获取部件中的所有特征。
> 5. 定义了WRITE系列宏，用于输出信息到日志窗口。
> 6. 定义了report_features_link_faces函数，用于输出单个特征的信息，包括是否与面关联等。
> 7. 定义了do_it函数，用于遍历部件中的所有特征，并输出信息。
> 8. 定义了ufusr函数，作为NX二次开发的入口函数，在其中初始化NX系统，调用do_it函数，然后终止NX系统。
> 9. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 综上所述，这段代码实现了遍历当前部件中的所有特征，并输出每个特征的信息的功能。
>
