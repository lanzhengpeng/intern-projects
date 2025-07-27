### 【1917】report all features dates 报告所有特征日期

#### 代码

```cpp
    /*HEAD REPORT_ALL_FEATURES_DATES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat));  
        return feat;  
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
    static void translate_gmtime(int gm_time, char *time_buff)  
    {  
        time_t  
            the_time;  
        struct tm  
            *time_ptr;  
        char  
            *time_format = "%d %b %Y %H:%M";  
        the_time = (time_t) gm_time;  
        time_ptr = localtime (&the_time);  
        strftime( time_buff, 21*sizeof(char), time_format, time_ptr);  
    }  
    static void do_it(void)  
    {  
        int  
            cre_version,  
            gm_time,  
            mod_version,  
            n_histories,  
            version;  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            *machine,  
            msg[MAX_LINE_SIZE+1],  
            *program,  
            time_buff[21],  
            *user;  
        UF_PART_history_list_p_t  
            history_list;  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        UF_CALL(UF_PART_ask_part_history(part, history_list));  
        UF_CALL(UF_PART_ask_num_histories(history_list, &n_histories));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            report_feature_name_and_type("feat", feat);  
            UF_CALL(UF_OBJ_ask_cre_mod_versions(feat, &cre_version, &mod_version));  
            UF_CALL(UF_PART_ask_nth_history(history_list, n_histories-cre_version,  
                &program, &user, &machine, &version, &gm_time));  
            translate_gmtime(gm_time, time_buff);  
            sprintf(msg, "  first created: %s\n", time_buff);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_CALL(UF_PART_ask_nth_history(history_list, n_histories-mod_version,  
                &program, &user, &machine, &version, &gm_time));  
            translate_gmtime(gm_time, time_buff);  
            sprintf(msg, "  last modified: %s\n", time_buff);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        UF_CALL(UF_PART_delete_history_list(history_list));  
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

> 这段代码是用于NX的二次开发，其主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出错误代码和错误信息。
> 2. 定义了一个获取特征名称和类型的函数report_feature_name_and_type。
> 3. 定义了一个将GMT时间转换为本地时间的函数translate_gmtime。
> 4. 主函数do_it()首先获取当前显示的零件，然后遍历零件中的所有特征，打印每个特征的名称、类型、创建时间和最后修改时间。
> 5. ufusr()是NX调用的入口函数，其中初始化NX环境，调用do_it()函数，然后终止NX环境。
> 6. ufusr_ask_unload()函数用于卸载该二次开发代码。
>
> 总的来说，这段代码的功能是遍历当前显示的零件中的所有特征，并打印出每个特征的名称、类型、创建时间和最后修改时间。
>
