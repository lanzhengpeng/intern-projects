### 【3030】update all features 更新所有特征

#### 代码

```cpp
    /*HEAD UPDATE_ALL_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是一个在V18版本中新增的函数，用于打印系统日志。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_new,  
            n_post,  
            n_pre;  
        tag_t  
            feat;  
        char  
            *feat_name;  
        uf_list_p_t  
            post_list,  
            pre_list;  
        UF_CALL(UF_MODL_ask_suppress_list(&pre_list));  
        UF_CALL(UF_MODL_ask_list_count(pre_list, &n_pre));  
        UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_SUPPRESS));  
        if (!UF_CALL(UF_MODL_update_all_features()))  
            printf("\tAll features updated OK.\n");  
        UF_CALL(UF_MODL_ask_suppress_list(&post_list));  
        UF_CALL(UF_MODL_ask_list_count(post_list, &n_post));  
        for (ii = 0; ii < n_post; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(post_list, ii, &feat));  
            UF_CALL(UF_MODL_put_list_item(pre_list, feat));  
            UF_CALL(UF_MODL_ask_list_count(pre_list, &n_new));  
            if (n_new != n_pre)  
            {  
                UF_CALL(UF_MODL_ask_feat_name(feat, &feat_name));  
                printf("\t%s suppressed on update error\n", feat_name);  
                UF_free(feat_name);  
                n_pre = n_new;  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&pre_list));  
        UF_CALL(UF_MODL_delete_list(&post_list));  
        UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_NO_OPTION));  
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
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("\t%s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        tag_t  
            part;  
        UF_ASSEM_options_t  
            assy_options;  
        UF_PART_load_status_t  
            status;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            if (uc4621(part_name) == 1)  
            {  
                printf("%s\n", part_name);  
                /* Do not load components if it is Assembly part 里海译:请勿加载组件，如果是装配部件。 */  
                UF_CALL(UF_ASSEM_ask_assem_options(&assy_options));  
                assy_options.load_components = UF_ASSEM_dont_load_components;  
                UF_CALL(UF_ASSEM_set_assem_options(&assy_options));  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0)  
                    report_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_PART_close_all(); /* Cleans up undo files 里海译:清理撤销文件 */  
                }  
            }  
            else  
            {  
                printf("\nThis program requires a part file name as input.\n");  
                printf("The syntax for this program is as follows:\n\n");  
                printf("\t%s <part_name>\n", argv[0]);  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要功能是更新所有特征并处理更新失败的特性。以下是代码的主要功能：
>
> 1. 初始化和终止：使用UF_initialize()和UF_terminate()初始化和终止NX环境。
> 2. 更新所有特征：调用UF_MODL_update_all_features()来更新当前打开的NX部件的所有特征。
> 3. 处理更新失败的特性：更新失败的特征会被添加到抑制列表中，代码会遍历这些特征，并输出更新失败的特征名称。
> 4. 错误报告：使用自定义的report_error()函数来报告调用NX API时的错误。
> 5. 命令行参数解析：主函数接收命令行参数指定的部件文件名，并打开该部件进行特征更新。
> 6. 清理撤销文件：更新完成后，调用UF_PART_close_all()来清理撤销文件。
> 7. 支持组件不加载：如果是装配体，可以选择不加载组件进行更新。
>
> 总体来说，这段代码实现了NX部件特征批量更新的功能，并提供了错误处理和撤销文件清理等辅助功能。
>
