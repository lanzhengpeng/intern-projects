### 【2480】report smd flat pattern preferences 报告SMD平板图案偏好

#### 代码

```cpp
    /*HEAD REPORT_SMD_FLAT_PATTERN_PREFERENCES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog在V18版本中是新增的。请只回答翻译，不要添加无关内容。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        UF_SMD_fp_prefs_p_t    
            fp_prefs;  
        if (UF_CALL(UF_SMD_initialize())) return;  
        UF_CALL(UF_SMD_ask_fp_prefs ( &fp_prefs ));  
        WRITE_F(fp_prefs->angle_tol);  
        WRITE_F(fp_prefs->chord_tol);  
        WRITE_F(fp_prefs->grid_size);  
        WRITE_D(fp_prefs->layer);  
        WRITE_D(fp_prefs->distortion_constraint);  
        WRITE_D(fp_prefs->fp_orientation);  
        WRITE_D(fp_prefs->curve_set);  
        WRITE_L(fp_prefs->tangent_lines);  
        WRITE_L(fp_prefs->center_lines);  
        WRITE_L(fp_prefs->form_block_curves);  
        WRITE_L(fp_prefs->contour_curves);  
        WRITE_L(fp_prefs->multiple_fp);  
        WRITE_L(fp_prefs->auto_update);  
        WRITE_L(fp_prefs->use_baf);  
        WRITE_S(fp_prefs->name);  
        WRITE_D(fp_prefs->fp_algorithm);  
        /* -----------------------------------------------------  
        you could make changes to the structure elements  
        and then call the set function:  
        fp_prefs->multiple_fp = TRUE;  
        UF_CALL(UF_SMD_set_fp_prefs ( fp_prefs ));  
         ----------------------------------------------*/  
        UF_free(fp_prefs);  
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

> 这段代码是一个用于读取和报告NX中SMD平展模式的参数设置，并可以通过修改这些参数来更新SMD平展模式参数的NX Open C++程序。
>
> 主要功能包括：
>
> 1. 定义了UF_CALL宏，用于执行UF函数并报告错误。
> 2. 定义了WRITE系列宏，用于向列表窗口输出各种类型的数据。
> 3. 定义了do_it函数，用于执行以下操作：初始化NX Open SMD模块读取当前的SMD平展模式参数设置输出参数设置到列表窗口可以修改参数设置，并调用函数更新SMD平展模式参数释放参数设置结构体内存终止NX Open SMD模块
> 4. 初始化NX Open SMD模块
> 5. 读取当前的SMD平展模式参数设置
> 6. 输出参数设置到列表窗口
> 7. 可以修改参数设置，并调用函数更新SMD平展模式参数
> 8. 释放参数设置结构体内存
> 9. 终止NX Open SMD模块
> 10. 定义了ufusr函数，用于在NX中加载和运行该程序。
> 11. 定义了ufusr_ask_unload函数，用于在程序卸载时立即卸载。
>
> 整体来看，这段代码提供了读取、报告和修改NX中SMD平展模式参数设置的二次开发功能。
>
