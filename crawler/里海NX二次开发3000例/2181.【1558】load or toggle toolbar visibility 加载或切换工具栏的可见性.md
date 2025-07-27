### 【1558】load or toggle toolbar visibility 加载或切换工具栏的可见性

#### 代码

```cpp
    /*HEAD LOAD_OR_TOGGLE_TOOLBAR_VISIBILITY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能。 */  
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
    static UF_UI_toolbar_id_t MY_toolbar_id = NULL;  
    static void do_it(void)  
    {  
        int  
            show;  
        if (MY_toolbar_id == NULL)  
        {  
            printf("Loading toolbar\n");  
            UF_CALL(UF_UI_create_toolbar("my.tbr", UF_UI_SHOW, &MY_toolbar_id));  
        /*  Specifically SHOW the tool in case is was hidden in the registry 里海译:具体地展示一下工具，以防它在注册表中隐藏了。 */  
            UF_CALL(UF_UI_set_toolbar_vis(MY_toolbar_id, UF_UI_SHOW));  
        }  
        else  
        {  
            printf("Toggling toolbar visibility\n");  
            UF_CALL(UF_UI_ask_toolbar_vis(MY_toolbar_id, &show));  
            UF_CALL(UF_UI_set_toolbar_vis(MY_toolbar_id, (1 - show)));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX Open二次开发代码实现了加载或切换工具条可见性的功能。主要功能如下：
>
> 1. 定义了错误报告函数report_error，用于在UF函数调用失败时输出错误信息。
> 2. 定义了全局工具条ID变量MY_toolbar_id，用于存储工具条的唯一标识。
> 3. 定义了函数do_it，用于实现加载或切换工具条可见性的功能。
> 4. 如果工具条尚未加载，使用UF_UI_create_toolbar创建一个名为"my.tbr"的工具条，并设置其可见性为UF_UI_SHOW。
> 5. 如果工具条已加载，使用UF_UI_ask_toolbar_vis获取当前工具条的可见性，然后使用UF_UI_set_toolbar_vis切换工具条的可见性。
> 6. 在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it实现功能，最后调用UF_terminate终止UF系统。
> 7. 通过UF_print_syslog输出信息到系统日志，以便调试。
> 8. 通过UF_UI_open_listing_window和UF_UI_write_listing_window在列表窗口中输出信息，以便用户查看。
>
> 这段代码实现了简单的工具条加载和切换功能，适用于NX Open二次开发环境。
>
