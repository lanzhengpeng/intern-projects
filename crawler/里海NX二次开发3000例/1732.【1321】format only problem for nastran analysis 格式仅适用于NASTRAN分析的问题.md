### 【1321】format only problem for nastran analysis 格式仅适用于NASTRAN分析的问题

#### 代码

```cpp
    /*HEAD FORMAT_ONLY_PROBLEM_FOR_NASTRAN_ANALYSIS CCC UFUN */  
    /* This program requires a complete defined FEM model, and the program has to   
    be executed in UG/scenario or UG/structures while the scenario part is work part.*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于在Caché中打印系统日志信息。 */  
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
        char  
            job_title[] = {""},  
            loadcase_name[] = {""};  
        int  
            format_choice =1,  
            num_roots = 1;  
        double  
            temperature = 70.0,  
            upper_limit = 100.0,  
            lower_limit = 10.0;   
        /* this may work in some older versions ===============================  
        UF_CALL(UF_SF_analysis_nastran("", format_choice, "",   
                temperature, upper_limit, lower_limit, num_roots));  
        =====================================================================*/  
        UF_CALL(UF_SF_analysis_nastran("", format_choice,    
                temperature, upper_limit, lower_limit, num_roots));  
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

> 这段NX二次开发代码主要用于执行Nastran分析，其中包含以下关键部分：
>
> 1. 头文件包含：引入了标准C库头文件，以及NX的UF、UF_UI和UF_SF头文件，用于进行NX API调用。
> 2. 错误处理函数：report_error函数用于在出错时打印错误信息，包括文件名、行号、函数名和错误码，以便于调试。
> 3. 执行函数：do_it函数中定义了Nastran分析所需的参数，如温度、上下限等，并调用了UF_SF_analysis_nastran函数执行Nastran分析。
> 4. 主函数：ufusr函数是NX二次开发的主入口，在函数中初始化了NX环境，执行了do_it函数，然后终止了NX环境。
> 5. 卸载函数：ufusr_ask_unload函数返回立即卸载，表示在NX中执行完该代码后立即卸载。
> 6. 代码注释：作者在代码中添加了中文注释，以便其他开发者阅读和理解代码。
> 7. 代码版本：作者在注释中指出了代码适用于NX的某些旧版本，而在新版本中需要调整函数调用。
> 8. 代码格式：代码遵循良好的编程规范，如缩进、注释等，有助于提高代码的可读性和可维护性。
>
> 综上所述，这段代码实现了在NX中执行Nastran分析的功能，作者在代码结构和注释方面做了精心设计，使得代码清晰易读，方便其他开发者使用和修改。
>
