### 【1319】format and submit problem for nastran analysis 格式化并提交NASTRAN分析问题

#### 代码

```cpp
    /*HEAD FORMAT_AND_SUBMIT_PROBLEM_FOR_NASTRAN_ANALYSIS CCC UFUN */  
    /* This program requires a complete defined FEM model, and the program has to be   
    executed in UG/scenario or UG/structures while the scenario part is work part. Also   
    the UG environment variable UGII_NASTRAN needs to point to the bin directory of the   
    MSC install directory, and UGII_NASTRAN_VERSION needs to have as value the correct   
    version number.  
    However it has to be noted that user function UF_SF_analysis_nastran   
    with format_choice=0 does not work in V16, V17 untill V18.0.2.1, but works in V15   
    and V18.0.3.1 (refer to PR-4398655)*/  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数，用于打印系统日志。该函数的参数包括日志级别、模块名、日志信息，分别对应日志的严重程度、生成日志的模块名称以及实际的日志内容。在 V18 版本之前，并不存在这个函数。 */  
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
            format_choice =0,  
            num_roots = 1;  
        double  
            temperature = 70.0,  
            upper_limit = 100.0,  
            lower_limit = 10.0;   
        /* this may work in some older versions ===============================  
        UF_CALL(UF_SF_analysis_nastran("", format_choice, "",   
                temperature, upper_limit, lower_limit, num_roots));  
        =====================================================================*/  
        UF_CALL(UF_SF_analysis_nastran("", format_choice, temperature, upper_limit,  
                 lower_limit, num_roots));  
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

> 这段代码是一个NX的二次开发用户函数，主要功能是格式化并提交模型进行Nastran有限元分析。具体来说：
>
> 1. 包含了UF、UF_UI、UF_SF等头文件，这些是NX提供的二次开发API头文件。
> 2. 定义了report_error函数，用于在调用NX API时出错时报告错误信息。
> 3. 定义了do_it函数，其中调用UF_SF_analysis_nastran API，用于设置分析参数并提交模型进行Nastran分析。
> 4. 定义了ufusr主函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示用户函数在调用后可以被立即卸载。
> 6. 使用宏UF_CALL包装NX API调用，如果调用出错，会通过report_error函数报告错误。
> 7. 在do_it函数中，设置了Nastran分析的参数，如温度、上下限等，并调用UF_SF_analysis_nastran进行提交分析。
> 8. 注释中提到该代码在NX V15和V18.0.3.1版本下经过测试，而在V16、V17、V18.0.2.1版本下存在问题，需要注意。
> 9. 整个代码逻辑清晰，通过封装错误处理，提高了代码的健壮性。
>
