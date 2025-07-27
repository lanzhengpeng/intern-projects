### 【2438】report where work part is used 报告中工作部件的使用

#### 代码

```cpp
    /*HEAD REPORT_WHERE_WORK_PART_IS_USED CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

UF_print_syslog 是 V18 版本新增的功能，用于打印系统日志信息。 */  
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
        int  
            irc;  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            comp_name[MAX_FSPEC_SIZE+1],  
            d_spec[MAX_FSPEC_SIZE+1],  
            f_spec[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        UF_CALL(uc4400(0,"",0));  
        FTN(uf4401)(&irc);  
        UF_PART_ask_part_name(part, f_spec);  
        UF_CALL(uc4576(f_spec, 2, d_spec, comp_name));  
        UF_CALL(UF_ASSEM_where_used_report(comp_name, d_spec, 2, TRUE, &status));  
        report_load_status(&status);  
    }  
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

> 这段NX二次开发代码的主要功能是查询当前工作部件在装配体中的使用情况，并将查询结果输出到系统日志和NX窗口中。具体来说：
>
> 1. 错误报告宏：定义了一个宏UF_CALL，用于调用NX API函数，并在发生错误时记录错误信息。
> 2. 错误处理函数：定义了一个函数report_error，用于在发生错误时输出错误信息到系统日志和NX窗口。
> 3. 加载状态报告函数：定义了一个函数report_load_status，用于输出部件的加载状态信息到NX窗口。
> 4. 主逻辑函数：定义了一个函数do_it，其中：获取当前工作部件获取部件的文件名查询该部件在装配体中的使用情况，并将结果存储在结构体status中调用report_load_status函数输出查询结果
> 5. 获取当前工作部件
> 6. 获取部件的文件名
> 7. 查询该部件在装配体中的使用情况，并将结果存储在结构体status中
> 8. 调用report_load_status函数输出查询结果
> 9. NX入口函数：定义了一个函数ufusr，是NX二次开发的入口函数。在函数中初始化NX环境，调用do_it函数执行主逻辑，然后终止NX环境。
> 10. 卸载函数：定义了一个函数ufusr_ask_unload，用于在二次开发模块卸载时立即卸载。
>
> 总的来说，这段代码实现了查询当前工作部件在装配体中的使用情况的功能，并通过日志和窗口输出查询结果。
>
