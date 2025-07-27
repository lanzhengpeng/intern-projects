### 【2070】report displayed parts file size 报告中显示的零件文件大小

#### 代码

```cpp
    /*HEAD REPORT_DISPLAYED_PARTS_FILE_SIZE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，用于打印系统日志。 */  
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
            length = -1;  
        char  
            pspec[MAX_FSPEC_SIZE + 1];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, pspec);  
        UF_CALL(uc4564(pspec, 0, (1 << 4)));  
        length = uc4605();  
        printf("Part: %s  Length: %d bytes \n", pspec, length);  
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

> 这是段NX Open的二次开发代码，用于获取当前显示部件的信息。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时报告错误信息。
> 2. 定义了一个do_it函数，用于获取当前显示部件的名称和文件大小。
> 3. 在ufusr函数中，首先调用UF_initialize进行NX Open初始化，然后调用do_it函数，最后调用UF_terminate终止NX Open。
> 4. ufusr_ask_unload函数用于设置立即卸载二次开发程序。
>
> 关键函数包括：
>
> 1. UF_PART_ask_display_part：获取当前显示部件的tag。
> 2. UF_PART_ask_part_name：根据部件tag获取部件名称。
> 3. uc4564：设置获取部件文件大小的参数。
> 4. uc4605：获取部件文件大小。
> 5. UF_print_syslog：打印系统日志信息。
> 6. UF_UI_open_listing_window：打开列表窗口。
> 7. UF_UI_write_listing_window：向列表窗口写入文本。
> 8. UF_initialize：初始化NX Open。
> 9. UF_terminate：终止NX Open。
>
> 这段代码实现了在NX中获取当前显示部件的名称和文件大小的功能，并在出错时能够进行错误报告。
>
