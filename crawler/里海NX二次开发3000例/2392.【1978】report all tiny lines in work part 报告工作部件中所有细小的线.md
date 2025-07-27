### 【1978】report all tiny lines in work part 报告工作部件中所有细小的线

#### 代码

```cpp
    /*HEAD REPORT_ALL_TINY_LINES_IN_WORK_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是在 V18 版本中新增的函数。文档指出，只需要提供UF_print_syslog的翻译，不需要添加任何其他内容。因此，UF_print_syslog的翻译为打印系统日志。 */  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    /* qq3123197280 */  
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
        tag_t  
            line = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        logical  
            is_tiny = 0;  
        char  
            msg[300] = { "" };  
        UF_CURVE_line_t  
            line_coords;  
            if( NULL_TAG == part ) return;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part( part, UF_line_type, &line ))  
                          && ( NULL_TAG != line ))  
        {  
            /* The 0.01 is an arbitrary tolerance to demonstrate how this works.  
                   You may wish to use another tolerance in a production program.   
                */  
            UF_CALL(UF_MODL_ask_tiny_geometry(line, 0.01, &is_tiny));  
            if ( is_tiny == TRUE )  
            {  
                sprintf( msg, "\nLine %d is tiny\n", line);  
                WRITE( msg );  
                UF_CALL( UF_CURVE_ask_line_data( line, &line_coords ));  
                WRITE3F( line_coords.start_point );  
                WRITE3F( line_coords.end_point );  
            }   
        }  
        /* then you would need to repeat the cycling process for   
              splines, arcs, conics, and possibly edges.  
          */  
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

> 这段代码是一个NX Open C++二次开发示例，主要用于检测并报告工作部件中的微小线段。
>
> 主要功能包括：
>
> 1. 初始化UF系统。
> 2. 循环遍历工作部件中的所有线段，并使用UF_MODL_ask_tiny_geometry函数检测线段是否小于给定的公差（本例中为0.01）。
> 3. 如果检测到微小线段，则打印相关信息到列表窗口，并输出线段起点和终点的坐标。
> 4. 继续循环，直到遍历完所有线段。
> 5. 终止UF系统。
>
> 代码的关键点包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个数组打印函数write_double_array_to_listing_window，用于将数组打印到列表窗口。
> 3. 主函数do_it实现了上述主要功能。
> 4. ufusr是NX调用的主函数。
> 5. ufusr_ask_unload函数用于立即卸载用户程序。
>
> 该示例代码主要用于演示如何检测并报告NX模型中的微小几何元素，具有一定的参考价值。
>
