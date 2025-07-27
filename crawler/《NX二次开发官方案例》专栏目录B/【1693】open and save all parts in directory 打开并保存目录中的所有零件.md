### 【1693】open and save all parts in directory 打开并保存目录中的所有零件

#### 代码

```cpp
    /*HEAD OPEN_AND_SAVE_ALL_PARTS_IN_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog 是在 V18 中新增的，只回答翻译，不要无关的内容。 */  
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
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" };  
        if (UF_CALL(UF_initialize())) return;  
        while (prompt_for_text("Refile parts - Enter directory path", dirspec))  
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

> 这段代码是一个用于批量打开和保存NX部件文件的实用程序。主要功能包括：
>
> 1. 初始化和终止NX环境。
> 2. 提示用户输入部件所在的目录路径。
> 3. 遍历目录下所有后缀为.prt的部件文件，并逐个打开。
> 4. 每打开一个部件，会显示部件的名称和状态。
> 5. 如果打开成功，则保存该部件，然后关闭。
> 6. 在打开失败时，会显示错误信息。
> 7. 最后输出处理过的部件总数。
> 8. 提供一个unload函数，用于卸载当前加载的所有部件。
>
> 这段代码充分利用了NX提供的文件接口，实现了自动化处理多个部件文件的功能，提高了工作效率。
>
