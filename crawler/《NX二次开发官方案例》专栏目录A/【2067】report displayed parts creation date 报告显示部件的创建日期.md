### 【2067】report displayed parts creation date 报告显示部件的创建日期

#### 代码

```cpp
    /*HEAD REPORT_DISPLAYED_PARTS_CREATION_DATE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数，用于打印系统日志信息。 */  
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
            cdate[2],  
            ldate[2],  
            mdate[2];  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            datestg[21],  
            pspec[MAX_FSPEC_SIZE + 1],  
            timestg[21];  
        UF_PART_ask_part_name(part, pspec);  
        if (!UF_CALL(uc4564(pspec, 0, (1 << 4))))  
        {  
            UF_CALL(uc4606(cdate, mdate, ldate));  
            UF_CALL(uc4582(cdate, 3, datestg, timestg));  
            printf("Creation date & time     - %s %s\n", datestg, timestg);  
            UF_CALL(uc4582(mdate, 3, datestg, timestg));  
            printf("Modification date & time - %s %s\n", datestg, timestg);  
            UF_CALL(uc4582(ldate, 3, datestg, timestg));  
            printf("Last Access date & time  - %s %s\n", datestg, timestg);  
        }  
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

> 这是一段用于在NX中获取当前显示部件的创建日期、修改日期和最后访问日期的二次开发代码。代码的主要功能包括：
>
> 1. 使用UF_PART_ask_display_part()函数获取当前在NX中显示的部件。
> 2. 使用UF_PART_ask_part_name()函数获取该部件的名称。
> 3. 使用uc4564()函数判断该部件是否具有日期属性。
> 4. 使用uc4606()函数获取该部件的创建日期、修改日期和最后访问日期。
> 5. 使用uc4582()函数将日期和时间转换为字符串格式。
> 6. 输出部件的创建日期、修改日期和最后访问日期。
> 7. 在函数ufusr()中初始化和终止NX的二次开发环境。
> 8. 在函数ufusr_ask_unload()中返回卸载标志，表示程序结束后立即卸载。
> 9. 使用宏UF_CALL()检查函数调用是否成功，如果不成功则输出错误信息。
> 10. 使用UF_print_syslog()函数输出错误信息到系统日志。
> 11. 使用UF_UI系列函数输出错误信息到列表窗口。
>
> 总体来说，这段代码实现了在NX中获取当前显示部件的创建日期、修改日期和最后访问日期的功能，并具有完善的错误处理机制。
>
