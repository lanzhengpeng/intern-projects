### 【0818】create parts list note 创建零件列表注释

#### 代码

```cpp
    /*HEAD CREATE_PARTS_LIST_NOTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plist.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，关于UF_print_syslog的翻译如下：

UF_print_syslog是在V18版本中新增的。 */  
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
            a_note,         
            pl_tag = UF_PLIST_ask_tag();  
        double  
            pos[3] = { 1, 1, 0};  
        UF_CALL(UF_PLIST_create_note(pl_tag, pos, &a_note));  
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

> 这段代码是NX二次开发中的用户函数，用于在零件列表中创建一个注释节点。具体功能如下：
>
> 1. 包含必要的NX头文件，如uf.h, uf_ui.h等，以便调用NX提供的函数。
> 2. 定义了一个报告错误的函数report_error，用于在调用NX函数出错时输出错误信息。
> 3. 定义了一个执行创建注释节点操作的函数do_it。在该函数中，首先获取当前零件列表的tag，然后调用UF_PLIST_create_note函数在零件列表中创建一个注释节点，并指定位置为(1,1,0)。
> 4. 定义了用户函数ufusr，在NX启动时会被调用。在ufusr中，首先调用UF_initialize进行初始化，然后调用do_it函数执行创建注释节点的操作，最后调用UF_terminate终止。
> 5. 定义了卸载用户函数ufusr_ask_unload，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载该用户函数。
>
> 总的来说，这段代码实现了在NX零件列表中创建一个注释节点的功能，并在出错时能够打印出错误信息。
>
