### 【1950】report all plot queues on default node 在默认节点上报告所有绘图队列

#### 代码

```cpp
    /*HEAD REPORT_ALL_PLOT_QUEUES_ON_DEFAULT_NODE CCC UFUN */  
    /* This sample program is valid for PRE-NX3 versions only 里海译:这个示例程序仅适用于PRE-NX3版本。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是一个新函数，在V18版本中首次引入。其功能是用于打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            que_cnt;  
        char  
            *node;  
        UF_PLOT_que_p_t  
            *queues;  
        if (UF_CALL(UF_translate_variable("UGII_PLOT_NODE", &node)) || !node)  
            return;  
        UF_CALL(UF_PLOT_ask_queues(node, &que_cnt, &queues));  
        WRITE_D(que_cnt);  
        for (ii = 0; ii < que_cnt; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_S(queues[ii]->name);  
            WRITE("queues[ii]->stat = ");  
            switch(queues[ii]->stat)  
            {  
                case UF_PLOT_QSTAT_UP:  
                    WRITE_D(UF_PLOT_QSTAT_UP);  
                    break;  
                case UF_PLOT_QSTAT_DOWN:  
                    WRITE_D(UF_PLOT_QSTAT_DOWN);  
                    break;  
                case UF_PLOT_QSTAT_CLOSED:  
                    WRITE_D(UF_PLOT_QSTAT_CLOSED);  
                    break;  
                case UF_PLOT_QSTAT_AUTO_UP:  
                    WRITE_D(UF_PLOT_QSTAT_AUTO_UP);  
                    break;  
                case UF_PLOT_QSTAT_AUTO_DOWN:  
                    WRITE_D(UF_PLOT_QSTAT_AUTO_DOWN);  
                    break;  
                case UF_PLOT_QSTAT_UNKNOWN:  
                    WRITE_D(UF_PLOT_QSTAT_UNKNOWN);  
                    break;  
                default:  
                    WRITE("Unknown value\n");  
                    WRITE_D(queues[ii]->stat);  
                    break;  
            }  
            UF_free(queues[ii]->name);  
        }  
        if (que_cnt > 0) UF_free(queues);  
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

> 这段代码是用于在NX中查询默认绘图节点上所有绘图队列状态的二次开发代码。
>
> 代码的主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息和调用信息。
> 2. 定义了宏WRITE用于在列表窗口打印信息。
> 3. 定义了函数do_it，用于执行查询。
> 4. 在do_it函数中，首先获取默认绘图节点名。
> 5. 然后调用UF_PLOT_ask_queues查询该节点上的所有绘图队列，包括队列数组和每个队列的状态。
> 6. 最后遍历队列数组，打印每个队列的名称和状态，状态包括开启、关闭、自动开启、自动关闭等。
> 7. ufusr函数是NX调用的主函数，首先初始化NX，然后调用do_it执行查询，最后终止NX。
> 8. ufusr_ask_unload函数用于在不需要卸载时返回立即卸载标志。
>
> 总的来说，这段代码实现了在NX中查询默认绘图节点上所有绘图队列状态的功能。
>
