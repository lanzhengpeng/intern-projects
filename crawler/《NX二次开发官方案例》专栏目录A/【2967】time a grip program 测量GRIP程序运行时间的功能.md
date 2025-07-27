### 【2967】time a grip program 测量GRIP程序运行时间的功能

#### 代码

```cpp
    /*HEAD TIME_A_GRIP_PROGRAM CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数，因此只需要提供翻译，不需要添加其他无关内容。

翻译如下：
注意：UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static logical prompt_for_grip_executable_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *grip_dir,  
            *grip_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_GRIP_DIR, &grip_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_FILE_GRIP_FLTR, &grip_ext));  
        combine_directory_and_wildcard(grip_dir, grip_ext, filter);  
        UF_free(grip_dir);  
        UF_free(grip_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "GRIP Executable", filter, "",  
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
            grip_prog[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            times;  
        if (prompt_for_grip_executable_file_name("Time Run of GRIP program",  
            grip_prog))  
        {  
            UF_CALL(UF_begin_timer(&timer));  
            UF_CALL(UF_call_grip(grip_prog, 0, NULL));  
            UF_CALL(UF_end_timer(timer, &times));  
            WRITE_S(grip_prog);  
            WRITE_F(times.cpu_time);  
            WRITE_F(times.real_time);  
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

> 这是段用于NX二次开发的代码，主要功能是调用GRIP程序并测量其执行时间。以下是代码的主要组成部分：
>
> 1. 包含头文件：包含NX提供的头文件，用于调用NX的函数。
> 2. 错误报告函数：用于捕获并报告函数调用错误。
> 3. 组合目录和通配符：用于组合目录和通配符，生成文件选择过滤条件。
> 4. 提示用户输入GRIP程序文件名：用于提示用户输入GRIP程序的文件名，并获取用户选择的文件名。
> 5. 向列表窗口写入字符串和浮点数：用于向NX的列表窗口写入提示信息和执行时间。
> 6. 执行测量：用于调用GRIP程序，并测量其执行时间。
> 7. ufusr函数：NX二次开发程序的入口点，用于初始化NX环境，调用执行函数，然后终止NX环境。
> 8. 卸载函数：用于立即卸载二次开发程序。
>
> 总的来说，这段代码实现了调用GRIP程序并测量其执行时间，将结果输出到NX的列表窗口。
>
