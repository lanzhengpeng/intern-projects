### 【2812】set view layer visibility mask for work view 设置工作视图的视图图层可见性掩码

#### 代码

```cpp
    /*HEAD SET_VIEW_LAYER_VISIBILITY_MASK_FOR_WORK_VIEW CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，它允许用户打印系统日志。 */  
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
        int  
            ii,  
            ip3[256],  
            ip2 = 0,  
            status;  
        UF_CALL(uc6446("", &ip2, ip3));  
        if (ip2 == 1)  /* Read current Global setting for all layers 里海译:Read current Global setting for all layers的中文翻译是：读取所有图层的当前全局设置。 */  
        {  
            for (ii = 0; ii < 256; ii++)  
            {  
                ip3[ii] = 1;  
                UF_CALL(UF_LAYER_ask_status(ii+1, &status));  
                if (status == UF_LAYER_INACTIVE_LAYER) ip3[ii] = 0;  
            }  
        }  
        ip3[0]=0;  /*  Turn layer 1 off 里海译:翻译 Turn layer 1 off 的结果是：关闭第一层。 */  
        ip3[5]=1;  /*  Turn layer 4 on 里海译:打开第4层。 */  
        UF_CALL(uc6447("", 2, ip3));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的NX头文件，用于调用NX的API函数。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了主函数do_it，用于设置工作视图中图层的可见性。
> 4. 首先调用UF_LAYER_ask_global_visibility函数，读取所有图层的当前全局可见性设置，并存储在ip3数组中。
> 5. 遍历ip3数组，如果某个图层处于非激活状态，则设置其可见性为0（不可见）。
> 6. 将第一层（索引为0）的可见性设置为0（关闭第一层），将第四层（索引为5）的可见性设置为1（打开第四层）。
> 7. 调用UF_LAYER_set_global_visibility函数，根据ip3数组设置工作视图中图层的可见性。
> 8. 定义了ufusr函数，初始化NX环境后调用do_it函数，最后终止NX环境。
> 9. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 总的来说，这段代码通过设置工作视图中图层的可见性，实现了控制图层显示的功能。
>
