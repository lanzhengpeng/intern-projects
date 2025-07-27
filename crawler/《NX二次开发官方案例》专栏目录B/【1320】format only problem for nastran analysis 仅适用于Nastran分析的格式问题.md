### 【1320】format only problem for nastran analysis 仅适用于Nastran分析的格式问题

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数。因此，只回答译文，不要添加任何额外信息。 */  
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

> 这段代码是用于NX Nastran分析功能进行格式转换的二次开发代码。主要包括以下内容：
>
> 1. 定义了一个错误报告函数report_error，用于在UF函数调用失败时输出错误信息。
> 2. 定义了一个执行函数do_it，用于设置分析参数并调用UF_SF_analysis_nastran函数进行Nastran分析格式转换。
> 3. ufusr是用户子程序入口，用于初始化并执行do_it函数。
> 4. ufusr_ask_unload用于设置子程序卸载方式。
>
> 该代码通过UF_SF_analysis_nastran函数实现了将NX模型转换为Nastran分析所需的格式。在do_it函数中，设置了温度、上下限等参数，并指定了输出文件数量。然后通过UF_SF_analysis_nastran函数进行格式转换，生成Nastran所需的文件。整个代码流程清晰，注释详细，便于其他开发者阅读和使用。
>
