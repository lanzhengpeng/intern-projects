### 【2840】set wcs to current view 将当前视图设置为WCS

#### 代码

```cpp
    /*HEAD SET_WCS_TO_CURRENT_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的功能。 */  
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
    static void set_wcs_to_current_view(void)  
    {  
        tag_t  
            matobj,                     /* Matrix object 里海译:矩阵对象 */  
            csys;                       /* Csys object  里海译:根据上下文，Csys object 的翻译为“Csys对象”。 */  
        double  
            origin[3],                  /* View's center 里海译:View's center的翻译为“视图的中心”。 */  
            matrix[9],                  /* View's matrix 里海译:视图的矩阵 */  
            scale;  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        if (!UF_CALL(UF_CSYS_create_matrix(matrix, &matobj)) &&  
            !UF_CALL(UF_CSYS_create_temp_csys(origin, matobj, &csys)))  
            UF_CALL(UF_CSYS_set_wcs(csys));  
    }  
    static void do_it(void)  
    {  
        set_wcs_to_current_view();  
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

> 根据代码内容，这是一段NX的二次开发代码，其主要功能是将当前的视图设置为WCS坐标系。具体来说，该代码包括以下部分：
>
> 1. 包含头文件：包含NX二次开发所需的头文件。
> 2. 报错函数：定义了report_error函数，用于在出错时打印错误信息。
> 3. 设置视图为WCS的函数：定义了set_wcs_to_current_view函数，通过获取当前视图的中心和矩阵，并创建临时坐标系，最后将其设置为WCS。
> 4. 主函数：定义了do_it函数，其中调用了set_wcs_to_current_view函数。
> 5. 用户函数：定义了ufusr函数，用于在NX中初始化、调用主函数，并终止。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载时返回立即卸载的标志。
>
> 总体来看，该代码通过获取当前视图的中心和矩阵，创建临时坐标系，并将其设置为WCS，实现了将当前视图设置为WCS坐标系的功能。
>
