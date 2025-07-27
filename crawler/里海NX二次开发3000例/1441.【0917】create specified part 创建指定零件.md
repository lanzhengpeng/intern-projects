### 【0917】create specified part 创建指定零件

#### 代码

```cpp
    /*HEAD CREATE_SPECIFIED_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18版本新增的功能。 */  
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
    static void do_it(void)  
    {  
        int  
            resp,  
            units = UF_PART_ENGLISH;  
        tag_t  
            new_part;  
        char  
            filename[MAX_FSPEC_SIZE+1] = { "" };  
    /*  This fails in NXManager mode - see PR 5757620 里海译:翻译：在NXManager模式下失败 - 参见PR 5757620 */  
        UF_CALL(UF_UI_create_part(NULL, filename, &units, &new_part, &resp));  
    }  
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

> 这段代码是一个NX Open C++程序，主要功能是创建一个新部件。以下是对代码的详细介绍：
>
> 1. 包含头文件：代码包含了必要的头文件，如stdio.h、string.h、uf.h和uf_ui.h，用于执行标准输入输出操作、字符串操作、UF函数和UF用户界面函数。
> 2. 错误报告函数：定义了一个report_error函数，用于在UF函数调用失败时报告错误，包括错误码、错误信息、调用失败的函数名等。
> 3. 创建部件函数：定义了一个do_it函数，用于调用UF_UI_create_part函数创建新部件。该函数指定了部件的文件名、单位制和响应码。
> 4. UFusr函数：定义了ufusr函数，这是NX Open C++程序的入口函数。首先初始化UF系统，然后调用do_it函数创建部件，最后终止UF系统。
> 5. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载的标志，表示该程序在关闭时会立即卸载。
> 6. 创建部件：在do_it函数中，调用UF_UI_create_part函数创建一个新部件，文件名为空字符串，单位制为英制，响应码存储在resp变量中。
> 7. 错误处理：在创建部件时，使用UF_CALL宏封装UF函数调用，以报告错误。如果创建部件失败，将输出错误信息。
> 8. 日志和窗口输出：在report_error函数中，错误信息会输出到系统日志和NX打开的列表窗口中。
>
> 总体来说，这段代码的目的是创建一个新部件，并具有完善的错误处理机制，以输出错误信息。
>
