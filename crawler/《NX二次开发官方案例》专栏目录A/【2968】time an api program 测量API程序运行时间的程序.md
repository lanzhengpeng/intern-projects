### 【2968】time an api program 测量API程序运行时间的程序

#### 代码

```cpp
    /*HEAD TIME_AN_API_PROGRAM CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中新增的函数。以下是翻译：

请注意，UF_print_syslog 是 V18 新增的函数。 */  
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
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_api_executable_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *ufun_dir,  
            *ufun_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_UFUN_DIR, &ufun_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_FILE_UFUN_FLTR, &ufun_ext));  
        combine_directory_and_wildcard(ufun_dir, ufun_ext, filter);  
        UF_free(ufun_dir);  
        UF_free(ufun_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "API Executable", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        char  
            ufun_prog[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            stop_times;  
        UF_load_f_p_t  
            go;  
        if (prompt_for_api_executable_file_name("Time Run of api program",  
            ufun_prog) &&  
            !UF_CALL(UF_load_library(ufun_prog, "ufusr", (UF_load_f_p_t *)&go)))  
        {  
            UF_CALL(UF_begin_timer(&timer));  
            go();  
            UF_CALL(UF_end_timer(timer, &stop_times));  
            UF_UI_close_listing_window();  
            WRITE_S(ufun_prog);  
            WRITE_F(stop_times.cpu_time);  
            WRITE_F(stop_times.real_time);  
            UF_CALL(UF_unload_library(ufun_prog));  
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

> 这是NX的二次开发代码，主要功能是测量API程序的运行时间。以下是代码的主要部分：
>
> 1. 头文件：包含了NX二次开发所需的头文件，如stdio.h、string.h、uf.h等。
> 2. 错误处理宏：UF_CALL宏用于调用NX API，并在出错时报告错误。
> 3. 目录和通配符组合函数：combine_directory_and_wildcard用于组合目录名和通配符，用于后续的文件对话框。
> 4. 提示选择API可执行文件：prompt_for_api_executable_file_name用于提示用户选择API可执行文件，并返回文件路径。
> 5. 写信息到列表窗口：write_string_to_listing_window/write_double_to_listing_window用于在列表窗口中输出字符串或浮点数。
> 6. 执行API程序并计时：do_it函数加载用户选择的API程序，运行它，并输出运行时间。
> 7. UFusr函数：这是二次开发的入口函数，初始化NX环境，调用do_it函数，然后终止环境。
> 8. 卸载函数：ufusr_ask_unload用于控制API程序的卸载方式。
>
> 综上，这段代码的主要功能是提示用户选择一个NX API程序，运行它，并输出其运行时间，主要用于测量API程序的效率。
>
