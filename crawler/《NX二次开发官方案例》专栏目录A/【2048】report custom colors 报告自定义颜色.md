### 【2048】report custom colors 报告自定义颜色

#### 代码

```cpp
    /* Include files 里海译:Include files 翻译为：包含文件 */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_cgm.h>  
    #include <uf_plot.h>  
    #include <uf_ui.h>  
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
    static void report_custom_colors(void)  
    {  
        UF_CGM_custom_colors_t   
            custom_colors;  
        UF_CGM_color_t  
            col;  
        int i;  
        char szColor[132];  
        UF_UI_open_listing_window();  
        // usage of the Cgm variant  
        UF_UI_write_listing_window("\nUF_CGM_ask_default_custom_colors\n");  
        UF_CALL( UF_CGM_ask_default_custom_colors( &custom_colors ) );  
        for( i=0; i<216; i++)  
        {  
            col = custom_colors.colors[i];  
            sprintf( szColor, "%d, %s, %f, %f, %f, %d\n",  
                col.clr_index, col.clr_name,   
                col.clr_vals[0], col.clr_vals[1], col.clr_vals[2],  
                col.favorite_index);  
            UF_UI_write_listing_window(szColor);  
        }  
        UF_UI_write_listing_window("\nUF_CGM_ask_session_custom_colors\n");  
        UF_CALL( UF_CGM_ask_session_custom_colors( &custom_colors ) );  
        for( i=0; i<216; i++)  
        {  
            col = custom_colors.colors[i];  
            sprintf( szColor, "%d, %s, %f, %f, %f, %d\n",  
                col.clr_index, col.clr_name,   
                col.clr_vals[0], col.clr_vals[1], col.clr_vals[2],  
                col.favorite_index);  
            UF_UI_write_listing_window(szColor);  
        }  
        // usage of the Plot variant  
        UF_UI_write_listing_window("\nUF_PLOT_ask_default_custom_colors\n");  
        UF_CALL( UF_PLOT_ask_default_custom_colors( &custom_colors ) );  
        for( i=0; i<216; i++)  
        {  
            col = custom_colors.colors[i];  
            sprintf( szColor, "%d, %s, %f, %f, %f, %d\n",  
                col.clr_index, col.clr_name,   
                col.clr_vals[0], col.clr_vals[1], col.clr_vals[2],  
                col.favorite_index);  
            UF_UI_write_listing_window(szColor);  
        }  
        UF_UI_write_listing_window("\nUF_PLOT_ask_session_custom_colors\n");  
        UF_CALL( UF_PLOT_ask_session_custom_colors( &custom_colors ) );  
        for( i=0; i<216; i++)  
        {  
            col = custom_colors.colors[i];  
            sprintf( szColor, "%d, %s, %f, %f, %f, %d\n",  
                col.clr_index, col.clr_name,   
                col.clr_vals[0], col.clr_vals[1], col.clr_vals[2],  
                col.favorite_index);  
            UF_UI_write_listing_window(szColor);  
        }  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 里海译:Failed to initialize */  
            return;  
        }  
        /* TODO: Add your application code here 里海译:翻译TODO: 在此处添加您的应用程序代码 */  
        report_custom_colors();  
        /* Terminate the API environment 里海译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，主要实现了以下功能：
>
> 1. 包含文件：代码包含了一些NX Open C++的库文件，如stdio.h, uf.h, uf_cgm.h等。
> 2. 错误报告函数：定义了一个report_error函数，用于报告函数调用时的错误代码、错误消息和调用语句。
> 3. 自定义颜色报告函数：定义了一个report_custom_colors函数，用于报告NX默认自定义颜色和会话自定义颜色。
> 4. 显式激活方法：定义了ufusr函数，作为程序的入口点。在这个函数中，先初始化NX Open C++环境，然后调用report_custom_colors函数，最后终止NX Open C++环境。
> 5. 卸载处理函数：定义了ufusr_ask_unload函数，用于指定何时从NX中卸载该应用程序。此函数返回UF_UNLOAD_IMMEDIATELY，表示应用程序可以立即被卸载。
>
> 综上所述，该代码的主要功能是获取NX的自定义颜色信息，并通过列表窗口显示出来。通过调用NX Open C++的UF_CGM和UF_PLOT函数实现此功能。
>
