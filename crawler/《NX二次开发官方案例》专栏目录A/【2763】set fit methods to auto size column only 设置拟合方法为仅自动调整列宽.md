### 【2763】set fit methods to auto size column only 设置拟合方法为仅自动调整列宽

#### 代码

```cpp
    /*HEAD SET_FIT_METHODS_TO_AUTO_SIZE_COLUMN_ONLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_tabnot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog是在V18版本中新增的函数。因此，在回答问题时，我们只需翻译这个句子，无需添加任何其他内容。 */  
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
        UF_TABNOT_cell_prefs_t  
            cell_prefs;  
        UF_CALL(UF_TABNOT_ask_default_cell_prefs(&cell_prefs));  
        cell_prefs.nm_fit_methods = 1;  
        cell_prefs.fit_methods[0] = UF_TABNOT_fit_method_auto_size_col;  
        UF_CALL(UF_TABNOT_set_default_cell_prefs(&cell_prefs));  
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

> 这段代码是一个NX的二次开发示例，主要实现了设置默认单元格属性的功能。下面是代码的主要功能介绍：
>
> 1. 包含了必要的NX开发库头文件，包括UF、UF_UI和UF_TABNOT等。
> 2. 定义了一个错误报告函数report_error，用于在调用NX API函数出错时，打印错误信息到系统日志和消息窗口。
> 3. 定义了一个do_it函数，用于设置默认的单元格属性。在函数中，首先获取默认的单元格属性，然后将其中的nm_fit_methods设置为1，并将fit_methods[0]设置为UF_TABNOT_fit_method_auto_size_col，表示默认使用自动调整列宽的方式。
> 4. 定义了ufusr函数，是NX二次开发的标准入口函数。在函数中，首先调用UF_initialize初始化NX开发环境，然后调用do_it函数设置默认单元格属性，最后调用UF_terminate终止NX开发环境。
> 5. 定义了ufusr_ask_unload函数，用于在二次开发模块卸载时调用，这里直接返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 总的来说，这段代码通过二次开发的方式，实现了设置NX默认单元格属性的功能，使其默认使用自动调整列宽的方式。这段代码对NX的表格功能进行了扩展，增强了用户体验。
>
