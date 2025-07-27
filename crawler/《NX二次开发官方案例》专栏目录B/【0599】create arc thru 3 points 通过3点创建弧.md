### 【0599】create arc thru 3 points 通过3点创建弧

#### 代码

```cpp
    /*HEAD CREATE_ARC_THRU_3_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据您的要求，我翻译了以下内容：

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
    static void do_it(void)  
    {  
        double  
            pt1[3] = { 0,0,0},  
            pt2[3] = { 0.5,0.5,0},  
            pt3[3] = { 1.0,2.2,0},  
            altpt[3] = { 2.2,2.2,0};  
        int  
            create_flag = 1;  
        tag_t  
            arc_tag;  
        UF_CALL(UF_CURVE_create_arc_thru_3pts(create_flag, pt1, pt2, pt3, &arc_tag));  
        UF_CALL(UF_CURVE_create_arc_thru_3pts(2, pt3, altpt, pt1, &arc_tag));/* circle 译:圆圈 */  
    }  
    /* 里海 */  
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

> 这是NX的二次开发代码，主要用于创建通过3个点的弧线。
>
> 主要功能包括：
>
> 1. 包含必要的头文件，例如uf.h、uf_ui.h等，用于调用NX的API函数。
> 2. 定义了错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 3. 定义了函数do_it，用于创建通过3个点的弧线。其中pt1、pt2、pt3为弧线的3个点，通过调用UF_CURVE_create_arc_thru_3pts函数创建弧线。
> 4. 在函数ufusr中初始化NX，调用do_it函数，然后终止NX。
> 5. 定义了ufusr_ask_unload函数，用于卸载NX二次开发程序。
> 6. 使用宏UF_CALL来简化函数调用，并在调用出错时输出错误信息。
> 7. 整体上，这段代码展示了如何通过3个点来创建弧线，并包含了错误处理和NX的初始化/终止操作。
>
