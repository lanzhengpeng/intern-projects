### 【2989】translate custom strings translate custom strings

#### 代码

```cpp
    /*HEAD TRANSLATE_CUSTOM_STRINGS CCC UFUN */  
    /*   
    Content of "c:\\custom_strings_german.lng":  
     'Command one' = 'Kommando eins'  
     'Command two' = 'Kommando zwei'  
     'Command three' = 'Kommando drei'  
    */  
    /* Include files 里海译:Include files 翻译为：包含文件 */  
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_text.h>  
    #include <uf_ui.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        char   
            *xstring,  
            *input_strings[] = {"Open", "Command one", "Command two", "Command three" };  
        int  
            ii;  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 里海译:Failed to initialize的翻译为：初始化失败。 */  
            return;  
        }  
        /* TODO: Add your application code here 里海译:翻译为：

TODO: 在此添加您的应用程序代码 */  
        UF_CALL( UF_TEXT_init_native_lang_support() );  
        UF_CALL( UF_TEXT_load_translation_file("c:\\custom_strings_german.lng") );  
        for( ii=0; ii<4; ii++ )  
        {  
            UF_CALL( UF_TEXT_translate_string_2(input_strings[ii], &xstring) );  
            ECHO("%s = %s\n", input_strings[ii], xstring);  
            UF_free(xstring);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 初始化和终止API环境：使用UF_initialize()和UF_terminate()函数分别初始化和终止NX的API环境。
> 2. 加载翻译文件：通过UF_TEXT_load_translation_file()函数加载了自定义的德语翻译文件custom_strings_german.lng，其中包含了一些自定义的字符串翻译。
> 3. 字符串翻译：使用UF_TEXT_translate_string_2()函数，将英语字符串翻译为德语字符串，并打印输出。
> 4. 错误处理：通过UF_CALL宏包装了NX API调用，在出错时打印错误信息，以便调试。
> 5. 窗口输出：使用UF_UI_open_listing_window()和UF_UI_write_listing_window()函数，在NX的列表窗口中输出信息。
> 6. 卸载处理：提供了ufusr_ask_unload()函数，以便在卸载二次开发应用程序时调用。
>
> 总体来说，这段代码展示了如何在NX中实现简单的字符串翻译功能，同时提供了错误处理和日志输出机制。
>
