### 【0487】cleanup highlighting and groups 清理高亮和组

#### 代码

```cpp
    /*HEAD CLEANUP_HIGHLIGHTING_AND_GROUPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。 */  
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
        UF_CALL(UF_PART_cleanup(  
            UF_PART_cleanup_highlight | UF_PART_cleanup_all_groups ));  
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

> 这段代码是用于NX CAD软件的二次开发代码，主要功能是清理零件的突出显示和高亮显示。具体来说：
>
> 1. 引入了NX二次开发常用的头文件，如uf.h, uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了清理函数do_it，调用了UF_PART_cleanup函数，传入参数UF_PART_cleanup_highlight | UF_PART_cleanup_all_groups，表示清理突出显示和高亮显示。
> 4. 定义了主函数ufusr，初始化NX环境后调用do_it函数进行清理操作，然后终止NX环境。
> 5. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码的主要作用是清理NX零件的突出显示和高亮显示，属于NX二次开发的基础功能实现。
>
