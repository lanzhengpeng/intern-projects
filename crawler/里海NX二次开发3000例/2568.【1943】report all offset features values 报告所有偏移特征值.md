### 【1943】report all offset features values 报告所有偏移特征值

#### 代码

```cpp
    /*HEAD REPORT_ALL_OFFSET_FEATURES_VALUES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18版本中新增的功能。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats,  
            part = UF_PART_ask_display_part();  
        char  
            *feat_name,  
            *feat_type,  
            msg[UF_UI_MAX_STRING_LEN+1],  
            *value;  
        UF_CALL(UF_UI_open_listing_window());  
        n_feats = ask_part_feats(part, &feats);  
        for (ii = 0; ii < n_feats; ii++)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feats[ii], &feat_type));  
            if (!strcmp(feat_type, "OFFSET"))  
            {  
                UF_CALL(UF_MODL_ask_feat_name(feats[ii], &feat_name));  
                UF_CALL(UF_MODL_ask_offset_parms(feats[ii], 0, &value));  
                sprintf(msg, "%s - %s\n", feat_name, value);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_free(feat_name);  
                UF_free(value);  
            }  
            UF_free(feat_type);  
        }  
        if (n_feats > 0) UF_free(feats);  
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

> 这段NX二次开发代码的主要功能是：
>
> 1. 报告当前显示部件的所有偏置特征及其值。
> 2. 通过自定义的report_error函数来报告函数调用错误。
> 3. 定义ask_next_body函数来遍历部件中的所有实体，获取下一个实体。
> 4. 定义allocate_memory函数来申请内存。
> 5. 定义make_an_array函数将列表转换为数组。
> 6. 定义ask_part_feats函数来获取部件中的所有特征。
> 7. 在do_it函数中，首先打开列表窗口，然后获取当前显示部件的所有特征，并遍历这些特征，如果特征类型为OFFSET，则获取特征名称和值，并输出到列表窗口。
> 8. ufusr函数首先初始化NX，然后调用do_it函数，最后终止NX。
> 9. ufusr_ask_unload函数用于卸载该UF。
> 10. 整个代码主要用于获取部件中的所有OFFSET特征及其值，并在列表窗口中输出。
>
> 总体来说，这段代码通过遍历部件实体，获取特征，并输出偏置特征名称和值，实现了一个简单的二次开发功能。
>
