### 【2150】report if displayed part is read only 报告如果显示的部件是只读的

#### 代码

```cpp
    /*HEAD REPORT_IF_DISPLAYED_PART_IS_READ_ONLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include  <stdlib.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            disp_part = UF_PART_ask_display_part();  
        logical   
            is_writable  = FALSE;  
        char  
            part[MAX_FSPEC_SIZE+1] = { "" };  
        if( NULL_TAG == disp_part ) return;  
        UF_CALL( UF_PART_ask_part_name( disp_part, part ));   
        UF_CALL( UF_PART_check_part_writable ( part, &is_writable ));  
        WRITE_L( is_writable );  
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

> 这段代码是NX的二次开发代码，主要用于检测当前显示的部件是否可写。以下是代码的主要内容和流程：
>
> 1. 包含必要的NX API头文件，如uf.h、uf_part.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数执行失败时打印错误信息。
> 3. 定义了一个布尔值写入列表窗口的函数write_logical_to_listing_window。
> 4. 定义了do_it函数，该函数首先获取当前显示的部件tag，然后获取该部件的名称，并检查该部件是否可写，最后将结果写入列表窗口。
> 5. ufusr函数是NX二次开发程序的入口函数，在这里调用do_it函数，并在结束时进行资源清理。
> 6. ufusr_ask_unload函数用于设置二次开发程序卸载时的行为，这里设置为立即卸载。
>
> 总的来说，这段代码实现了检测当前显示部件是否可写的功能，并将结果输出到列表窗口。如果我是这段代码的作者，我会考虑加入更多的错误处理和日志输出，以增强程序的健壮性。
>
