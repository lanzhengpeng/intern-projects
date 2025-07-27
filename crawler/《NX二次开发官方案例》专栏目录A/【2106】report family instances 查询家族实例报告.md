### 【2106】report family instances 查询家族实例报告

#### 代码

```cpp
    /*HEAD REPORT_FAMILY_INSTANCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中新增的函数。 */  
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
        tag_t   part = UF_ASSEM_ask_work_part();  
        logical is_template;  
        int     fam_cnt, ii;  
        tag_t   *fam_tags;  
        char    *part_name, part_fspec[MAX_FSPEC_SIZE+1];  
        UF_FAM_family_data_t    family_data;  
        UF_PART_ask_part_name(part, part_fspec);  
            printf("\n\n");  
        UF_CALL( UF_PART_is_family_template( part, &is_template));  
        if(is_template)  
        {  
            UF_CALL(UF_PART_ask_families( part, &fam_cnt, &fam_tags));  
                printf("Tag of family of %s: %d\n", part_fspec, fam_tags[0]);  
            UF_CALL( UF_FAM_ask_family_data( fam_tags[0], &family_data));  
                printf("    Number of family members: %d\n", family_data.member_count);  
            for( ii = 0; ii < family_data.member_count; ii++ )  
            {  
                UF_CALL(UF_PART_find_family_instance( fam_tags[0], ii,  
                                              FALSE, TRUE, &part_name));  
                printf("        Family Member #%d: %s\n", ii+1, part_name);  
                UF_free( part_name);  
            }  
            UF_free( fam_tags);  
        }  
        else  
            printf("Work part is not a template part\n");  
    }  
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
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            if (argc > 1)  
            {  
                int  
                    cnt = 0,  
                    ii;  
                logical  
                    new_flag  = FALSE,  
                    save_flag = FALSE;  
                tag_t  
                    part;  
                UF_PART_load_status_t  
                    status;  
                for (ii=1; ii<argc; ii++)  
                {  
                    if (!strcmp(argv[ii], "-save")) save_flag = TRUE;  
                    else if (!strcmp(argv[ii], "-new")) new_flag = TRUE;  
                    else  
                    {  
                        printf("%d.  %s\n", ++cnt, argv[ii]);  
                        if (new_flag)  
                        {  
                            if (!UF_CALL(UF_PART_new(argv[ii], UF_PART_ENGLISH,  
                                &part)))  
                            {  
                                do_it();  
                                if (save_flag)  UF_CALL(UF_PART_save());  
                                UF_PART_close_all();  
                            }  
                        }  
                        else  
                        {  
                            if (UF_CALL(UF_PART_open(argv[ii], &part, &status)))  
                                report_load_status(&status);  
                            else  
                            {  
                                do_it( );  
                                if (save_flag)  UF_CALL(UF_PART_save());  
                                UF_PART_close_all();  
                            }  
                        }  
                    }  
                }  
                printf("\nProcessed %d parts.\n", cnt);  
            }  
            else do_it();  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 根据代码，这是一个NX Open的二次开发示例，其主要功能是：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败的信息。
> 2. 定义了一个函数do_it，用于查询当前工作部件是否为模板，如果是，则查询其所属的族，并打印族的成员信息。
> 3. 定义了ufusr函数，用于初始化和终止NX Open API，并调用do_it函数。
> 4. 定义了main函数，作为程序的入口。它会根据命令行参数来打开指定的部件，或者创建新的部件，然后调用do_it函数，最后保存并关闭部件。
> 5. 在main函数中，还定义了一个函数report_load_status，用于报告部件加载的状态。
> 6. 代码中还使用了NX Open的UF_PART、UF_FAM等API，用于查询部件信息、族信息等。
> 7. 整体来说，这是一个典型的NX Open二次开发示例，涵盖了初始化、错误处理、族管理等功能，具有一定的参考价值。
>
