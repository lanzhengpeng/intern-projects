### 【3029】update all features for all parts in directory and save 更新目录中所有零件的特征并保存

#### 代码

```cpp
    /*HEAD UPDATE_ALL_FEATURES_FOR_ALL_PARTS_IN_DIRECTORY_AND_SAVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的内容，UF_print_syslog 是 V18 新增的函数。 */  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
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
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void update_all_features(void)  
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
    static void do_it(char *dirspec)  
    {  
        int  
            cnt = 0,  
            resp;  
        tag_t  
            part;  
        char  
            fname[MAX_FSPEC_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        WRITE(dirspec);  
        WRITE(":\n");  
        uc4508(dirspec, (1<<13)|(1<<11), 0, "*.prt");  
        while ((resp = uc4518()) == 0)  
        {  
            UF_CALL(uc4600(fname));  
            UF_CALL(uc4575(dirspec, 2, fname, part_name));  
            sprintf(msg, "%d.  %s\n", ++cnt, part_name);  
            WRITE(msg);  
            UF_CALL(UF_PART_open(part_name, &part, &status));  
            if (status.n_parts > 0) check_load_status(&status);  
            if (!status.failed)  
            {  
                WRITE("\topened OK - saving...\n");  
                update_all_features();  
                UF_CALL(UF_PART_save());  
                UF_PART_close_all();  
            }  
        }  
        UF_CALL(uc4548());  
        sprintf(msg, "\nProcessed %d parts.\n", cnt);  
        WRITE(msg);  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
    #ifdef _USRDLL  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    #else   
        return TRUE;  
    #endif      
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" };  
        if (UF_CALL(UF_initialize())) return;  
        if (prompt_for_text("Refile parts - Enter directory path", dirspec))  
        {  
            do_it(dirspec);  
        }  
        UF_CALL(UF_terminate());  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    int main( void )  
    {  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" };  
        if (UF_CALL(UF_initialize())) return 1;  
        while (printf("Enter directory path:\n") && gets(dirspec) &&  
            strlen(dirspec))  
        {  
            do_it(dirspec);  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是遍历指定目录下的所有prt文件，并更新每个零件中的所有特征。以下是代码的主要功能：
>
> 1. 包含NX Open C++的必要头文件，并定义了错误报告函数report_error。
> 2. 定义了检查加载状态的函数check_load_status。
> 3. 定义了更新所有特征的函数update_all_features。
> 4. 定义了遍历目录并处理每个prt文件的函数do_it。
> 5. 定义了获取用户输入的目录路径的函数prompt_for_text。
> 6. 定义了UFusr函数，这是NX二次开发程序的入口函数。在该函数中，首先调用UF_initialize初始化NX Open C++环境，然后获取用户输入的目录路径，并调用do_it函数进行处理。
> 7. 定义了main函数作为程序的入口，在main函数中，循环获取用户输入的目录路径，并调用do_it进行处理，直到用户输入空字符串退出。
> 8. 在do_it函数中，首先列出指定目录下的所有prt文件，然后逐个打开每个prt文件，更新特征并保存，最后关闭文件。
> 9. 使用NX Open C++的API实现prt文件的遍历、打开、特征更新、保存等功能。
>
> 总的来说，这段代码提供了一个遍历目录、更新零件特征的NX二次开发示例，具有很高的参考价值。
>
