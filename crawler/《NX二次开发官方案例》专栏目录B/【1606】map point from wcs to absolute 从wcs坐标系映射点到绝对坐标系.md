### 【1606】map point from wcs to absolute 从wcs坐标系映射点到绝对坐标系

#### 代码

```cpp
    /*HEAD MAP_POINT_FROM_WCS_TO_ABSOLUTE CCC UFUN */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void do_it(void)  
    {  
        double  
            input[3] = { 100., 100., 0. },  
            output[3] = { 0., 0., 0. };  
        UF_CALL( UF_CSYS_map_point( UF_CSYS_ROOT_WCS_COORDS,input,   
                                       UF_CSYS_ROOT_COORDS, output ));  
        WRITE3F( output );  
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

> 这是段NX Open C++ API开发的代码，主要实现了将一个点从WCS坐标系映射到绝对坐标系的功能。
>
> 1. 代码结构：包含了必要的头文件。定义了UF_CALL宏，用于调用NX API函数，并报告错误。定义了WRITE宏，用于向日志窗口输出信息。定义了write_double_array_to_listing_window函数，用于向日志窗口输出数组。定义了do_it函数，用于实现坐标映射功能。实现了ufusr函数，作为程序的入口，初始化NX Open，调用do_it函数，然后终止NX Open。实现了ufusr_ask_unload函数，返回立即卸载标志。
> 2. 包含了必要的头文件。
> 3. 定义了UF_CALL宏，用于调用NX API函数，并报告错误。
> 4. 定义了WRITE宏，用于向日志窗口输出信息。
> 5. 定义了write_double_array_to_listing_window函数，用于向日志窗口输出数组。
> 6. 定义了do_it函数，用于实现坐标映射功能。
> 7. 实现了ufusr函数，作为程序的入口，初始化NX Open，调用do_it函数，然后终止NX Open。
> 8. 实现了ufusr_ask_unload函数，返回立即卸载标志。
> 9. 主要功能：通过UF_CSYS_map_point函数，将WCS坐标系下的点(input)映射到绝对坐标系下(output)。通过WRITE3F宏，将映射后的绝对坐标(output)输出到日志窗口。
> 10. 通过UF_CSYS_map_point函数，将WCS坐标系下的点(input)映射到绝对坐标系下(output)。
> 11. 通过WRITE3F宏，将映射后的绝对坐标(output)输出到日志窗口。
> 12. 注意事项：调用NX API函数前要调用UF_initialize进行初始化。调用UF_terminate终止NX Open。使用宏UF_CALL可以方便地捕获和报告API函数调用错误。输出日志信息可以方便调试。
> 13. 调用NX API函数前要调用UF_initialize进行初始化。
> 14. 调用UF_terminate终止NX Open。
> 15. 使用宏UF_CALL可以方便地捕获和报告API函数调用错误。
> 16. 输出日志信息可以方便调试。
>
> 这段代码实现了基本的坐标系映射功能，并且使用了NX Open C++ API中的坐标系处理函数。
>
