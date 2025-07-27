### 【2426】report wave delay status 报告波延迟状态

#### 代码

```cpp
    /*HEAD REPORT_WAVE_DELAY_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_wave.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_delay_status( UF_WAVE_delay_status_t delay_status )  
    {  
        switch ( delay_status )  
        {  
            case UF_WAVE_not_delayed:  
            WRITE( "The part is not delayed.\n" );  
            break;  
            case UF_WAVE_session_delayed:  
            WRITE( "The part is delayed because the session is delayed.\n" );  
            break;  
            case UF_WAVE_partial_load_delayed:  
            WRITE( " The part is delayed because it's partially loaded.\n" );  
            break;  
            case UF_WAVE_session_frozen:  
            WRITE( "The part is frozen in this session.\n" );  
            break;  
            case UF_WAVE_persistent_frozen:  
            WRITE( "The part is frozen for all sessions.\n" );  
            break;  
            case UF_WAVE_not_in_assembly_delayed:  
            WRITE( "Part delayed: it is not in the displayed assembly.\n" );  
            break;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            num_parts = 0,  
            inx = 0;  
        tag_t  
            thisPart = NULL_TAG;  
        char   
            full_name[MAX_FSPEC_SIZE + 1] = { "" };  
        UF_WAVE_delay_status_t   
            delay_status;  
        num_parts = UF_PART_ask_num_parts();  
        for( inx = 0; inx < num_parts; inx++ )  
        {  
            thisPart = UF_PART_ask_nth_part(inx);  
            UF_CALL( UF_PART_ask_part_name( thisPart, full_name ));  
            WRITE_S( full_name );  
            UF_CALL( UF_WAVE_ask_delay_status( thisPart, &delay_status ));  
            report_delay_status( delay_status );          
            WRITE( "\n\n" );  
        }  
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

> 这段代码是一个NX的二次开发示例，主要功能是遍历当前会话中的所有零件，并打印出每个零件的名称和时延状态。
>
> 主要步骤包括：
>
> 1. 引入所需的NX API头文件。
> 2. 定义错误报告函数report_error，用于打印错误信息。
> 3. 定义WRITE和WRITE_S宏，用于打印信息到日志窗口。
> 4. 定义write_string_to_listing_window函数，用于将字符串写入日志窗口。
> 5. 定义report_delay_status函数，根据不同的时延状态打印相应的信息。
> 6. 定义do_it函数，遍历所有零件，打印名称和时延状态。
> 7. ufusr函数是程序的入口，初始化NX，调用do_it函数，然后终止NX。
> 8. ufusr_ask_unload函数用于在用户退出时卸载程序。
>
> 整体上，这段代码实现了遍历所有零件并打印其名称和时延状态的功能，是NX二次开发的一个典型示例。
>
