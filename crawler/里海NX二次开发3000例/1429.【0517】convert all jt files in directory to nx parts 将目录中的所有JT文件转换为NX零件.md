### 【0517】convert all jt files in directory to nx parts 将目录中的所有JT文件转换为NX零件

#### 代码

```cpp
    /*HEAD CONVERT_ALL_JT_FILES_IN_DIRECTORY_TO_NX_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
        if ((resp = uc4508(dirspec, (1<<13)|(1<<11), 0, "*.jt")) < 0)  
        {  
            UF_CALL(resp);  
        }  
        else  
        {  
            while ((resp = uc4518()) == 0)  
            {  
                UF_CALL(uc4600(fname));  
                UF_CALL(uc4575(dirspec, 0, fname, part_name));  
                sprintf(msg, "%d.  %s\n", ++cnt, part_name);  
                WRITE(msg);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    UF_CALL(UF_PART_update_jt_brep(part));  
                    UF_CALL(UF_PART_ask_part_name(part, part_name));  
                    WRITE("\tsaving as");  
                    WRITE(part_name);  
                    WRITE("\n");  
                    UF_CALL(UF_PART_save());  
                    UF_PART_close_all();  
                }  
            }  
            UF_CALL(uc4548());  
            sprintf(msg, "\nProcessed %d jt files.\n", cnt);  
            WRITE(msg);  
        }  
    }  
    #ifdef INTERNAL_UFUN  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    /* 里海 */  
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
    #endif  /* INTERNAL_UFUN 译:INTERNAL_UFUN的中文翻译是“内部函数”。 */  
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

> 这段代码是一个NX Open C++的二次开发代码，其主要功能是将指定目录下的所有JT文件转换成NX部件文件。
>
> 代码的主要逻辑如下：
>
> 1. 定义了错误报告函数report_error，用于在调用UF函数出错时报告错误信息。
> 2. 定义了检查加载状态函数check_load_status，用于检查UF_PART_open加载部件文件的状态，并打印出每个文件的加载状态。
> 3. 定义了处理函数do_it，用于遍历指定目录下的所有JT文件，打开每个文件，更新其BREP数据，然后另存为NX部件文件。
> 4. 在ufusr函数中，通过循环调用do_it函数，处理用户输入的目录路径，实现批量转换JT文件为NX部件文件。
> 5. 在main函数中，通过标准输入获取用户输入的目录路径，并调用do_it函数进行批量转换。
> 6. 定义了ufusr_ask_unload函数，用于在卸载用户函数时立即卸载。
>
> 总的来说，这段代码实现了对指定目录下的JT文件批量转换成NX部件文件的功能，通过UF_PART系列函数实现了NX部件的打开、更新、保存等操作。
>
