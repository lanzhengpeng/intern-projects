### 【3033】update assembly structure 更新装配结构

#### 代码

```cpp
    /*HEAD UPDATE_ASSEMBLY_STRUCTURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_fltr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_FLTR_update_structure(part));  
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

> 这段代码是NX的二次开发代码，主要用于更新零件结构。以下是代码的详细解释：
>
> 1. 包含头文件：代码包含了必要的NX API头文件，如uf.h、uf_ui.h、uf_part.h等。
> 2. 错误报告函数：定义了report_error函数，用于在函数调用失败时报告错误信息。
> 3. 更新结构函数：定义了do_it函数，用于获取当前显示的零件并调用UF_FLTR_update_structure函数更新其结构。
> 4. ufusr入口函数：这是NX二次开发的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载二次开发的值。
> 6. 错误处理：使用宏UF_CALL封装函数调用，并在调用失败时通过report_error函数报告错误。
> 7. 打印错误日志：使用UF_print_syslog函数打印错误日志到NX日志窗口，这是NX 18版本新增的功能。
> 8. 用户界面：使用UF_UI系列函数操作NX用户界面，如打开列表窗口、写入信息等。
>
> 总的来说，这段代码实现了NX二次开发中更新零件结构的功能，并对错误进行了处理和报告。
>
