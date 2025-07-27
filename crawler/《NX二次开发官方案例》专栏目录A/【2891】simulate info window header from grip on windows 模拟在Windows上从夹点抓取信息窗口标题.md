### 【2891】simulate info window header from grip on windows 模拟在Windows上从夹点抓取信息窗口标题

#### 代码

```cpp
    /*HEAD SIMULATE_INFO_WINDOW_HEADER_FROM_GRIP_ON_WINDOWS CCC UFUN */  
    /*============================================================================  
    This example works on Windows.  
    You may need to change the environment variables for other platforms.  
    ============================================================================*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
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
        UF_CALL( UF_translate_variable( "USERNAME", &envvar ) );      
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
        UF_CALL( UF_translate_variable( "COMPUTERNAME", &envvar ) );  
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

> 这段代码是NX的二次开发示例，主要功能是模拟信息窗口的标题栏。具体步骤如下：
>
> 1. 包含必要的NX头文件，并定义了错误报告函数report_error。
> 2. 定义了宏WRITE和WRITE_，用于打开或写入信息窗口。
> 3. 模拟信息窗口标题栏的函数simulate_info_window_header_from_grip，主要步骤包括：获取当前工作部件的名称获取当前用户名、计算机名、日期时间等信息打开信息窗口，并写入标题栏内容，包括用户名、日期时间、当前工作部件名、计算机名等
> 4. 获取当前工作部件的名称
> 5. 获取当前用户名、计算机名、日期时间等信息
> 6. 打开信息窗口，并写入标题栏内容，包括用户名、日期时间、当前工作部件名、计算机名等
> 7. do_it函数调用simulate_info_window_header_from_grip函数。
> 8. 定义了ufusr函数，这是NX二次开发的入口函数，在其中调用do_it函数。
> 9. 定义了ufusr_ask_unload函数，表示二次开发模块立即卸载。
>
> 总的来说，这段代码通过获取系统信息，模拟了NX信息窗口的标题栏，实现了在二次开发中自定义信息窗口标题栏的功能。
>
