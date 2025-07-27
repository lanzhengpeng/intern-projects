### 【2973】toggle minimal window mode 切换最小窗口模式

#### 代码

```cpp
    锘/* Include files 里海译:翻译为：包含文件 */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui_types.h>  
    #include <uf_ui.h>  
    static void ECHO(char* format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        //printf(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char* file, int line, char* call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    void do_it()  
    {  
        int left = 10,  
            top = 10,  
            right = 800,  
            bottom = 600;  
        logical  
            bIsMini = false;  
        UF_CALL(UF_UI_ask_minimal_graphics_window(&bIsMini));  
        if (!bIsMini)  
        {  
            UF_CALL(UF_UI_set_minimal_graphics_window(true));  
            UF_CALL(UF_UI_set_minimal_graphics_window_location(left, top, right, bottom));  
        }  
        else  
            UF_CALL(UF_UI_set_minimal_graphics_window(false));  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr(char* parm, int* returnCode, int rlen)  
    {  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if (UF_CALL(UF_initialize()))  
        {  
            /* Failed to initialize 里海译:Failed to initialize翻译为：初始化失败 */  
            return;  
        }  
        do_it();  
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
    extern int ufusr_ask_unload(void)  
    {  
      // Unloads the image when NX session terminates  
        return(UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码的功能和注释，这是一个UG/NX二次开发示例，主要功能是控制最小化图形窗口的显示和位置。
>
> 主要步骤包括：
>
> 1. 调用UF_UI_ask_minimal_graphics_window函数，判断当前是否已经是最小化图形窗口模式。
> 2. 如果不是最小化模式，则调用UF_UI_set_minimal_graphics_window将窗口设置为最小化模式。
> 3. 设置最小化窗口的位置，调用UF_UI_set_minimal_graphics_window_location函数。
> 4. 如果已经是最小化模式，则调用UF_UI_set_minimal_graphics_window将其关闭。
> 5. 使用宏UF_CALL包装UF函数调用，以报告错误。
> 6. ufusr函数作为二次开发的入口点，初始化API环境，执行主功能do_it，然后关闭API环境。
> 7. ufusr_ask_unload函数用于卸载二次开发应用程序。
>
> 这段代码实现了对NX最小化图形窗口模式的控制，通过简单的逻辑判断和UF函数调用即可实现。
>
