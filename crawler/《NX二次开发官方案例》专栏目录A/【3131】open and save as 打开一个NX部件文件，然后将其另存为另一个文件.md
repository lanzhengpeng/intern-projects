### 【3131】open and save as 打开一个NX部件文件，然后将其另存为另一个文件

#### 代码

```cpp
    /*HEAD OPEN_AND_SAVE_AS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本新增的函数。 */  
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
    /* qq3123197280 */  
    int main( int argc, char *argv[] )  
    {  
        tag_t  
            part;  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            printf("\nOpening %s...\n", argv[1]);  
            UF_CALL(UF_PART_open(argv[1], &part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            if (!status.failed)  
            {  
                printf("\nSaving as %s...\n", argv[2]);  
                UF_CALL(UF_PART_save_as(argv[2]));  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中打开和保存部件的二次开发程序。
>
> 主要功能包括：
>
> 1. 错误报告：定义了一个错误报告函数report_error，用于在调用UF函数失败时输出错误信息。
> 2. 加载状态报告：定义了一个加载状态报告函数report_load_status，用于打印打开部件时的状态信息。
> 3. 程序入口：在main函数中，首先调用UF_initialize初始化NX。然后，打开指定路径的部件，并检查打开状态，若打开失败则打印失败信息。如果打开成功，则保存部件到指定路径。
> 4. 使用UF函数：主要使用UF_PART_open打开部件，使用UF_PART_save_as保存部件，并使用UF_terminate终止NX。
> 5. 参数输入：通过命令行参数输入要打开的部件路径和保存的部件路径。
> 6. 容错处理：在打开和保存部件时进行了容错处理，打印错误信息。
> 7. 内存释放：在打开部件后，释放了相关内存。
>
> 综上所述，该代码实现了在NX中打开和另存部件的基本功能，具有错误报告和容错处理，是一段典型的NX二次开发代码。
>
