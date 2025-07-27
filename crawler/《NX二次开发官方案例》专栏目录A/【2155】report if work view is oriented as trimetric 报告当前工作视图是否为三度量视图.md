### 【2155】report if work view is oriented as trimetric 报告当前工作视图是否为三度量视图

#### 代码

```cpp
    /*HEAD REPORT_IF_WORK_VIEW_IS_ORIENTED_AS_TRIMETRIC CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_csys.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static void do_it(void)  
    {  
        double  
            wv_csys[9];  
        tag_t  
            tfr_mx,  
            tfr_view,  
            wv_mx;  
        UF_CALL(uc6433("", wv_csys));  
        UF_CALL(UF_CSYS_create_matrix(wv_csys, &wv_mx));  
        UF_CALL(UF_VIEW_ask_tag_of_view_name("TFR-TRI", &tfr_view));  
        UF_CALL(UF_CSYS_ask_matrix_of_object(tfr_view, &tfr_mx));  
        if (wv_mx == tfr_mx)  
            uc1601("Current view is oriented as TRF-TRI", TRUE);  
        else  
            uc1601("Current view is NOT oriented as TRF-TRI", TRUE);  
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

> 这段代码是NX Open的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了宏ECHO和UF_CALL，分别用于输出信息到列表窗口和打印日志，并在调用NX API时使用。
> 3. 定义了函数do_it，用于获取当前视图的坐标系，并与TRF-TRI视图坐标系进行比较，然后打印当前视图是否与TRF-TRI视图对齐。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数，在初始化NX系统后调用do_it函数，执行主要功能，并在结束时关闭NX系统。
> 5. 定义了ufusr_ask_unload函数，用于在卸载用户函数时立即卸载。
>
> 总体来说，这段代码实现了检查当前视图是否与TRF-TRI视图对齐的功能，通过比较坐标系来实现。它使用了NX的UF接口进行二次开发，利用UF系统函数进行错误处理和日志输出，并遵循了NX二次开发的基本流程。
>
