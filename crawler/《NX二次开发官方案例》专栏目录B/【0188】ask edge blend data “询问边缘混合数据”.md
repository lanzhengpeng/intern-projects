### 【0188】ask edge blend data “询问边缘混合数据”

#### 代码

```cpp
    /*HEAD ASK_EDGE_BLEND_DATA CCC UFUN */  
    /*  The function UF_MODL_ask_edge_blend does not work - see PR 4258403 译:The function UF_MODL_ask_edge_blend does not work - see PR 4258403 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的功能，用于打印系统日志。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            blend;  
        UF_MODL_edge_blend_data_t  
            blend_data;  
        while (((blend = select_a_feature("Select an edge blend")) != NULL_TAG) &&  
                !UF_CALL(UF_MODL_ask_edge_blend(blend, &blend_data)))  
        {  
            WRITE_D(blend_data.number_edges);  
            for (ii = 0; ii < blend_data.number_edges; ii++)  
                if (blend_data.edge_data[ii].number_points > 0)  
                    UF_free(blend_data.edge_data[ii].point_data);  
            UF_free(blend_data.edge_data);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了UF_CALL宏，用于调用NX的UF函数，并检查返回码，如果返回码不为0，则记录错误信息。
> 2. 定义了write_integer_to_listing_window函数，用于向Listing窗口打印整数值。
> 3. 定义了select_a_feature函数，用于让用户选择一个特征。
> 4. 定义了do_it函数，用于获取用户选择的边混合特征的数据，并打印出来。其中，使用了UF_MODL_ask_edge_blend函数获取边混合数据，然后释放内存。
> 5. 定义了ufusr函数，作为NX的二次开发入口函数，在NX启动时调用，初始化UF环境，执行do_it函数，然后终止UF环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，用于卸载二次开发程序。
>
> 总体来说，这段代码实现了获取用户选择的边混合特征的数据并打印的功能。
>
