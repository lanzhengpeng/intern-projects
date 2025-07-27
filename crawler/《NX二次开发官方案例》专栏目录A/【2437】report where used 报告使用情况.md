### 【2437】report where used 报告使用情况

#### 代码

```cpp
    /*HEAD REPORT_WHERE_USED CCC UFUN */  
    /*  Sample external program to generate a Where Used report  
        call it with 3 arguments as follows:  
            1st arg = component part name  
            2nd arg = directory to search  
            3rd arg = output file name  
        e.g.)  where_used comp.prt /users/mydir where_used.lis  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是在 V18 版本中新增的，只回答译文，不要废话。

Note: UF_print_syslog is new in V18, just answer the translation, no nonsense. */  
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
        if (!UF_CALL(UF_initialize()))  
        {  
            int  
                ii = 0;  
            UF_PART_load_status_t  
                status;  
            /*  Initialize and open output file as listing device 里海译:初始化并打开输出文件作为列表设备。 */  
            UF_CALL(uc4400(4, argv[3], ii));  
            FTN(uf4401)(&ii);  
            /*  Generate Where Used report 里海译:根据上下文，"Generate Where Used report" 的中文翻译是：生成使用报告。 */  
            UF_CALL(UF_ASSEM_where_used_report(argv[1], argv[2], 2, TRUE, &status));  
            report_load_status(&status);  
            /*  Close listing device  里海译:设备下架 */  
            FTN(uf4402)();  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，其主要功能是生成一个“Where Used”报告，用于展示指定组件在装配体中的使用情况。代码的基本逻辑如下：
>
> 1. 初始化：通过调用UF_initialize()函数初始化NX环境。
> 2. 输出文件设置：调用uc4400()函数初始化输出文件，将其设置为列表设备。
> 3. Where Used报告生成：调用UF_ASSEM_where_used_report()函数生成Where Used报告。函数的参数指定了要查询的组件名、搜索目录、输出类型等。
> 4. 报告加载状态输出：调用report_load_status()函数输出报告的加载状态，包括加载成功或失败的组件及其原因。
> 5. 关闭列表设备：调用uf4402()函数关闭输出文件。
> 6. 环境终止：通过调用UF_terminate()函数终止NX环境。
>
> 此外，代码还定义了report_error()函数用于错误报告，以及一些宏用于简化函数调用。整体而言，这段代码通过调用NX的API函数，实现了查询组件使用情况的二次开发功能。
>
