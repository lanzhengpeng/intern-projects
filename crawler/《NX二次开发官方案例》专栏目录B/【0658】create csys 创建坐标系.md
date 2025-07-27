### 【0658】create csys 创建坐标系

#### 代码

```cpp
    /*HEAD CREATE_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
        const double x_vec[] = { 0.700, 0.714, 0.0 },  
                     y_vec[] = { -0.714, 0.700, 0.0};  
        tag_t   matrix_id, csys_id;  
        double  mtx[9], csys_origin[] = { 0., 0., 0.};  
        UF_CALL( UF_MTX3_initialize( x_vec, y_vec, mtx ));  
        UF_CALL( UF_CSYS_create_matrix( mtx, &matrix_id ));  
        UF_CALL( UF_CSYS_create_temp_csys( csys_origin, matrix_id, &csys_id ));  
        UF_CALL( UF_CSYS_set_wcs( csys_id ));  
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

> 这段代码是用于NX CAD软件的二次开发，主要功能是创建一个坐标系。
>
> 代码流程如下：
>
> 1. 包含了必要的头文件，如stdio.h、uf.h、uf_mtx.h、uf_csys.h等。
> 2. 定义了宏UF_CALL，用于调用NX API函数，并捕获错误码。如果调用失败，会打印错误信息。
> 3. 定义了report_error函数，用于打印错误信息。
> 4. 定义了do_it函数，其中：
>
> 1. 定义了ufusr函数，这是NX二次开发的入口函数。在这里，初始化NX环境，调用do_it函数，然后终止NX环境。
> 2. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示二次开发模块不需要延迟卸载。
>
> 总体来说，这段代码实现了创建一个临时坐标系，并设置为当前工作坐标系的功能，适用于NX的二次开发。
>
