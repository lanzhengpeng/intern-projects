### 【2408】report ugmgr part tag extern report ugmgr part tag extern

#### 代码

```cpp
    /* Include files 里海译:Include files */  
    #include <strstream>  
    #include <iostream>  
    #include <string>  
    using std::ostrstream;  
    using std::endl;      
    using std::cout;  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[256];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        cout << msg << endl;  
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
    static void do_it(const char *param)  
    {  
        char  
            itemid[UF_UGMGR_NAME_SIZE+1] = "";  
        UF_UGMGR_tag_t   
            db_tag = NULL_TAG;   
        strcpy_s(itemid, UF_UGMGR_NAME_SIZE, param);  
        UF_CALL( UF_UGMGR_ask_part_tag(itemid, &db_tag) );  
        ECHO("UF_UGMGR_ask_part_tag returned the DB Tag <%d>\n", db_tag);  
    }  
    extern void main( char argc, const char *argv[] )  
    {  
        if( argc < 2 )  
        {  
            ECHO("USAGE: %s <ItemID>\n", argv[0]);  
            return;  
        }  
        if (!UF_CALL(UF_UGMGR_initialize(argc, argv)))  
        {  
            do_it(argv[1]);  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段代码是基于NX的二次开发代码，其主要功能是获取指定Item ID的零件在NX数据库中的Tag号。代码的流程如下：
>
> 1. 引入了必要的头文件，包括C++标准库头文件、NX提供的UF头文件等。
> 2. 定义了一个ECHO宏，用于打印信息到控制台和NX日志。
> 3. 定义了一个report_error函数，用于在UF函数调用失败时打印错误信息。
> 4. 定义了do_it函数，该函数接收一个参数，即Item ID，并调用UF_UGMGR_ask_part_tag函数获取对应的DB Tag，然后打印出来。
> 5. 在main函数中，首先检查命令行参数，如果小于2，则打印使用说明并退出。然后调用UF_UGMGR_initialize函数初始化NX环境，调用do_it函数执行主要功能，最后调用UF_terminate终止NX环境。
>
> 总体来说，这段代码实现了根据Item ID查询零件DB Tag的基本功能，并使用了错误处理机制，结构清晰，代码质量较高。
>
