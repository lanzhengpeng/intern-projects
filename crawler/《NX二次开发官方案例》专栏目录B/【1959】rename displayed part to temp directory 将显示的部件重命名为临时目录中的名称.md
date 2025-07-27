### 【1959】rename displayed part to temp directory 将显示的部件重命名为临时目录中的名称

#### 代码

```cpp
    /*HEAD RENAME_DISPLAYED_PART_TO_TEMP_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文意，UF_print_syslog 是 V18 新增的函数。 */  
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
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        UF_CALL(uc4578(temp_name, 0, temp_name));  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            o_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            n_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_PART_ask_part_name(part, o_fspec));  
        build_similar_temp_filespec(o_fspec, 2, n_fspec);  
        UF_CALL(UF_PART_rename(part, n_fspec));  
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

> 这段代码是NX的二次开发代码，实现了将当前显示的部件临时重命名并移动到临时目录的功能。以下是代码的主要功能：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并在出错时打印错误信息。
> 3. 定义了report_error函数，用于打印错误信息。
> 4. 定义了build_similar_temp_filespec函数，用于构建临时文件路径。
> 5. 定义了do_it函数，用于实现重命名和移动部件的逻辑。
> 6. ufusr函数是NX二次开发程序的入口函数，调用do_it函数实现功能。
> 7. ufusr_ask_unload函数用于设置卸载模式。
>
> 通过调用NX的部件API，获取当前显示的部件名称，构建临时文件路径，调用rename函数实现重命名和移动。这段代码实现了将当前显示的部件临时重命名并移动到临时目录的功能。
>
