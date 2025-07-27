### 【2260】report out of date wave parts 报告过时的Wave组件

#### 代码

```cpp
    /*HEAD REPORT_OUT_OF_DATE_WAVE_PARTS CCC UFUN */  
    /*  Components with WAVE geometry should be fully loaded before using this program. 里海译:组件需要完全加载 WAVE 几何图形后才能使用此程序。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_wave.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的内容，UF_print_syslog是V18版本新增的。 */  
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
        int  
            inx = 0,  
            n_parts = 0;  
        tag_t  
            *parts = NULL;  
        char  
            name[MAX_FSPEC_SIZE+1] = { "" };  
        UF_CALL( UF_WAVE_ask_out_of_date_parts ( &n_parts, &parts ));  
        if( n_parts )  
        {  
            UF_UI_exit_listing_window();  
            WRITE( "Out of Date:\n" );     
        }  
        for( inx = 0; inx < n_parts; inx++ )  
        {  
            UF_CALL( UF_PART_ask_part_name ( parts[inx], name ) );  
            WRITE( name );  
            WRITE( "\n" );  
        }  
        if( n_parts )  UF_free( parts );     
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

> 这段代码是NX Open C++ API编写的二次开发代码，其主要功能是检查NX装配中的WAVE组件是否已过时，即其几何形状是否已更新，但装配中的引用尚未更新。代码主要包含以下部分：
>
> 1. 包含了必要的头文件，如UF、UF_WAVE、UF_UI、UF_PART等。
> 2. 定义了错误报告函数report_error，用于在调用UF函数出错时输出错误信息。
> 3. 定义了宏WRITE，用于在UF_listing窗口中输出信息。
> 4. 定义了do_it函数，该函数是主要功能实现：调用UF_WAVE_ask_out_of_date_parts函数获取过时的WAVE组件数量和组件标签数组。如果存在过时的组件，则打开UF_listing窗口，并输出“Out of Date:”。遍历组件数组，使用UF_PART_ask_part_name函数获取组件名称，并在窗口中输出。
> 5. 调用UF_WAVE_ask_out_of_date_parts函数获取过时的WAVE组件数量和组件标签数组。
> 6. 如果存在过时的组件，则打开UF_listing窗口，并输出“Out of Date:”。
> 7. 遍历组件数组，使用UF_PART_ask_part_name函数获取组件名称，并在窗口中输出。
> 8. 定义了ufusr函数，作为程序的入口函数，在初始化UF系统后调用do_it函数，最后终止UF系统。
> 9. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序卸载时不需要等待。
>
> 总体来说，这段代码通过查询NX装配中的WAVE组件，判断其是否已过时，并在UF_listing窗口中输出过时的组件名称，以提示用户进行更新。
>
