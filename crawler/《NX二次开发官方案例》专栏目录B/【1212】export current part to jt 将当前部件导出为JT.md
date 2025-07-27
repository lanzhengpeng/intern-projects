### 【1212】export current part to jt 将当前部件导出为JT

#### 代码

```cpp
    /*HEAD EXPORT_CURRENT_PART_TO_JT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中的新功能。 */  
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
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            *base_dir,  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            prog_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            pv_dir[UF_CFI_MAX_PATH_NAME_SIZE];  
    /*  The translator will pick up the part as last saved, so save it now. 里海译:翻译如下：

翻译器将接上次保存的部分，所以现在保存它。 */  
        UF_CALL(UF_PART_save());  
        UF_PART_ask_part_name(part, partname);  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "pvtrans", pv_dir));  
        UF_CALL(uc4575(pv_dir, 0, "ugtopv", prog_name));  
        sprintf(cmd, "%s %s", prog_name, partname);  
        system(cmd);  
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

> 这段代码是一个NX Open C++应用程序，用于将当前部件导出到JT文件。
>
> 主要功能包括：
>
> 1. 使用UF_PART_save()函数保存当前部件。
> 2. 获取当前部件的名称。
> 3. 获取UGII_BASE_DIR环境变量，从中得到pvtrans目录的路径。
> 4. 在pvtrans目录中找到ugtopv可执行文件的路径。
> 5. 构造命令行，将当前部件的名称作为参数，调用ugtopv程序。
> 6. 使用system()函数执行该命令行，将当前部件导出到JT文件。
> 7. 在出错时，使用UF_print_syslog()和UF_UI_write_listing_window()函数打印错误信息。
> 8. 提供ufusr()和ufusr_ask_unload()函数作为程序入口和卸载函数。
>
> 该程序通过NX Open C++ API调用UG/NX的内部函数，实现了将当前部件导出到JT文件的功能。代码结构清晰，功能明确，是一个典型的NX二次开发应用程序。
>
