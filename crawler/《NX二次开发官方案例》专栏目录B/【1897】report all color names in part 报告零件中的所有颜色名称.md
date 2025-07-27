### 【1897】report all color names in part 报告零件中的所有颜色名称

#### 代码

```cpp
    /*HEAD REPORT_ALL_COLOR_NAMES_IN_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。 */  
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
    #define WRITE(X)   UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        int  
            cnt,  
            ii;  
        double  
            clr_values[3];  
        char  
            *color_name,  
            msg[MAX_LINE_SIZE+1];  
        UF_UI_open_listing_window();  
        UF_CALL(UF_DISP_ask_color_count(&cnt));  
        for (ii = UF_OBJ_MIN_COLOR; ii < cnt; ii++)  
        {  
            UF_CALL(UF_DISP_ask_color(ii, UF_DISP_rgb_model, &color_name,  
                clr_values));  
            sprintf(msg, "color #%d = %s", ii, color_name);  
            WRITE(msg);  
            if (strlen(color_name) > UF_DISP_MAX_NAME_SIZE)  
                sprintf(msg, " (strlen = %d exceeds UF_DISP_MAX_NAME_SIZE)\n",  
                    strlen(color_name));  
            else  
                strcpy(msg, "\n");  
            WRITE(msg);  
            UF_free(color_name);  
        }  
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

> 这段代码是NX的二次开发代码，主要功能是获取NX系统中所有的颜色名称并打印出来。具体来说，代码主要包含以下几个部分：
>
> 1. 引入必要的NX二次开发头文件，包括uf.h、uf_ui.h、uf_obj.h、uf_disp.h等。
> 2. 定义宏UF_CALL，用于调用NX的UF函数，并捕获错误码。如果函数调用失败，会输出错误信息到syslog和列表窗口。
> 3. 定义宏WRITE，用于向列表窗口打印信息。
> 4. 定义函数do_it，用于获取NX系统中所有的颜色名称并打印。首先获取颜色数量，然后遍历每个颜色，调用UF_DISP_ask_color函数获取颜色名称和RGB值，并打印到列表窗口。同时检查颜色名称长度是否超过最大值。
> 5. 定义ufusr函数，这是NX二次开发的入口函数。在这里初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义ufusr_ask_unload函数，用于卸载二次开发代码。这里设置为立即卸载。
>
> 总的来说，这段代码实现了获取NX系统中所有颜色名称的功能，并通过列表窗口打印出来。代码结构清晰，使用了错误处理和资源释放，是一个典型的NX二次开发示例。
>
