### 【1433】report current full nx version 报告当前完整NX版本

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
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
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void strip_extra_decimal_points(char *s)  
    {  
        unsigned int  
            dots = 0,  
            ii,  
            jj;  
        for (ii = 0; ii < strlen(s); ii++)  
        {  
            if (s[ii] == '.')  
            {  
                dots++;  
                if (dots > 1)  
                    for (jj = ii; jj < strlen(s); jj++)  
                        s[jj] = s[jj+1];  
            }  
        }  
    }  
    // This returns a double so that you can use > or < for comparisons  
    static double ask_full_nx_version(char **version)  
    {  
        double  
            rel_no;  
        UF_system_info_t  
            info;  
        UF_CALL(UF_ask_system_info(&info));  
        UF_CALL(allocate_memory(  
            (unsigned int)(sizeof(char)*(strlen(info.program_name)+1)),  
            (void **)version));  
        strcpy(*version, info.program_name);  
        strip_extra_decimal_points(info.program_name);  
        rel_no = atof(strrchr(info.program_name, 'X')+1);  
        UF_CALL(UF_free_system_info(&info));  
        return rel_no;  
    }  
    static void do_it(void)  
    {  
       double  
            f_ver;  
        char  
            *s_ver = NULL;  
        f_ver = ask_full_nx_version(&s_ver);  
        WRITE_S(s_ver);  
        WRITE_F(f_ver);  
        WRITE_L(f_ver > 8.0);  
        if (s_ver != NULL) UF_free(s_ver);  
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
    int main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，用于查询NX系统信息，包括NX版本，并打印相关信息。
>
> 主要功能包括：
>
> 1. 定义了一个ECHO宏，用于将信息输出到列表窗口和系统日志。
> 2. 定义了一个UF_CALL宏，用于调用NX API函数，并在出错时打印错误信息。
> 3. ask_full_nx_version函数用于查询NX版本信息，并返回版本号。
> 4. do_it函数用于执行主要功能，包括获取NX版本，打印版本字符串和浮点数，以及比较版本号。
> 5. ufusr函数是NX调用的主函数，用于初始化NX，执行do_it函数，然后终止NX。
> 6. main函数用于在命令行运行时执行上述功能。
> 7. ufusr_ask_unload函数用于在NX卸载应用程序时的清理工作。
>
> 总的来说，这段代码实现了查询和打印NX版本的功能，并包含了错误处理和内存管理等辅助功能。
>
