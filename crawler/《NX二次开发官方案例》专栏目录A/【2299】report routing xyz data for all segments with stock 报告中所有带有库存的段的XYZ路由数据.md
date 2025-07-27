### 【2299】report routing xyz data for all segments with stock 报告中所有带有库存的段的XYZ路由数据

#### 代码

```cpp
    /*HEAD REPORT_ROUTING_XYZ_DATA_FOR_ALL_SEGMENTS_WITH_STOCK CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_route.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的，它是一个新的函数。文档要求只回答翻译内容，不要添加任何额外说明。因此，翻译内容如下：

注意：UF_print_syslog是V18版本中新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
        UF_ROUTE_bend_report_xyz_t  
            *xyz_data;  
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
            UF_CALL(UF_ROUTE_bend_report_generate_xyz_report(bend_segs,  
                &xyz_data));  
            WRITE_D(xyz_data->num_bends);  
            for (jj = 0; jj < xyz_data->num_bends; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_F(xyz_data->xc[jj]);  
                WRITE_F(xyz_data->yc[jj]);  
                WRITE_F(xyz_data->zc[jj]);  
                WRITE_F(xyz_data->radii[jj]);  
            }  
            WRITE_F(xyz_data->total_length);  
            UF_CALL(UF_ROUTE_bend_report_free_xyz_report(xyz_data));  
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

> 这段NX二次开发代码的主要功能是获取零件中所有线段的弯曲信息，并报告每个线段的弯曲点的XYZ坐标和半径。
>
> 具体来说，代码首先获取当前显示的零件，然后通过UF_ROUTE_ask_part_segs函数获取零件中所有线段的tag，并存入segments数组中。接着，对每个线段，使用UF_ROUTE_bend_report_get_segment_info函数获取其弯曲信息，并使用UF_ROUTE_bend_report_generate_xyz_report函数生成弯曲点的XYZ报告。最后，遍历每个弯曲点，打印出其XYZ坐标和半径信息，并释放相关资源。在遍历完所有线段后，释放segments数组。
>
> 此外，代码还定义了宏WRITE_D和WRITE_F，用于向列表窗口打印整型和浮点型变量。定义了report_error函数，用于在函数调用失败时打印错误信息。
>
> 总的来说，这段代码实现了获取零件线段弯曲信息的功能，并提供了打印调试信息的能力。
>
