### 【2298】report routing mil98c data for all segments with stock 查询所有带有库存的线路段的MIL98C路由报告数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_route.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_segments,  
            n_stock;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *segments,  
            *stock;  
        UF_ROUTE_bend_segment_info_t  
            *bend_segs;  
        UF_ROUTE_bend_report_mil98_t  
            *mil98c_data;  
        UF_CALL(UF_ROUTE_ask_part_segs(part, &n_segments, &segments));  
        WRITE_D(n_segments);  
        for (ii = 0; ii < n_segments; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(segments[ii]);  
            UF_CALL(UF_ROUTE_ask_segment_stock(segments[ii], &n_stock, &stock));  
            WRITE_D(n_stock);  
            if (n_stock == 0) continue;  
            UF_free(stock);  
            UF_CALL(UF_ROUTE_bend_report_get_segment_info(segments[ii],  
                &bend_segs));  
            UF_CALL(UF_ROUTE_bend_report_generate_mil98_report(bend_segs,  
                &mil98c_data));  
            WRITE_D(mil98c_data->num_bends);  
            for (jj = 0; jj < mil98c_data->num_bends; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_F(mil98c_data->c[jj]);  
                WRITE_F(mil98c_data->f[jj]);  
                WRITE_F(mil98c_data->e[jj]);  
                WRITE_F(mil98c_data->g[jj]);  
                WRITE_F(mil98c_data->y[jj]);  
            }  
            WRITE_F(mil98c_data->total_length);  
            UF_CALL(UF_ROUTE_bend_report_free_mil98_report(mil98c_data));  
            UF_CALL(UF_ROUTE_bend_report_free_segment_info(bend_segs));  
        }  
        if (n_segments > 0) UF_free(segments);  
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

> 这段代码是NX的二次开发代码，其主要功能是获取当前显示零件的折弯信息，并输出到日志窗口。
>
> 具体来说，代码的主要步骤包括：
>
> 1. 初始化NX API。
> 2. 获取当前显示零件的折弯路径段信息，并输出路径段数量。
> 3. 遍历每个路径段，获取其对应的折弯报告信息。
> 4. 输出每个折弯的参数，包括C、F、E、G、Y等值。
> 5. 输出路径段的总长度。
> 6. 释放折弯报告信息。
> 7. 释放路径段信息。
> 8. 结束NX API。
>
> 此外，代码中使用了宏定义来简化错误报告、日志输出等操作。整体来看，该代码实现了获取并输出零件折弯信息的功能。
>
