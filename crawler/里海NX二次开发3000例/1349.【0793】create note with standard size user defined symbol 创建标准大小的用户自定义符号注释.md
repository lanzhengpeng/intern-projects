### 【0793】create note with standard size user defined symbol 创建标准大小的用户自定义符号注释

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_view.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            note;  
        double  
            center[3],  
            scale;  
        char  
            *text[1] = { "datum1 <%datum1>" };  
        UF_DRF_uds_size_t  
            uds_size = { UF_DRF_SCALE_ASPECT_RATIO, 1, 1 };  
        UF_CALL(uc6430("", center, &scale));  
        UF_CALL(UF_DRF_set_uds_size(&uds_size));  
        UF_CALL(UF_DRF_create_note(1, text, center, 0, &note));  
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

> 这段NX二次开发代码主要包括以下内容：
>
> 1. 包含了NX二次开发所需的头文件，如uf.h、uf_ui.h、uf_drf.h、uf_view.h。
> 2. 定义了宏ECHO，用于打印信息到NX的输出窗口和日志文件。
> 3. 定义了宏UF_CALL，用于调用NX的函数，并在出错时打印错误信息。
> 4. 定义了report_error函数，用于打印函数调用出错的详细信息。
> 5. 定义了do_it函数，该函数实现了以下功能：获取当前工作坐标系的原点坐标。设置注释的缩放比例。创建一个注释对象，内容为"datum1 <%datum1>"，放置在坐标系原点。
> 6. 获取当前工作坐标系的原点坐标。
> 7. 设置注释的缩放比例。
> 8. 创建一个注释对象，内容为"datum1 <%datum1>"，放置在坐标系原点。
> 9. 定义了ufusr函数，这是NX二次开发程序的入口函数。它首先初始化NX环境，然后调用do_it函数，最后终止NX环境。
> 10. 定义了ufusr_ask_unload函数，用于设置二次开发程序的卸载方式为立即卸载。
>
> 总的来说，这段代码实现了一个简单的功能：在NX的当前工作坐标系原点位置创建一个指定格式的注释。
>
