### 【2714】select work view 选择工作视图

#### 代码

```cpp
    /*HEAD SELECT_WORK_VIEW CCC UFUN */  
    /* This program changes the work view in a mult-view layout 里海译:该程序改变了多视图布局中的工作视图。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

注意：UF_print_syslog是V18版本新增的功能，请仅提供翻译，无需添加任何评论。 */  
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
    static logical select_a_view( char *prompt, char *view_name)  
    {  
        int  
            ret_code,  
            view_of_cursor;  
        double  
            pos[3];  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        ret_code = uc1615(prompt, pos);  
        UF_CALL( UF_UI_set_cursor_view( view_of_cursor));  
        if (ret_code == 5)  
        {  
            UF_CALL( uc1653( view_name ));  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }  
    static void do_it(void)  
    {  
        char  
            work_view_name[MAX_ENTITY_NAME_SIZE+1] = "TFR-ISO";  
        while (select_a_view("Select Work View", work_view_name))  
        {  
            UF_CALL(uc6449(work_view_name));  
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

> 根据代码内容，这是一段用于NX 12版本或更早版本的二次开发代码，其功能是改变多视图布局中的工作视图。主要内容包括：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了错误报告函数report_error，用于在出错时输出错误信息。
> 3. 定义了选择视图函数select_a_view，用于提示用户选择工作视图，并获取视图名称。
> 4. 定义了执行函数do_it，循环调用select_a_view获取视图名称，然后调用函数uc6449激活对应的工作视图。
> 5. 定义了ufusr函数作为主入口，初始化NX环境后调用do_it执行主要逻辑，并在结束时终止NX环境。
> 6. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
> 7. 整体逻辑是通过循环提示用户选择视图，并激活用户选择的工作视图。
>
> 代码的主要功能是交互式改变多视图布局中的工作视图，以满足用户的特定需求。
>
