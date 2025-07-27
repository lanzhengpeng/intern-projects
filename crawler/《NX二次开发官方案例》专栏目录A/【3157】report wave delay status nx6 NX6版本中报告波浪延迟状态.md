### 【3157】report wave delay status nx6 NX6版本中报告波浪延迟状态

#### 代码

```cpp
    /*HEAD REPORT_WAVE_DELAY_STATUS_NX6 CCC UFUN */  
    /******************************************************  
        The UF_WAVE_delay_status_e enum was modified in NX6  
    ******************************************************/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_wave.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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

> 这段代码是NX6版本中的二次开发示例，其主要功能是查询当前打开的部件在波前中的延迟状态，并将结果输出到日志窗口。具体实现包括以下关键点：
>
> 1. 定义了report_error函数，用于在调用UF函数出错时输出错误信息。
> 2. 定义了WRITE和WRITE_S宏，用于向日志窗口输出信息。
> 3. 定义了report_delay_status函数，用于根据延迟状态枚举值输出相应的提示信息。
> 4. do_it函数遍历当前会话中的所有部件，调用UF_WAVE_ask_delay_status函数查询每个部件的延迟状态，并输出部件名称和延迟状态。
> 5. ufusr是NX二次开发的入口函数，在NX启动时调用，其中初始化UF函数，调用do_it函数，然后终止UF。
> 6. ufusr_ask_unload函数用于在NX卸载时调用，返回立即卸载标志。
>
> 整体来看，这段代码通过遍历部件并调用UF函数，实现了查询和输出部件波前延迟状态的功能，展现了NX二次开发的基本流程。
>
