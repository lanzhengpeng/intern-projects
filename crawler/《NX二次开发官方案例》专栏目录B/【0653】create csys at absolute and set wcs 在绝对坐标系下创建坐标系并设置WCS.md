### 【0653】create csys at absolute and set wcs 在绝对坐标系下创建坐标系并设置WCS

#### 代码

```cpp
    /*HEAD CREATE_CSYS_AT_ABSOLUTE_AND_SET_WCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 的新功能，用于打印系统日志。 */  
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
            matrix_id = NULL_TAG,  
            abs_csys = NULL_TAG;  
        double  
            origin[3] = { 0., 0., 0. },  
            matrix_values[ 9 ]= { 1., 0., 0.,   0., 1., 0.,   0., 0., 1. };  
        UF_CALL( UF_CSYS_create_matrix ( matrix_values, &matrix_id ));  
        UF_CALL( UF_CSYS_create_csys( origin, matrix_id, &abs_csys ));  
        UF_CALL( UF_CSYS_set_wcs( abs_csys ));       
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_set_retiring_flag( 2 ); /* notify about retiring functions 译:关于退役功能的通告 */  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++ API编写的NX二次开发示例代码，主要用于创建一个绝对坐标系并设置它为工作坐标系。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个do_it函数，用于创建一个单位矩阵对应的坐标系，并创建一个原点为(0,0,0)的绝对坐标系。
> 3. 在ufusr函数中，首先调用UF_initialize初始化NX Open API，然后调用do_it函数执行创建坐标系和设置工作坐标系的功能。
> 4. 最后调用UF_terminate终止NX Open API。
> 5. ufusr_ask_unload函数用于在用户卸载该程序时立即卸载。
>
> 通过这个示例，展示了如何使用NX Open C++ API进行坐标系操作，包括创建坐标系、设置工作坐标系等。这对于NX二次开发中坐标系变换等操作非常有用。
>
