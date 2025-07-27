### 【2364】report selected snap point 报告选择的捕捉点

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** report_selected_snap_point.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 里海译:Include files 翻译为：包含文件 */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[1025];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), 1024, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        double   
            location[3];  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 里海译:Failed to initialize的翻译是“初始化失败”。 */  
            return;  
        }  
        /* TODO: Add your application code here 里海译:在这里添加你的应用程序代码。 */  
        while( specify_point("Select an endpoint", location) == TRUE )  
        {  
            ECHO( "Point = %f, %f, %f\n", location[0], location[1], location[2]);  
        }  
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

> 这是段NX（Unigraphics）的二次开发代码，主要包括以下几个部分：
>
> 1. 包含文件：包含了必要的头文件，如stdarg.h、stdio.h等，以及NX提供的API头文件uf.h和uf_ui.h。
> 2. 宏定义：定义了UF_CALL宏，用于调用NX API函数，并自动处理错误。
> 3. 错误处理函数：定义了report_error函数，用于打印API调用错误信息。
> 4. 点选择函数：定义了specify_point函数，用于提示用户选择一个点，并返回点的坐标。
> 5. 主函数：定义了ufusr函数，这是NX二次开发的入口函数。在函数中，首先初始化NX API环境，然后循环调用specify_point函数让用户选择点，选择后打印点坐标，最后终止NX API环境。
> 6. 卸载处理函数：定义了ufusr_ask_unload函数，用于处理NX卸载本应用程序时的行为。
>
> 总体来说，这段代码实现了在NX中提示用户选择点，并打印所选点坐标的功能。通过调用NX提供的API函数，实现了二次开发。
>
