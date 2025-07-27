### 【0846】create points 创建点

#### 代码

```cpp
    /*HEAD CREATE_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新功能，用于在日志中打印系统日志信息。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        double  
            pos[3];  
        tag_t  
            point;  
        while (specify_point("Indicate point location", pos))  
            UF_CALL(UF_CURVE_create_point(pos, &point));  
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

> 这段代码是一个NX Open C++应用程序，用于创建点。以下是代码的详细解释：
>
> 1. 包含头文件：代码包含NX Open C++库的头文件，用于与NX系统进行交互。
> 2. 错误报告函数：定义了一个名为report_error的函数，用于报告运行时错误，包括错误代码、错误消息和出错的函数调用。
> 3. 获取点位置函数：定义了一个名为specify_point的函数，用于提示用户指定点的位置，并返回一个布尔值表示是否成功获取位置。
> 4. 创建点函数：定义了一个名为do_it的函数，用于不断提示用户指定点位置，并在成功获取位置后调用NX函数UF_CURVE_create_point来创建点。
> 5. 主函数：定义了一个名为ufusr的主函数，这是NX Open应用程序的入口点。在初始化NX系统后，调用do_it函数，然后终止NX系统。
> 6. 卸载函数：定义了一个名为ufusr_ask_unload的函数，用于指定应用程序卸载时的行为。
>
> 综上所述，这段代码通过不断提示用户指定点位置，并在获取位置后调用NX函数来创建点。
>
