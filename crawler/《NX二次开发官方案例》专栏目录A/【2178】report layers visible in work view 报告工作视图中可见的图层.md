### 【2178】report layers visible in work view 报告工作视图中可见的图层

#### 代码

```cpp
    /*HEAD REPORT_LAYERS_VISIBLE_IN_WORK_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_layer.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog在V18中是新增的，因此只需要回答翻译内容，不需要添加其他无关内容。 */  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
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
                sprintf(msg, "%d", array[kk++]);  
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
        int  
            cnt = 0,  
            ii,  
            ip3[256],  
            ip2,  
            status,  
            visible_layers[256];  
        UF_CALL(uc6446("", &ip2, ip3));  
        for (ii = 0; ii < 256; ii++)  
        {  
            if (ip2 == 1)  
            {  
                UF_CALL(UF_LAYER_ask_status(ii+1, &status));  
                if (status != UF_LAYER_INACTIVE_LAYER)  
                    visible_layers[cnt++] = ii+1;  
            }  
            else  
                if (ip3[ii] == 1) visible_layers[cnt++] = ii+1;  
        }  
        WRITE_D(cnt);  
        WRITEnD(cnt, visible_layers);  
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

> 这段代码是用于NX二次开发的用户函数，主要功能是获取当前工作视图下可见的图层并报告输出。具体来说，代码的主要功能如下：
>
> 1. 包含了NX Open相关的头文件，如uf.h、uf_ui.h、uf_view.h、uf_layer.h等，用于调用NX Open API。
> 2. 定义了宏UF_CALL，用于封装函数调用，并报告错误信息。
> 3. 实现了错误报告函数report_error，用于打印函数调用失败的信息。
> 4. 定义了宏WRITE_D和WRITEnD，用于向列表窗口输出整数和整数数组。
> 5. 实现了do_it函数，该函数首先获取所有图层的可见性状态，然后筛选出当前工作视图下可见的图层，并将它们输出到列表窗口。
> 6. 实现了ufusr函数，该函数初始化NX Open，调用do_it函数，然后终止NX Open。
> 7. 实现了ufusr_ask_unload函数，该函数用于卸载用户函数，返回立即卸载的标志。
> 8. 代码通过UF_LAYER_ask_status函数获取每个图层的激活状态，然后判断是否为活动图层，如果是，则认为该图层在当前工作视图下可见。
> 9. 可见图层的编号被存储在visible_layers数组中，并在列表窗口中输出。
> 10. 代码采用了NX Open版本18及以上版本的API函数。
>
> 总之，这段代码通过调用NX Open API，实现了获取和报告当前工作视图下可见图层编号的功能。
>
