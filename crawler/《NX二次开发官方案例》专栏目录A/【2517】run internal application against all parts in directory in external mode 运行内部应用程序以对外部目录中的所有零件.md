### 【2517】run internal application against all parts in directory in external mode 运行内部应用程序以对外部目录中的所有零件

#### 代码

```cpp
    /*HEAD RUN_INTERNAL_APPLICATION_AGAINST_ALL_PARTS_IN_DIRECTORY_IN_EXTERNAL_MODE CCC UFUN */  
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
            cnt = 0,  
            resp;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            libname[MAX_FSPEC_SIZE+1] = { "" },  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        UF_load_f_p_t  
            do_ufusr;  
        if (UF_CALL(UF_initialize())) return 1;  
        while (  
            printf("Enter path to internal application to run against parts:\n")  
            && gets(libname) &&  
            strlen(libname) &&  
            printf("Enter directory path containing parts to process:\n")  
            && gets(dirspec) &&  
            strlen(dirspec))  
        {  
            if (UF_CALL(UF_load_library(libname, "ufusr", &do_ufusr)))  
                continue;  
            uc4508(dirspec, (1<<13)|(1<<11), 0, "*.prt");  
            while ((resp = uc4518()) == 0)  
            {  
                UF_CALL(uc4600(fname));  
                UF_CALL(uc4575(dirspec, 2, fname, part_name));  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_ufusr();  
                    UF_PART_close_all();  
                }  
            }  
            UF_CALL(uc4548());  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_unload_library(libname));  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，其功能是在一个目录中找到所有部件，并为每个部件加载一个用户自定义的内部应用程序。
>
> 主要功能如下：
>
> 1. 代码首先包含必要的NX Open C++头文件，并定义了一些宏，用于打印错误信息。
> 2. 定义了一个报告错误的函数report_error，用于打印出函数调用失败时的错误信息。
> 3. 定义了一个检查加载状态的函数check_load_status，用于打印出部件加载失败的原因。
> 4. 主函数首先调用UF_initialize初始化NX Open。
> 5. 使用一个循环，提示用户输入要加载的内部应用程序库的路径和包含部件的目录路径。
> 6. 在循环中，调用UF_load_library加载内部应用程序库，并使用NX系统函数uc4508获取目录中的所有部件文件名。
> 7. 再使用一个循环，遍历所有部件文件名，打开每个部件，并调用内部应用程序函数。
> 8. 如果部件加载失败，调用check_load_status打印失败原因。
> 9. 如果部件加载成功，调用内部应用程序函数。
> 10. 最后，调用UF_unload_library卸载内部应用程序库，调用UF_terminate终止NX Open。
>
> 这个应用程序提供了一个简单的框架，可以用于对目录中的部件批量执行用户自定义的操作。
>
