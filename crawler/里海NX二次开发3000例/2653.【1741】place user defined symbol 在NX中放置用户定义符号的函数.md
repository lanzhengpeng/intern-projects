### 【1741】place user defined symbol 在NX中放置用户定义符号的函数

#### 代码

```cpp
    /*HEAD PLACE_USER_DEFINED_SYMBOL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_drf.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是在V18版本中新增的函数。文档要求只提供翻译，不要添加任何额外内容。因此，翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void build_util_filespec(char *short_name, int ftype, char *fspec)  
    {  
        char  
            *util_dir;  
        UF_CALL(UF_translate_variable("UGII_UTIL", &util_dir));  
        UF_CALL(uc4575(util_dir, ftype, short_name, fspec));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            symbol;  
        double  
            center[3],  
            scale;  
        char  
            sbf_name[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_DRF_symbol_create_data_t  
            symbol_data = { "DATUM1", 0, 2.0, 0.75, NULL_TAG };  
        UF_CALL(uc6430("", center, &scale));  
        UF_CALL(UF_CURVE_create_point(center, &symbol_data.anchor_tag));  
        build_util_filespec("ug_default", 95, sbf_name);  
        UF_CALL(UF_DRF_set_specified_sbf_file(sbf_name));  
        UF_CALL(UF_DRF_place_symbol(&symbol_data, FALSE, FALSE, &symbol));  
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

> 这段代码是用于NX二次开发的示例代码，其主要功能是创建一个基准符号。具体来说：
>
> 1. 包含了NX二次开发常用的头文件，如uf.h、uf_ui.h、uf_cfi.h、uf_drf.h、uf_view.h和uf_curve.h。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 定义了一个写字符串到列表窗口的函数write_string_to_listing_window。
> 4. 定义了一个构建文件路径的函数build_util_filespec。
> 5. 定义了一个主函数do_it，其中包含以下步骤：获取坐标系原点坐标创建一个基准点构建符号的sbf文件路径设置符号的sbf文件创建并放置一个符号
> 6. 获取坐标系原点坐标
> 7. 创建一个基准点
> 8. 构建符号的sbf文件路径
> 9. 设置符号的sbf文件
> 10. 创建并放置一个符号
> 11. 定义了ufusr函数，用于初始化和执行主函数，并在结束时进行清理。
> 12. 定义了一个卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码通过NX提供的API，实现了创建基准符号的功能，并且包含了错误处理、日志输出等辅助功能。
>
