### 【2792】set search dirs 设置搜索目录

#### 代码

```cpp
    /*HEAD SET_SEARCH_DIRS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的函数。翻译为：“UF_print_syslog 是在 V18 版本中新增加的。” */  
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
    static void do_it(void)  
    {  
        UF_ASSEM_options_t assem_options;  
        UF_PART_load_status_t   status;  
        tag_t   part;  
        int     count = 2;  
        logical     sub_dir[] = { FALSE, FALSE };  
        char    *dir_list[] = { "/users/stauder/parts", "/tmp/parts" };  
        UF_CALL(UF_ASSEM_ask_assem_options(&assem_options));  
        assem_options.load_options = UF_ASSEM_load_from_search_dirs;  
        UF_CALL(UF_ASSEM_set_assem_options(&assem_options));  
        UF_CALL( UF_ASSEM_set_search_directories( count,  
                 dir_list, sub_dir));  
        if (UF_CALL( UF_PART_open("/users/stauder/parts/large_assem",  
                 &part, &status) || status.n_parts > 0))  
            report_load_status(&status);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于设置NX的装配搜索路径，并尝试加载一个装配文件。
>
> 1. 函数和头文件：代码中包含了标准C语言头文件、NX Open C++的UF、UF_UI和UF_ASSEM头文件，以及NX Open C++错误处理宏。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 装配加载状态报告函数：定义了一个装配加载状态报告函数report_load_status，用于打印装配加载状态信息。
> 4. 设置装配搜索路径：do_it函数首先获取当前的装配选项，然后设置装配搜索路径为指定的两个目录，并设置为不搜索子目录。
> 5. 尝试加载装配文件：尝试加载指定路径的装配文件，如果加载失败，则调用report_load_status函数打印错误信息。
> 6. ufusr函数：ufusr是NX Open C++程序的入口点，首先初始化NX环境，然后调用do_it函数执行上述功能，最后终止NX环境。
> 7. 卸载请求函数：ufusr_ask_unload函数返回立即卸载标志，表示程序结束后立即卸载NX Open C++库。
>
> 综上所述，这段代码实现了设置NX装配搜索路径并加载指定装配文件的功能，通过错误处理和状态报告提供了完整的调试信息。
>
