### 【0473】check if specified api function exists 检查指定的API函数是否存在

#### 代码

```cpp
    /*HEAD CHECK_IF_SPECIFIED_API_FUNCTION_EXISTS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数。

UF_print_syslog 是 V18 版本新增的函数，用于打印系统日志。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        char  
            function_name[MAX_FSPEC_SIZE+1] = { "" },  
            full_path[MAX_FSPEC_SIZE+1],  
            *root_dir;  
        UF_load_f_p_t  
            f_ptr;  
        UF_CALL(UF_translate_variable("UGII_ROOT_DIR", &root_dir));  
        uc4575(root_dir, 0, "libufun.dll", full_path);  
        WRITE("Checking - ");  
        WRITE(full_path);  
        WRITE("\n");  
        while (prompt_for_text("API function to check", function_name))  
        {  
            if (!UF_CALL(UF_load_library(full_path, function_name, &f_ptr)))  
            {  
                UF_CALL(UF_unload_library(full_path));  
                WRITE("Found ");  
            }  
            WRITE(function_name);  
            WRITE("\n");  
        }  
    }  
    /* 里海 */  
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

> 这段代码是NX Open C++的二次开发示例，用于检测指定API函数是否存在。
>
> 主要功能包括：
>
> 1. 使用UF_print_syslog函数打印错误日志信息，该函数在NX 18版本中新增。
> 2. 提示用户输入要检测的API函数名，并调用UF_load_library函数来检测该函数是否存在于指定库文件中。
> 3. 使用UF_unload_library函数卸载指定的库文件。
> 4. 在ufusr函数中初始化NX Open C++环境，调用do_it函数执行检测逻辑，然后终止NX Open C++环境。
> 5. 定义ufusr_ask_unload函数，返回立即卸载标志，以便在卸载NX Open C++库时调用。
>
> 通过这段代码，用户可以方便地检测NX Open C++库文件中是否存在指定的API函数。
>
