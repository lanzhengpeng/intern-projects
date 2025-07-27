### 【1490】join curves from grip 在Grip中连接曲线

#### 代码

```cpp
    /*HEAD JOIN_CURVES_FROM_GRIP CCC UFUN */  
    /*  Since there is no GRIP command to join curves, this example demonstrates  
        passing an array of curves from GRIP, auto joining them, and then  
        returning the resultant joined curves back.  
        Here is sample GRIP code demonstating how to use the program:  
            ENTITY/ curves(1000), joins(1000)  
            NUMBER/ n_curves, n_joins  
            GRARGS/ curves, n_curves, joins, n_joins  
            MASK/ 3,5,6,9  
    a10:    IDENT/ 'Select curves to join', curves, CNT, n_curves, resp  
            IFTHEN/ resp > 2  
                XSPAWN/ UFUN, 'ufunc/join_curves_from_grip.sl'  
                DO/ a20:, ii, 1, n_joins  
                    &COLOR(joins(ii)) = &COLOR(curves(ii)) + 1  
    a20:        $$  end do ii  
                DELETE/ curves(1..n_curves)  
                JUMP/ a10:  
            ENDIF  
            HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。 */  
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
    #define SIZE_OF_ENTITY_ARRAY  1000  
    static void do_it(void)  
    {  
        tag_t  
            curves[SIZE_OF_ENTITY_ARRAY],  
            joins[SIZE_OF_ENTITY_ARRAY];  
        int  
            n;  
        double  
            n_curves,  
            n_joins;  
        UF_args_t  
            args[4];  
        args[0].type = UF_TYPE_TAG_T_ARRAY;  
        args[0].length = SIZE_OF_ENTITY_ARRAY;  
        args[0].address = curves;  
        args[1].type = UF_TYPE_DOUBLE;  
        args[1].length = 0;  
        args[1].address = &n_curves;  
        args[2].type = UF_TYPE_TAG_T_ARRAY;  
        args[2].length = SIZE_OF_ENTITY_ARRAY;  
        args[2].address = joins;  
        args[3].type = UF_TYPE_DOUBLE;  
        args[3].length = 0;  
        args[3].address = &n_joins;  
        UF_CALL(UF_ask_grip_args(4, args));  
        UF_CALL(UF_CURVE_auto_join_curves(curves, (int) n_curves, 1, joins, &n));  
        n_joins = n;  
        UF_CALL(UF_set_grip_args(4, args));  
    }  
    /* qq3123197280 */  
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

> 这段代码是NX的二次开发代码，主要实现了从GRIP获取一组曲线，自动连接这些曲线，并将连接后的曲线返回给GRIP的功能。具体来说：
>
> 1. 定义了UF_CALL宏，用于调用UF函数并报告错误。
> 2. 定义了report_error函数，用于打印错误信息。
> 3. 定义了do_it函数，用于从GRIP获取曲线数组、曲线数量、返回曲线数组、返回曲线数量，并调用UF_CURVE_auto_join_curves函数自动连接曲线。
> 4. 定义了ufusr函数，用于初始化、调用do_it函数、终止。
> 5. 定义了ufusr_ask_unload函数，用于立即卸载用户函数。
> 6. 使用UF_print_syslog函数在系统日志中打印错误信息，这是NX 18新增的函数。
> 7. 使用UF_ask_grip_args和UF_set_grip_args函数与GRIP进行参数传递。
> 8. 使用UF_CURVE_auto_join_curves函数自动连接曲线。
> 9. 使用tag_t数组存储曲线和连接后的曲线。
> 10. 使用UF_TYPE_TAG_T_ARRAY指定参数类型为tag_t数组。
> 11. 使用UF_TYPE_DOUBLE指定参数类型为double。
> 12. 使用UF_UNLOAD_IMMEDIATELY立即卸载用户函数。
>
> 总体来说，这段代码通过NX的二次开发接口，实现了GRIP与NX之间的曲线连接功能。
>
