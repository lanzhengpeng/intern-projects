### 【1764】print text file to info window 打印文本文件到信息窗口

#### 代码

```cpp
    /*HEAD PRINT_TEXT_FILE_TO_INFO_WINDOW CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。

翻译为：UF_print_syslog是V18版本新增的函数，请只回答译文，不要添加其他无关内容。 */  
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
            resp;  
        char  
            a_line[MAX_LINE_SIZE+2],  
            filter[MAX_FSPEC_SIZE+1] = { "*.txt" },  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        FILE  
            *the_file;  
        while (!UF_UI_create_filebox("Choose text file", "Show contents", filter,  
            "", fspec, &resp) && (resp != UF_UI_CANCEL))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            if ((the_file = fopen(fspec, "r")) != NULL)  
            {  
                while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL)  
                    UF_CALL(UF_UI_write_listing_window(a_line));  
                fclose(the_file);  
            }  
        }  
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

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是从用户指定的文本文件中读取内容并显示在NX的信息窗口中。具体介绍如下：
>
> 1. 文件包含：引入了stdio.h、stdlib.h、string.h、uf.h和uf_ui.h头文件，分别用于C标准输入输出、标准库、字符串操作、NX的UF函数库和UI函数库。
> 2. 宏定义：使用UF_CALL宏来调用UF函数，并在函数调用失败时报告错误。
> 3. 错误报告函数：定义了report_error函数，用于在UF函数调用失败时报告错误信息。
> 4. 主逻辑函数：定义了do_it函数，用于实现主逻辑。
> 5. 文件选择：使用UF_UI_create_filebox函数让用户选择文本文件。
> 6. 文件读取和显示：打开用户选择的文件，逐行读取内容，并使用UF_UI_write_listing_window函数显示在NX的信息窗口中。
> 7. UF初始化和终止：在ufusr函数中调用UF_initialize进行初始化，在结束时调用UF_terminate进行终止。
> 8. 卸载提示：定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载该UF。
>
> 总体来说，这段代码通过NX的UF和UI函数实现了读取文本文件并显示内容的功能，利用了NX提供的二次开发接口。
>
