### 【1436】report current ug version 报告当前UG版本

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

> 这段代码是一个NX Open应用程序，主要用于获取NX的版本信息，并打印到控制台和NX日志窗口中。
>
> 主要功能包括：
>
> 1. 定义了一个ECHO宏，用于将信息打印到控制台和NX日志窗口。
> 2. 定义了UF_CALL宏，用于调用NX API，并报告错误。
> 3. 定义了WRITE_F、WRITE_S、WRITE_L宏，用于打印浮点数、字符串和布尔值。
> 4. 实现了ask_full_nx_version函数，用于获取NX的版本号，并返回版本号的小数形式。
> 5. 实现了do_it函数，用于获取NX版本号，并打印到控制台和日志窗口。
> 6. 实现了ufusr和main函数，分别用于NX Open和独立应用程序的入口点。
>
> 通过调用UF_ask_system_info获取系统信息，并解析出NX的版本号。然后，使用定义的宏打印版本号信息。整体流程简单清晰，通过宏封装简化了代码。
>
