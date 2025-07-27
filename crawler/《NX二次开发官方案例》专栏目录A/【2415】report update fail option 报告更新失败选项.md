### 【2415】report update fail option 报告更新失败选项

#### 代码

```cpp
    /*HEAD REPORT_UPDATE_FAIL_OPTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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

注意：UF_print_syslog 是 V18 中的新功能，请只提供翻译，不要添加其他内容。

UF_print_syslog 是 V18 中的一个新功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        UF_MODL_update_option_t  
            current_option;  
    /*  This may not return the correct results - see PR 5169001 里海译:这可能不会返回正确的结果，请参考 PR 5169001。 */  
        UF_CALL(UF_MODL_ask_update_fail_option(&current_option));  
        WRITE("Update fail option: ");  
        switch (current_option)  
        {  
            case UF_MODL_UPDATE_NO_OPTION:  
                WRITE_D(UF_MODL_UPDATE_NO_OPTION);  
                break;  
            case UF_MODL_UPDATE_UNDO:  
                WRITE_D(UF_MODL_UPDATE_UNDO);  
                break;  
            case UF_MODL_UPDATE_SUPPRESS:  
                WRITE_D(UF_MODL_UPDATE_SUPPRESS);  
                break;  
            case UF_MODL_UPDATE_SUPPRESS_ALL:  
                WRITE_D(UF_MODL_UPDATE_SUPPRESS_ALL);  
                break;  
            case UF_MODL_UPDATE_ACCEPT:  
                WRITE_D(UF_MODL_UPDATE_ACCEPT);  
                break;  
            case UF_MODL_UPDATE_ACCEPT_ALL:  
                WRITE_D(UF_MODL_UPDATE_ACCEPT_ALL);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(current_option);  
                break;  
        }  
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

> 这段代码是一个NX Open Application的示例，用于查询和显示当前NX系统中的更新失败处理选项。
>
> 主要功能包括：
>
> 1. 使用UF_MODL_ask_update_fail_option函数查询当前更新失败处理选项，并将其存储在current_option变量中。
> 2. 通过WRITE宏将"Update fail option: "写入列表窗口。
> 3. 使用switch语句判断current_option的值，并使用WRITE_D宏将对应的枚举常量写入列表窗口。如果current_option的值未知，则写入"Unknown "和current_option的值。
> 4. 在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it函数执行上述查询和显示功能，最后调用UF_terminate终止。
> 5. 提供了一个ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载该应用程序。
> 6. 使用了UF_PRINT_SYSLOG宏在系统日志中打印错误信息，这是NX 18版本的新功能。
> 7. 定义了REPORT_ERROR宏，用于在发生UF函数调用错误时报告错误信息。
>
> 总体来说，这段代码简单实现了查询和显示NX系统更新失败处理选项的功能，并包含了错误处理和日志记录。
>
