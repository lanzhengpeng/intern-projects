### 【1692】open and save all parts in directory tree 打开并保存目录树中的所有零件

#### 代码

```cpp
    /*HEAD OPEN_AND_SAVE_ALL_PARTS_IN_DIRECTORY_TREE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*internal added to create preview images*/  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        int  
            ir3 = 0,  
            cnt,  
            type,  
            resp;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (UF_CALL(UF_initialize())) return;  
        while ( uc1600 ("Enter path", dirspec, &ir3 ) == 5 && ir3 > 1 )  
        {  
            cnt = 0;  
            if ((resp = uc4508(dirspec, (1<<10)|(1<<11)|(1<<13), 0, "*.prt")) > 0)  
            {  
                while ((resp = uc4518()) < 2)  
                {  
                    switch (resp)  
                    {  
                        case 0:  
                            type = uc4601();  
                            if (type == 100)  
                                UF_CALL(uc4509());  
                            else  
                            {  
                                UF_CALL(uc4519(fspec));  
                                printf("%d.  %s\n", ++cnt, fspec);  
                                UF_CALL(UF_PART_open(fspec, &part, &status));  
                                if (status.n_parts > 0) report_load_status(&status);  
                                if (!status.failed)  
                                {  
                                    printf("\topened OK - saving...\n");  
                                    UF_CALL(UF_PART_save());  
                                    UF_PART_close_all();  
                                }  
                            }  
                            break;  
                        case 1:  
                            UF_CALL(uc4549());  
                            break;  
                        default:  
                            UF_CALL(resp);  
                            break;  
                    }  
                }  
                UF_CALL(uc4548());  
                printf("\nRefiled %d parts.\n", cnt);  
            }  
            else  
                UF_CALL(resp);  
        }  
        UF_CALL(UF_terminate());  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    /*external*/  
    int main( void )  
    {  
        int  
            cnt,  
            type,  
            resp;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (UF_CALL(UF_initialize())) return 1;  
        while (printf("Enter directory path:\n") && gets(dirspec) &&  
            strlen(dirspec))  
        {  
            cnt = 0;  
            uc4508(dirspec, (1<<10)|(1<<11)|(1<<13), 0, "*.prt");  
            while ((resp = uc4518()) < 2)  
            {  
                switch (resp)  
                {  
                    case 0:  
                        type = uc4601();  
                        if (type == 100)  
                            UF_CALL(uc4509());  
                        else  
                        {  
                            UF_CALL(uc4519(fspec));  
                            printf("%d.  %s\n", ++cnt, fspec);  
                            UF_CALL(UF_PART_open(fspec, &part, &status));  
                            if (status.n_parts > 0) report_load_status(&status);  
                            if (!status.failed)  
                            {  
                                printf("\topened OK - saving...\n");  
                                UF_CALL(UF_PART_save());  
                                UF_PART_close_all();  
                            }  
                        }  
                        break;  
                    case 1:  
                        UF_CALL(uc4549());  
                        break;  
                    default:  
                        UF_CALL(resp);  
                        break;  
                }  
            }  
            UF_CALL(uc4548());  
            printf("\nRefiled %d parts.\n", cnt);  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是从指定目录中打开并保存所有的prt文件。
>
> 1. 首先，代码包含了必要的NX Open C++头文件，并定义了ECHO和UF_CALL宏，用于打印错误信息和调用NX函数。
> 2. report_error函数用于打印函数调用失败的错误信息。
> 3. report_load_status函数用于打印打开每个prt文件的状态信息。
> 4. ufusr函数是NX二次开发的入口函数，它实现了以下功能：初始化NX系统不断提示用户输入目录路径对于每个路径，调用NX函数查询所有prt文件遍历查询结果，打开每个prt文件，并保存打印打开每个prt文件的状态信息如果打开成功，则保存文件关闭所有打开的prt文件打印保存prt文件的总数终止NX系统
> 5. 初始化NX系统
> 6. 不断提示用户输入目录路径
> 7. 对于每个路径，调用NX函数查询所有prt文件
> 8. 遍历查询结果，打开每个prt文件，并保存
> 9. 打印打开每个prt文件的状态信息
> 10. 如果打开成功，则保存文件
> 11. 关闭所有打开的prt文件
> 12. 打印保存prt文件的总数
> 13. 终止NX系统
> 14. main函数是程序的入口点，它调用ufusr函数来实现prt文件打开和保存的功能。
> 15. ufusr_ask_unload函数用于在二次开发模块卸载时立即卸载。
>
> 综上所述，这段代码实现了NX环境下prt文件的批量打开和保存功能，通过用户交互指定目录，遍历目录下所有prt文件，打开并保存，并提供了详细的错误和状态信息打印。
>
