### 【1977】report all tc contexts 报告所有TC上下文

#### 代码

```cpp
    /*HEAD REPORT_ALL_TC_CONTEXTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        logical  
            nxman;  
        int  
            count,  
            ii;  
        char  
            **contexts,  
            part_type[UF_UGMGR_FTYPE_SIZE+1] = { "Item" };  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (!nxman) return;  
        while (prompt_for_text("Enter part type to show contexts of", part_type))  
        {  
            WRITE_S(part_type);  
            count = -1;  
        /*  The function will not work with NULL part_type - see PR 1632362 里海译:该函数无法在 part_type 为 NULL 时工作，请参阅 PR 1632362。 */  
            UF_CALL(UF_UGMGR_list_contexts(part_type, &count, &contexts));  
        /*  If the count is still -1 and error occured - see PR 5865817 里海译:如果计数仍然为-1并且出现错误，请参阅PR 5865817。 */  
            WRITE_D(count);  
            for (ii = 0; ii < count; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_S(contexts[ii]);  
            }  
        /*  Do not use UF_free_string_array - see PR 1632365 里海译:不要使用UF_free_string_array，请参阅PR 1632365。 */  
            if (count > 0) UF_free(contexts);  
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

> 根据代码内容，这是段NX二次开发代码，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败的错误代码和消息。
> 2. 定义了整数和字符串写入列表窗口的宏WRITE_D和WRITE_S，用于在列表窗口中打印出变量的值。
> 3. 定义了一个获取用户输入文本的函数prompt_for_text，用于提示用户输入文本并获取。
> 4. 定义了一个函数do_it，用于获取指定部件类型的上下文列表，并打印出来。使用了UF_UGMGR_list_contexts函数获取上下文列表。
> 5. 在ufusr函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义了一个卸载函数ufusr_ask_unload，用于立即卸载该代码。
>
> 总体来说，这段代码主要用于演示如何获取指定部件类型的上下文列表，并在列表窗口中打印出来，是一个简单的NX二次开发示例。
>
