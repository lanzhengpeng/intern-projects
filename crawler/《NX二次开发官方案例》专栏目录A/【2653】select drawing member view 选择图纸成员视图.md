### 【2653】select drawing member view 选择图纸成员视图

#### 代码

```cpp
    /*HEAD SELECT_DRAWING_MEMBER_VIEW CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文内容，我翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
        double  
            rr2[3];  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        while (uc1615("Select member view", rr2) == 5)  
        {  
            uc1653(view_name);  
            uc1601(view_name, TRUE);  
        }  
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

> 这段代码是NX的二次开发代码，其主要功能是选择并高亮显示装配体中的组件视图。
>
> 主要步骤包括：
>
> 1. 包含NX的UF和UI库的头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了主函数do_it，其中：
>
> 1. 定义了ufusr函数，用于初始化NX并调用do_it函数，最后终止NX。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了在NX装配体中循环选择并高亮显示组件视图的功能，属于比较简单的二次开发示例。
>
