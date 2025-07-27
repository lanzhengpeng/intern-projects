### 【2497】report system color 报告系统颜色

#### 代码

```cpp
    /*HEAD REPORT_SYSTEM_COLOR CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的描述，UF_print_syslog 是一个在 V18 版本中新增的函数。它用于打印系统日志消息。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /*  Prior to NX 4.0, the returned full color name looked like this:  
        full_color_name = "2 (Green)"  
        So the method shown here cleans it up and returns just the name.  
    */  
    static void ask_color_name(int color_num, char *color_name)  
    {  
        double  
            clr_values[3];  
        char  
            *full_color_name;  
        UF_CALL(UF_DISP_ask_color(color_num, UF_DISP_rgb_model, &full_color_name,  
            clr_values));  
        if (strchr(full_color_name, '('))  
        {  
            strcpy(color_name, strchr(full_color_name, '(')+1);  
            strcpy(strchr(color_name, ')'), "");  
        }  
        else  
            strcpy(color_name, full_color_name);  
        UF_free(full_color_name);  
    }  
    static void do_it(void)  
    {  
        UF_DISP_system_params_t  
            system_parameters;  
        char  
            color_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_DISP_ask_system_parameters(&system_parameters));  
        ask_color_name(system_parameters.color, color_name);  
        WRITE_S(color_name);  
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

> 这是NX二次开发中的系统颜色报告示例代码，主要功能包括：
>
> 1. 定义错误报告函数report_error，用于打印错误代码和消息。
> 2. 定义字符串写入列表窗口的函数write_string_to_listing_window，用于将字符串写入列表窗口。
> 3. 定义获取颜色名称的函数ask_color_name，用于根据颜色编号获取颜色名称。
> 4. 定义执行主体函数do_it，用于获取系统颜色编号，获取颜色名称并写入列表窗口。
> 5. 定义ufusr函数，作为NX二次开发程序的入口，初始化后执行主体函数，然后终止。
> 6. 定义卸载函数ufusr_ask_unload，用于设置程序卸载方式为立即卸载。
> 7. 代码使用了NX提供的UF开发库，通过调用UF函数实现了获取系统颜色名称的功能，并通过列表窗口展示结果。整体逻辑清晰，结构合理，是NX二次开发的一个很好的示例。
>
