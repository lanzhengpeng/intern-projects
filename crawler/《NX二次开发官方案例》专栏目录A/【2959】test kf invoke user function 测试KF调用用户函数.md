### 【2959】test kf invoke user function 测试KF调用用户函数

#### 代码

```cpp
    /*  
    This sample program is designed to test Knowledge Fusion invoking an Open C API program.  
    The corresponding dfa file to define the function to call from KF should look like this:  
     ----  
     #! NX/KF 6.0  
     Defun:  my_call_api(string ($libname, "this_lib_name"), string ($funcname, "kf_calls_api"))  
     @{ Cfunc("UF_KF_invoke_user_function"; "libufun"); } Integer;  
    ----  
    Change the "this_lib_name" above to the actual name of your dll.  Put the dll in any "application" folder, e.g.   
    UGII_USER_DIR/application and the dfa file above in any folder in your Preferences-> Knowledge Fusion Search Directories List.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    /* qq3123197280 */  
    extern "C" DllExport void kf_calls_api(UF_KF_value_t *data, int  
        num, UF_KF_value_t *rv)  
    {  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        ECHO("%s\n", "KF successfully called API function.");  
        UF_CALL(UF_KF_make_integer(0, rv));  
        UF_CALL(UF_terminate());  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        ECHO("%s\n", "Loaded API library successfully.");  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C API程序，用于演示如何从NX Knowledge Fusion中调用C函数。主要内容包括：
>
> 1. 函数原型定义：定义了C函数kf_calls_api，该函数将被Knowledge Fusion调用，以实现从Knowledge Fusion中调用C函数的功能。函数原型为extern “C” DllExport void kf_calls_api(UF_KF_value_t *data, int num, UF_KF_value_t *rv)。
> 2. 函数实现：kf_calls_api函数首先初始化NX Open，然后调用ECHO函数打印日志，最后使用UF_KF_make_integer创建一个整数类型的返回值，并终止NX Open。
> 3. ufusr函数：实现了NX Open的标准入口函数ufusr，用于加载API库并打印日志。
> 4. 卸载函数：实现了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载。
> 5. 错误处理：使用了宏UF_CALL来封装UF函数调用，以进行错误处理。若调用失败，将打印错误日志。
> 6. 日志函数：定义了ECHO函数，用于打印日志到NX的Listing Window和系统日志。
> 7. 说明注释：代码开头包含了对代码用途、使用方法和注意事项的说明注释。
>
> 综上所述，这段代码通过实现NX Open C API程序，展示了如何从Knowledge Fusion中调用C函数，并包含了错误处理、日志打印等辅助功能。
>
