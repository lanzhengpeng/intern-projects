### 【0402】blank all datum planes 隐藏所有基准面

#### 代码

```cpp
    /*HEAD BLANK_ALL_DATUM_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数。文档中提到：“注意：UF_print_syslog是V18版本新增的。” */  
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
    static tag_t ask_next_dplane(tag_t part, tag_t dplane)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_datum_plane_type, &dplane));  
        return (dplane);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dplane = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((dplane = ask_next_dplane(part, dplane)) != NULL_TAG)  
            UF_CALL(UF_OBJ_set_blank_status(dplane, UF_OBJ_BLANKED));  
    }  
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

> 这段代码是一个NX Open C++ API的二次开发示例，其主要功能是隐藏当前显示部件中的所有基准面。
>
> 代码的主要组成部分包括：
>
> 1. 错误报告函数report_error：用于在发生API调用错误时，输出错误信息到日志和列表窗口。
> 2. 基准面循环函数ask_next_dplane：用于循环获取当前部件中的下一个基准面。
> 3. 主体函数do_it：用于循环获取当前部件中的所有基准面，并设置它们的隐藏状态。
> 4. UFusr函数：NX二次开发程序的入口函数，用于初始化和调用主体函数。
> 5. 卸载函数ufusr_ask_unload：用于设置程序卸载方式为立即卸载。
>
> 这段代码通过循环获取当前部件中的所有基准面，并设置它们的隐藏状态，实现了隐藏当前显示部件中的所有基准面的功能。这为NX二次开发提供了一个隐藏基准面的示例。
>
