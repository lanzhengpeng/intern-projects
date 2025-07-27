### 【1846】register function to assign new dataset name in nxmanager 在NX Manager中注册一个函数以分配新数据集名称

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <ctype.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <uf_exit.h>  
    #include <stdarg.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(const char *title, const char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static int get_integer_from_string_end(const char *str)  
    {  
        const char  
            *numStr = str + strlen( str );  
        while ( (numStr > str) && (isdigit( numStr[ -1 ] ))) --numStr;  
        return atoi( numStr );  
    }  
    DllExport int GTAC_generate_new_dataset_name(  
        UF_UGMGR_new_dataset_name_p_t dataset_name_info)  
    {  
        int  
            n;  
        ECHO("GTAC_generate_new_dataset_name:\n");  
        WRITE_S(dataset_name_info->owner);  
        WRITE_S(dataset_name_info->owner_revision);  
        WRITE_S(dataset_name_info->dataset_type);  
        WRITE_S(dataset_name_info->relation_type);  
        WRITE_S(dataset_name_info->basis_name);  
        WRITE_L(dataset_name_info->dataset_name_modifiable);  
        WRITE_S(dataset_name_info->dataset_name);  
        WRITE_D(dataset_name_info->ifail);  
    /*  Build your desired name here: 里海译:在这里构建你想要的名称。 */  
        n = get_integer_from_string_end(dataset_name_info->basis_name);  
        sprintf(dataset_name_info->dataset_name, "%s-GTAC%d",  
            dataset_name_info->owner, n);  
        return 0;  
    }  
    static void do_it(void)  
    {  
        ECHO("Registering callback for custom new dataset naming\n");  
        UF_CALL(UF_UGMGR_reg_new_dataset_name(GTAC_generate_new_dataset_name));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现了自定义新数据集名称的功能。代码的主要部分包括：
>
> 1. 宏定义：定义了ECHO、WRITE_D、WRITE_L、WRITE_S等宏，用于输出信息。
> 2. ECHO函数：用于输出信息到日志窗口。
> 3. GTAC_generate_new_dataset_name函数：实现了自定义新数据集名称的逻辑，通过获取基础名称的数字后缀，生成新数据集的名称。
> 4. do_it函数：注册了自定义数据集名称的回调函数。
> 5. ufsta和ufusr函数：NX提供的二次开发入口函数，在启动NX时会调用这些函数。在这两个函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
>
> 总的来说，这段代码通过自定义回调函数的方式，实现了在NX中创建新数据集时自定义数据集名称的功能。
>
