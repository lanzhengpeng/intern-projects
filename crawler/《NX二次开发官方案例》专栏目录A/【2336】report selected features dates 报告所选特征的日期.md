### 【2336】report selected features dates 报告所选特征的日期

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_FEATURES_DATES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
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
            ii,  
            mod_version,  
            n_feats,  
            n_histories,  
            version;  
        tag_t  
            *feats,  
            part = UF_PART_ask_display_part();  
        char  
            *machine,  
            msg[UF_UI_MAX_STRING_LEN+1],  
            *program,  
            time_buff[21],  
            *user;  
        UF_PART_history_list_p_t  
            history_list;  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        UF_CALL(UF_PART_ask_part_history(part, history_list));  
        UF_CALL(UF_PART_ask_num_histories(history_list, &n_histories));  
        while ((n_feats = select_features("Select features", &feats)) > 0)  
        {  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                report_feature_name_and_type("feature", feats[ii]);  
                UF_CALL(UF_OBJ_ask_cre_mod_versions(feats[ii], &cre_version,  
                    &mod_version));  
                for (ii = 0; ii < n_histories; ii++)  
                {  
                    UF_CALL(UF_PART_ask_nth_history(history_list, ii,  
                        &program, &user, &machine, &version, &gm_time));  
                    if (version == cre_version) break;  
                }  
                if (ii < n_histories)  
                {  
                    translate_gmtime(gm_time, time_buff);  
                    sprintf(msg, "  first created: %s\n", time_buff);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
                for (ii = 0; ii < n_histories; ii++)  
                {  
                    UF_CALL(UF_PART_ask_nth_history(history_list, ii,  
                        &program, &user, &machine, &version, &gm_time));  
                    if (version == mod_version) break;  
                }  
                if (ii < n_histories)  
                {  
                    translate_gmtime(gm_time, time_buff);  
                    sprintf(msg, "  last modified: %s\n", time_buff);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_free(feats);  
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

> 根据提供的NX二次开发代码，该代码主要实现以下功能：
>
> 1. 初始化和终止：在 ufusr 函数中，使用 UF_initialize 和 UF_terminate 来初始化和终止NX系统。
> 2. 错误报告：定义了 report_error 函数，用于打印函数执行过程中的错误信息。
> 3. 特征选择：定义了 select_features 函数，用于让用户选择特征，并返回所选特征的数量。
> 4. 特征名称和类型输出：定义了 report_feature_name_and_type 函数，用于输出特征的名称和类型。
> 5. 时间格式转换：定义了 translate_gmtime 函数，用于将GMT时间转换为本地时间格式。
> 6. 执行主体：在 do_it 函数中，获取当前显示的零件，创建零件历史记录列表，遍历每个特征，并输出其名称、类型、首次创建时间和最后修改时间。
> 7. 询问卸载：在 ufusr_ask_unload 函数中，返回立即卸载。
>
> 综上所述，该代码实现了遍历零件中的特征，并输出每个特征的相关信息，包括名称、类型、首次创建时间和最后修改时间。
>
