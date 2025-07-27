### 【1666】move wcs to current view 移动WCS到当前视图

#### 代码

```cpp
    /*HEAD MOVE_WCS_TO_CURRENT_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数。 */  
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
        tag_t  
            matobj,                     /* Matrix object 里海译:矩阵对象（Matrix object） */  
            csys;                       /* Csys object  里海译:Csys object是坐标系对象。 */  
        double  
            origin[3],                  /* View's center 里海译:View的中心 */  
            matrix[9],                  /* View's matrix 里海译:视图的矩阵 */  
            scale;  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        if (!UF_CALL( UF_CSYS_create_matrix(matrix, &matobj) ) &&  
            !UF_CALL( UF_CSYS_create_temp_csys(origin, matobj, &csys) ))  
            UF_CALL( UF_CSYS_set_wcs(csys) );  
        UF_DISP_refresh();  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了将当前视图坐标系(WCS)转换到当前视图的功能。代码逻辑如下：
>
> 1. 头文件引入：引入了NX提供的头文件，包括UF、UF_CSYS、UF_DISP、UF_VIEW等，这些是二次开发常用的头文件。
> 2. 错误处理宏：定义了UF_CALL宏，用于处理函数调用错误，调用失败时会打印错误信息。
> 3. 错误处理函数：实现了report_error函数，用于输出错误信息，包括文件名、行号、调用函数名和错误码等。
> 4. 视图转换函数：实现了do_it函数，该函数通过调用UF函数获取当前视图的中心点和矩阵，然后创建一个临时坐标系对象，并将其设置为WCS。最后刷新显示。
> 5. ufusr函数：这是NX二次开发程序的入口函数。在这个函数中，初始化NX环境，调用do_it函数实现视图转换，并在结束时关闭NX环境。
> 6. 卸载函数：实现了ufusr_ask_unload函数，返回立即卸载标志，表示该程序可以被立即卸载。
>
> 整体而言，这段代码利用NX提供的二次开发接口，实现了将当前视图坐标系转换到当前视图的功能。
>
