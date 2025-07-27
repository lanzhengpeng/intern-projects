### 【2304】report save as info 报告保存为信息

#### 代码

```cpp
    /*HEAD REPORT_SAVE_AS_INFO CCC UFUN */  
    /*  This program demonstrates how to capture any non-part Save As information.  
        It is designed to be installed as a POST action on the File-> Save As...  
        This is the corresponding menufile:  
        VERSION 120  
        EDIT UG_GATEWAY_MAIN_MENUBAR  
        MODIFY  
            BUTTON UG_FILE_SAVE_AS  
            ACTIONS/POST report_save_as_info  
        END_OF_MODIFY  
    */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
    static void do_it(void)  
    {  
        char  
            a_line[MAX_LINE_SIZE+2],  
            the_line[MAX_LINE_SIZE+2],  
            *syslog = NULL;  
        FILE  
            *the_file;  
        /* this function is available beginning with NX 2.0.5 里海译:这个功能从NX 2.0.5版本开始提供。 */  
        UF_CALL( UF_ask_syslog_filename( &syslog ));  
        WRITE( syslog );  
        WRITE( ":\n" );  
        if ((the_file = fopen(syslog, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL)  
                if (strstr(a_line, "! Save Part File As"))  
                    strcpy(the_line, a_line);  
            fclose(the_file);  
        }  
        UF_CALL(UF_UI_write_listing_window(the_line));  
        UF_free( syslog );  
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

> 这段NX二次开发代码的主要功能是捕获文件另存为操作的非零件信息，并安装为文件->另存为…操作的POST动作。
>
> 关键要点包括：
>
> 1. 代码通过修改菜单文件，将report_save_as_info函数作为UG_FILE_SAVE_AS按钮的POST动作。
> 2. report_error函数用于打印错误信息和调用堆栈。
> 3. do_it函数通过UF_ask_syslog_filename获取系统日志文件名，并打开系统日志文件。它遍历文件内容，查找包含"! Save Part File As"的行，并打印该行。
> 4. ufusr函数是二次开发程序的入口点，调用do_it函数，然后终止。
> 5. ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示程序可以立即卸载。
> 6. 这段代码的作用是捕获文件另存为操作的相关信息，并输出到列表窗口，用于调试和记录NX的另存为操作。
>
