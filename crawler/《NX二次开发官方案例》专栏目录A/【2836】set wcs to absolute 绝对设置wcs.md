### 【2836】set wcs to absolute 绝对设置wcs

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的功能，用于打印系统日志信息。 */  
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
            origin[] = { 0,0,0},  
            csys[] = {1,0,0, 0,1,0, 0,0,1};  
        tag_t  
            wcs,  
            mx;  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
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

> 这段代码是NX Open C++ API编写的NX二次开发用户函数代码，主要功能是创建一个临时坐标系并设置为WCS。代码主要包含以下几个部分：
>
> 1. 头文件：包含必要的NX Open C++ API头文件，如uf.h、uf_ui.h、uf_csys.h等。
> 2. UF_CALL宏：该宏用于调用NX Open C++ API函数，并捕获返回码。如果返回码不为0，表示调用失败，则调用report_error函数报告错误。
> 3. report_error函数：用于报告错误信息。它会获取错误码对应的错误信息，并打印到系统日志和NX的Listing Window窗口。
> 4. do_it函数：这是核心功能实现部分。首先，创建一个矩阵mx表示坐标系。然后，使用该矩阵创建一个临时坐标系wcs，并将其设置为WCS。
> 5. ufusr函数：这是NX调用的用户函数入口。在函数内部，先调用UF_initialize进行初始化，然后调用do_it执行主要功能，最后调用UF_terminate进行清理。
> 6. ufusr_ask_unload函数：NX在卸载用户函数库时调用该函数。返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 总体来说，这段代码通过NX Open C++ API实现了创建和设置坐标系的功能，并通过错误处理机制保证了程序的健壮性。
>
