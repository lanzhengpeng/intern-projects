### 【2893】simulate info window header from grip 模拟抓取信息窗口标题

#### 代码

```cpp
    /*HEAD SIMULATE_INFO_WINDOW_HEADER_FROM_GRIP CCC UFUN */  
    /*============================================================================  
    This example works on HP-UX.  
    You may need to change the environment variables for other platforms.  
    ============================================================================*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_cfi.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_(X)  UF_UI_write_listing_window(X)  
    static void simulate_info_window_header_from_grip()  
    {  
        int  
            now[2] = { -1, -1 };  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            user[257] = { "" },  
            partname[257] =  { "" },  
            nodename[257] =  { "" },  
            msg[257] = { "" },  
            *envvar = NULL,  
            dstr[21] = { "" },  
            mstr[MAX_LINE_SIZE+1] = { "" },  
            tstr[21] = { "" };  
        if( NULL_TAG != part )  
        {  
            UF_PART_ask_part_name( part, partname );  
        }  
        UF_UI_exit_listing_window();  
        UF_UI_open_listing_window();  
        WRITE_( "============================================================\n");  
        UF_CALL( UF_translate_variable( "USER", &envvar ) );      
        strcpy( user, envvar );  
        sprintf( msg, "Information listing created by :  %s\n", user );  
        WRITE_( msg );  
        UF_CALL(uc4582(now, 2, dstr, tstr));  
        sprintf(mstr, "%s %s", dstr, tstr);  
        sprintf( msg, "Date                           :  %s", mstr );  
        WRITE_( msg );  
        WRITE_( "\n" );  
        sprintf( msg, "Current work part              :  %s\n", partname );  
        WRITE_( msg );  
        UF_CALL( UF_translate_variable( "SESSION_SVR", &envvar ) );  
        strcpy( nodename, envvar );  
        sprintf( msg, "Node name                      :  %s\n", nodename );  
        WRITE_( msg );  
        WRITE_( "============================================================\n" );  
    }  
    static void do_it(void)  
    {  
        simulate_info_window_header_from_grip();  
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

> 这段代码是NX二次开发的示例代码，其主要功能是模拟NX信息窗口的头部。
>
> 关键功能包括：
>
> 1. 报告错误：使用宏UF_CALL定义了一个函数report_error，用于报告函数调用错误，并输出错误信息到系统日志和列表窗口。
> 2. 环境变量获取：通过UF_translate_variable函数获取环境变量，例如用户名、会话服务器名等，并输出到列表窗口。
> 3. 日期和时间获取：通过uc4582函数获取当前日期和时间，并格式化输出到列表窗口。
> 4. 当前工作部件获取：通过UF_ASSEM_ask_work_part和UF_PART_ask_part_name函数获取当前工作部件名称，并输出到列表窗口。
> 5. 节点名称获取：通过环境变量获取节点名称，并输出到列表窗口。
> 6. 列表窗口操作：使用UF_UI_open_listing_window、UF_UI_write_listing_window等函数操作NX的列表窗口。
> 7. 主函数：定义了主函数ufusr，用于初始化NX，调用do_it执行主要功能，然后终止NX。
> 8. 卸载函数：定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总的来说，这段代码展示了如何通过NX提供的API获取环境变量、日期时间、工作部件等信息，并格式化输出到列表窗口，实现类似NX信息窗口头部的功能。
>
