### 【0463】change view in layout 改变布局中的视图

#### 代码

```cpp
    /*HEAD CHANGE_VIEW_IN_LAYOUT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layout.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        char  
            work_view_name[MAX_ENTITY_NAME_SIZE+1] = "TFR-ISO";  
        while (prompt_for_text("Enter View Name", work_view_name))  
        {  
            UF_CALL(uc6464("", "", work_view_name));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是用于NX Open的二次开发示例代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个提示输入文本的函数prompt_for_text，用于在NX界面提示用户输入文本，并获取用户输入。
> 3. 定义了一个主函数do_it，用于循环提示用户输入视图名称，然后调用函数uc6464切换到该视图。如果用户没有输入视图名称，则退出循环。
> 4. 在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it函数实现主功能，最后调用UF_terminate进行终止。
> 5. 定义了一个询问卸载函数ufusr_ask_unload，返回立即卸载的标志。
>
> 整体来看，这段代码实现了一个简单的NX视图切换功能，用户可以通过输入视图名称来切换到不同的视图。
>
