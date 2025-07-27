### 【1568】loop to specify points 循环指定点

#### 代码

```cpp
    /*HEAD LOOP_TO_SPECIFY_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #define ECHO3F(X)  (printf("%s = %f, %f, %f\n", #X, *X,*(X+1),*(X+2)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，以下是关键信息的译文：

UF_print_syslog 是 V18 中的新增功能。 */  
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
            ii = 0,  
            ia2[2] = { 4, 0 },  
            irc = 5;  
        char  
            prompt[41];  
        double  
            loc[3];  
        do  
        {  
            ii++;  
            sprintf(prompt, "Indicate point %d", ii);  
            irc = uc1616(prompt, ia2, 0, loc);  
            if (irc == 5) ECHO3F(loc);  
        } while (irc == 5);  
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

> 这是NX Open C API编写的NX二次开发代码。主要功能是使用对话框循环提示用户指定点，直到用户取消。具体步骤如下：
>
> 1. 包含必要的头文件，定义了宏ECHO3F用于打印点坐标，宏UF_CALL用于检查UF函数调用是否成功，并报告错误。
> 2. 定义了report_error函数，用于报告UF函数调用失败时的错误信息。
> 3. 定义了do_it函数，循环调用uc1616函数提示用户指定点，直到用户取消。如果成功指定点，则使用ECHO3F打印点坐标。
> 4. 定义了ufusr函数，是NX二次开发的入口函数。初始化NX系统，调用do_it函数，然后终止NX系统。
> 5. 定义了ufusr_ask_unload函数，用于设置NX二次开发程序的卸载方式为立即卸载。
> 6. 使用UF_print_syslog函数将错误信息打印到NX日志文件中，这是NX18版本新增的功能。
> 7. 整个代码实现了简单的循环提示指定点功能，使用了NX提供的UF函数接口，并且包含了错误处理和日志打印。
>
