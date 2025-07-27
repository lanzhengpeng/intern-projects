### 【2372】report sheet metal design general preferences 报告金属板设计通用首选项

#### 代码

```cpp
    /*HEAD REPORT_SHEET_METAL_DESIGN_GENERAL_PREFERENCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_smd.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        UF_SMD_gen_prefs_p_t  
            general_prefs;  
        if (UF_CALL(UF_SMD_initialize())) return;  
        UF_CALL(UF_SMD_ask_general_prefs(&general_prefs));  
        WRITE_D(general_prefs->seq_labels);  
        WRITE_D(general_prefs->use_feat_stds);  
        WRITE_D(general_prefs->chk_stds);  
        WRITE_S(general_prefs->default_baf);  
        if (general_prefs->default_baf != NULL)  
            UF_free(general_prefs->default_baf);  
        WRITE_S(general_prefs->material);  
        if (general_prefs->material != NULL)  
            UF_free(general_prefs->material);  
        WRITE_S(general_prefs->thickness);  
        if (general_prefs->thickness != NULL)  
            UF_free(general_prefs->thickness);  
        WRITE_D(general_prefs->line_color);  
        WRITE_D(general_prefs->enforce_cre_state);  
    /*  These fields are not filled in - see PR 1121810  
        WRITE_S(general_prefs->mat_margin);  
        if (general_prefs->mat_margin != NULL)  
            UF_free(general_prefs->mat_margin);  
        WRITE_D(general_prefs->use_bat);  
    */  
        WRITE_L(general_prefs->use_global_thickness);  
        WRITE_D(general_prefs->thickness_type);  
        WRITE_L(general_prefs->use_global_radius);  
        WRITE_S(general_prefs->global_radius);  
        if (general_prefs->global_radius != NULL)  
            UF_free(general_prefs->global_radius);  
        WRITE_D(general_prefs->radius_type);  
        WRITE_L(general_prefs->use_global_angle);  
        WRITE_S(general_prefs->global_angle);  
        if (general_prefs->global_angle != NULL)  
            UF_free(general_prefs->global_angle);  
        WRITE_D(general_prefs->angle_type);  
        WRITE_D(general_prefs->baf_option);  
        WRITE_L(general_prefs->use_global_baf);  
        UF_free(general_prefs);  
        UF_CALL(UF_SMD_terminate());  
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

> 这段代码是用于NX的二次开发，主要功能是获取NX钣金设计模块中的常规参数设置，并将其输出到日志窗口。以下是代码的主要功能和流程：
>
> 1. 头文件包含：引入了必要的NX二次开发库头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了report_error函数，用于在出错时输出错误信息。
> 3. 日志窗口写入函数：定义了write_integer_to_listing_window、write_string_to_listing_window等函数，用于将数据写入日志窗口。
> 4. 主体函数do_it：初始化NX钣金设计模块。获取常规参数设置。将参数设置输出到日志窗口，包括序列标签、默认材料、厚度、颜色等。释放内存。终止NX钣金设计模块。
> 5. 初始化NX钣金设计模块。
> 6. 获取常规参数设置。
> 7. 将参数设置输出到日志窗口，包括序列标签、默认材料、厚度、颜色等。
> 8. 释放内存。
> 9. 终止NX钣金设计模块。
> 10. ufusr函数：NX二次开发的入口函数，调用do_it函数以执行主体功能。
> 11. 卸载函数：返回立即卸载标志，表示该库在调用完成后可立即卸载。
>
> 综上，这段代码实现了获取NX钣金设计模块的常规参数设置并输出到日志窗口的功能，采用了NX二次开发库提供的接口。
>
