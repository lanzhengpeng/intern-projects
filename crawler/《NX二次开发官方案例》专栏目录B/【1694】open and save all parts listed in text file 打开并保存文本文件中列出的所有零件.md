### 【1694】open and save all parts listed in text file 打开并保存文本文件中列出的所有零件

#### 代码

```cpp
    /*HEAD OPEN_AND_SAVE_ALL_PARTS_LISTED_IN_TEXT_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，用于在 uftrace 的输出中打印系统调用日志。 */  
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
    int main( void )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            fspec[MAX_FSPEC_SIZE+1] = { "" },  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        FILE  
            *the_file;  
        if (UF_CALL(UF_initialize())) return 1;  
        while (printf("Enter text file name:\n") && gets(fspec) &&  
            strlen(fspec))  
        {  
            if ((the_file = fopen(fspec, "r")) != NULL)  
            {  
                while ((fgets(part_name, MAX_FSPEC_SIZE, the_file)) != NULL)  
                {  
                    part_name[strlen(part_name)-1] = '\0';  /* overwrite \n 里海译:重写(覆盖) */  
                    printf("%d.  %s\n", ++cnt, part_name);  
                    UF_CALL(UF_PART_open(part_name, &part, &status));  
                    if (status.n_parts > 0) check_load_status(&status);  
                    if (!status.failed)  
                    {  
                        printf("\topened OK - saving...\n");  
                        UF_CALL(UF_PART_save());  
                        UF_PART_close_all();  
                    }  
                }  
                fclose(the_file);  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个用于NX的二次开发程序，主要功能是打开和保存文本文件中列出的所有零件。
>
> 程序流程如下：
>
> 1. 初始化NX环境。
> 2. 循环提示用户输入文本文件名。
> 3. 对于每个文件，逐行读取零件名，并尝试打开该零件。
> 4. 打开成功后，保存零件并关闭。
> 5. 打印已处理的零件数。
> 6. 结束NX环境。
>
> 主要函数包括：
>
> 代码中还定义了两个辅助函数：
>
> 整体而言，这是一个简单的NX二次开发示例，演示了基本的零件打开和保存操作。
>
