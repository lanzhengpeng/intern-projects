### 【0621】create bsurface thru points the old way 创建旧式通过点创建BSURFACE

#### 代码

```cpp
    /*HEAD CREATE_BSURFACE_THRU_POINTS_THE_OLD_WAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。因此，对于提问中的NOTE: UF_print_syslog is new in V18，我的回答是：UF_print_syslog是在V18版本中新增的函数。 */  
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
    #define NUM_ROWS 4  
    static void do_it(void)  
    {  
        int  
            irc,  
            num_rows = NUM_ROWS,  
            one = 1,  
            open = 0,  
            pts_per_row[NUM_ROWS] = {4, 4, 4, 4},  
            three = 3;  
        tag_t  
            bsurf;  
        double  
            points[16][3] = {  3.5, 2.5, 0.0,  
                               4.0, 2.6, 0.2,  
                               4.5, 2.6, 0.2,  
                               5.0, 2.5, 0.0,  
                               3.3, 3.0, 0.2,  
                               4.1, 3.2, 0.3,  
                               4.5, 3.2, 0.3,  
                               5.0, 3.0, 0.2,  
                               3.0, 3.5, 0.2,  
                               3.8, 3.6, 0.3,  
                               4.5, 3.6, 0.3,  
                               5.0, 3.5, 0.2,  
                               2.6, 4.0, 0.0,  
                               3.3, 4.0, 0.2,  
                               4.5, 4.0, 0.2,  
                               5.0, 4.0, 0.0 },  
            weights[16] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,  
                1.0, 1.0, 1.0, 1.0, 1.0 };  
        FTN(uf5448)(&one, &open, &open, &three, &three, &num_rows, pts_per_row,  
            (double *)points, weights, &bsurf, &irc);  
        ECHO(bsurf);  
        ECHO(irc);  
        UF_CALL(UF_VIEW_fit_view(NULL_TAG, .9));  
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

> 这段代码是一个NX Open C++ API的示例，实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于输出函数调用失败时的错误信息。
> 2. 定义了创建通过点的方式创建基本曲面(BSURFACE)的函数do_it。其中，定义了4行4列共16个点的坐标和权重，然后调用UF5448函数创建基本曲面。
> 3. 定义了ufusr函数，作为NX Open C++ API的入口函数。在该函数中，首先调用UF_initialize初始化API，然后调用do_it函数执行创建基本曲面的操作，最后调用UF_terminate终止API。
> 4. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示该例程不需要卸载操作。
> 5. 使用了#define定义了一些常量，如行数NUM_ROWS等。
> 6. 使用了UF_VIEW_fit_view函数调整视图，以更好地显示创建的曲面。
> 7. 注释中有中文，说明代码的某些部分的功能。
>
> 总体来说，这段代码演示了如何使用NX Open C++ API通过点阵创建基本曲面，并包含了错误处理、API初始化和视图调整等操作。
>
