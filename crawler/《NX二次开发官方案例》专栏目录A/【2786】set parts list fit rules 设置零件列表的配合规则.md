### 【2786】set parts list fit rules 设置零件列表的配合规则

#### 代码

```cpp
    /*HEAD SET_PARTS_LIST_FIT_RULES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plist.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释内容，翻译如下：

注意：UF_print_syslog 是 V18 新增的，只回答译文，不要废话。 */  
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
        int  
            fitRules[7] = {4, 0, 0, 0, 0, 0, 0};  
                       /*  4 = Shrink the character size 里海译:缩小字符大小 */  
        tag_t  
            partslistNote,  
            partslistTag = UF_PLIST_ask_tag();  
        double  
            orig[3] = { 0, 0, 0 };  
        UF_PLIST_set_fit_rules(partslistTag, fitRules);  
        UF_CALL(UF_PLIST_create_note(partslistTag, orig, &partslistNote));  
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

> 这段NX二次开发代码实现了设置部件列表拟合规则的功能。主要功能如下：
>
> 1. 包含必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h、uf_plist.h等，以支持代码中的函数调用。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个do_it函数，用于设置部件列表的拟合规则，并创建注释。具体包括：定义一个拟合规则数组fitRules，其中包含7个元素，分别表示不同的拟合方式。此处只设置了第1个元素为4，表示缩放字符大小。获取当前部件列表的tag。调用UF_PLIST_set_fit_rules函数设置部件列表的拟合规则。调用UF_PLIST_create_note函数在指定位置创建注释。
> 4. 定义一个拟合规则数组fitRules，其中包含7个元素，分别表示不同的拟合方式。此处只设置了第1个元素为4，表示缩放字符大小。
> 5. 获取当前部件列表的tag。
> 6. 调用UF_PLIST_set_fit_rules函数设置部件列表的拟合规则。
> 7. 调用UF_PLIST_create_note函数在指定位置创建注释。
> 8. 定义了ufusr函数，作为用户自定义函数，用于初始化NX并调用do_it函数。
> 9. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示立即卸载二次开发应用程序。
>
> 总体来说，这段代码通过二次开发实现了NX中部件列表的拟合规则设置功能，以及创建注释的功能。
>
