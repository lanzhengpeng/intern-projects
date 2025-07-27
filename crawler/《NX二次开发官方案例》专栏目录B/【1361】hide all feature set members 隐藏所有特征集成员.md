### 【1361】hide all feature set members 隐藏所有特征集成员

#### 代码

```cpp
    /*HEAD HIDE_ALL_FEATURE_SET_MEMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static int ask_all_feature_sets(tag_t part, tag_p_t *feats)  
    {  
        int  
            ii,  
            is_member,  
            n_sets;  
        tag_t  
            feat = NULL_TAG,  
            *sets;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_is_feature_a_set_member(feat, &is_member));  
            if (is_member)  
            {  
                UF_CALL(UF_MODL_ask_sets_of_member(feat, &sets, &n_sets));  
                for (ii = 0; ii < n_sets; ii++)  
                    UF_CALL(UF_MODL_put_list_item(feat_list, sets[ii]));  
                if (n_sets > 0) UF_free(sets);  
            }  
        }  
        return (make_an_array(&feat_list, feats));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            hide,  
            ii,  
            n_sets;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *sets;  
        if ((n_sets = ask_all_feature_sets(part, &sets)) > 0)  
        {  
            WRITE_D(n_sets);  
            for (ii = 0; ii < n_sets; ii++)  
            {  
                WRITE_D(ii);  
                report_feature_name_and_type("sets[ii]", sets[ii]);  
                WRITE("Setting to ");  
                hide = TRUE;  
                WRITE_D(hide);  
                UF_CALL(UF_MODL_edit_set_hide_state(sets[ii], &hide));  
                WRITE("Previous setting ");  
                WRITE_D(hide);  
            }  
            UF_CALL(UF_MODL_update());  
            UF_free(sets);  
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

> 这段代码是一个NX Open的UF程序，其主要功能是获取当前显示部件中的所有特征集，然后遍历这些特征集，并隐藏或显示它们。下面是代码的主要内容和步骤：
>
> 1. 包含头文件：代码包含了NX Open的UF模块头文件，如uf.h、uf_ui.h等。
> 2. 错误处理宏：定义了UF_CALL宏，用于检查UF函数的返回码，并在出错时输出错误信息。
> 3. 内存分配函数：实现了allocate_memory函数，用于分配内存并检查返回码。
> 4. 特征集列表处理函数：实现了make_an_array函数，用于将特征集列表转换为数组，并释放列表。
> 5. 获取特征集函数：实现了ask_all_feature_sets函数，用于获取部件中的所有特征集。
> 6. 隐藏/显示特征集：在do_it函数中，遍历获取到的特征集列表，使用edit_set_hide_state函数来隐藏或显示每个特征集。
> 7. 更新显示：在隐藏/显示特征集后，调用update函数来更新NX的显示。
> 8. UF初始化和终止：在ufusr函数中，先调用UF_initialize进行初始化，然后执行do_it函数，最后调用UF_terminate进行终止。
> 9. 卸载处理：实现了ufusr_ask_unload函数，用于立即卸载UF程序。
>
> 总体来看，该代码实现了在NX中获取特征集并隐藏/显示它们的功能。
>
