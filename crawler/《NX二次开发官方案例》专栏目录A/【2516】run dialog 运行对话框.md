### 【2516】run dialog 运行对话框

#### 代码

```cpp
    /*HEAD RUN_DIALOG CCC UFUN */  
    #include <stdlib.h>  
    #include <stdio.h>  
    #include <Xm/PushB.h>  
    #include <Xm/DialogS.h>  
    #include <uf.h>  
    #include <uf_ui_xt.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解UF_print_syslog是在V18版本中新增的函数。因此，我的翻译是：

UF_print_syslog是V18版本新增的函数。 */  
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
    static void exCB(Widget w, XtPointer fd, XtPointer call_data)  
    {  
      UF_CALL(UF_UI_exit_dialog());  
    }  
    static void do_it(void)  
    {  
      Widget  toplevel, ds, pb;  
      toplevel = UF_UI_get_default_parent();  
      ds = XmCreateDialogShell( toplevel, "Hey!", NULL, 0);  
      pb = XmCreatePushButton( ds, "Push This", NULL, 0 );  
      XtManageChild(pb);  
      XtAddCallback( pb, XmNactivateCallback, exCB, NULL );  
      UF_CALL(UF_UI_run_dialog(ds));  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段NX二次开发代码，实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息到系统日志和控制台。
> 2. 定义了一个按钮回调函数exCB，当按钮被点击时，退出对话框。
> 3. 定义了一个主函数do_it，创建一个对话框窗口，包含一个按钮，点击按钮会调用exCB回调函数退出对话框。
> 4. 定义了ufusr函数，首先调用UF_initialize()初始化NX环境，然后调用do_it()执行主逻辑，最后调用UF_terminate()关闭NX环境。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示在NX进程退出时会立即卸载该UF。
>
> 总的来说，这段代码实现了一个简单的NX对话框，包含一个按钮，点击按钮会退出对话框。
>
