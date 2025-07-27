### 【1768】process all parts in directory tree in external mode 在独立模式下处理目录树中的所有零件

#### 代码

```cpp
    /*HEAD PROCESS_ALL_PARTS_IN_DIRECTORY_TREE_IN_EXTERNAL_MODE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
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
    static void do_it(void)  
    {  
        /* Add your per part processing commands here 里海译:翻译：在此处添加您的每部分处理命令。 */  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                ECHO("  ");  
                ECHO(status->file_names[ii]);  
                ECHO(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO(msg);  
                ECHO("\n");  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe <top_folder1_path> <top_folder2_path> [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0,  
            resp,  
            type;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(dirspec) == 1)  
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
                                    do_it();  
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
                printf("\nFixed up %d parts under %s.\n", cnt, dirspec);  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，用于递归遍历一个或多个指定的顶级目录树，并对每个目录树下的所有prt文件执行特定的处理。
>
> 代码的主要功能包括：
>
> 1. 初始化NX环境，并获取命令行参数指定的顶级目录路径。
> 2. 对于每个顶级目录，递归遍历所有子目录，并获取所有后缀为.prt的文件路径。
> 3. 打开每个prt文件，获取部件对象，并调用do_it函数执行特定处理。
> 4. 如果文件加载成功，调用UF_PART_save()保存修改，并调用UF_PART_close_all()关闭所有打开的部件。
> 5. 如果文件加载失败，报告错误信息。
> 6. 处理完一个目录后，打印处理了多少个prt文件。
> 7. 终止NX环境。
>
> 作者可以在do_it函数中添加自定义的每部分处理命令。此外，代码还包括错误报告和目录遍历的实用函数。
>
