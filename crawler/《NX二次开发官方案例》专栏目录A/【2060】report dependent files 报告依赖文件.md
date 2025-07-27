### 【2060】report dependent files 报告依赖文件

#### 代码

```cpp
    /*HEAD REPORT_DEPENDENT_FILES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_assem.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，可以总结如下：

- UF_print_syslog 是在 V18 版本中新增的函数。
- 目前尚不清楚 UF_print_syslog 的具体功能，因为它是一个新函数。
- UF_print_syslog 可能与打印系统日志有关，但具体用途和参数等信息尚未提供。
- 在 V18 之前的版本中，并不存在 UF_print_syslog 函数。

综上所述，对于 UF_print_syslog 函数，目前只能确认它是 V18 版本新增的，具体用途尚待进一步了解。 */  
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
        int  
            file_count,  
            ii;  
        char  
            part_fspec[MAX_FSPEC_SIZE+1],  
            **file_names,   
            msg[MAX_LINE_SIZE+1];  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        /* UF_PART_ask_part_name returns an encoded name in ugmanger 里海译:返回在ugmanager中编码的名称 */  
        UF_CALL(UF_UGMGR_ask_dependent_files(part_fspec, &file_count, &file_names));  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s\n", "Dependent files...");  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for (ii = 0; ii < file_count; ii++)  
        {  
            sprintf(msg, "\t%s\n", file_names[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        UF_free_string_array(file_count, file_names);      
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

> 这段NX二次开发代码主要实现了查询当前工作部件的依赖文件并打印输出的功能。
>
> 具体来说，代码首先包含了NX二次开发所需的相关头文件，并定义了宏ECHO和UF_CALL用于打印变量值和执行NX函数。同时定义了report_error函数，用于在NX函数调用失败时打印错误信息。
>
> 在do_it函数中，首先获取当前工作部件，然后通过UF_PART_ask_part_name获取部件的文件路径，并通过UF_UGMGR_ask_dependent_files获取该部件的依赖文件列表。最后，通过UF_UI_open_listing_window和UF_UI_write_listing_window将依赖文件列表打印到NX的输出窗口。
>
> 在ufusr函数中，先通过UF_initialize初始化NX环境，然后调用do_it函数实现主体功能，最后通过UF_terminate终止NX环境。
>
> 此外，还定义了ufusr_ask_unload函数，用于卸载NX二次开发程序。
>
> 总的来说，这段代码实现了查询当前工作部件的依赖文件并打印输出的功能，具有较好的NX二次开发示例代码。
>
