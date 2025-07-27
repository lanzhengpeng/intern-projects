### 【2524】save as 将文件另存为

#### 代码

```cpp
    /*HEAD SAVE_AS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_exit.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新增功能，用于在用户界面打印系统日志。 */  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    void ufsvas(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        build_copy_partname(param);  
        *retcode = 2;  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是NX Open C++开发的二次开发代码，主要功能是复制当前显示部件的文件到新文件名，并返回新文件路径。
>
> 代码主要包含以下部分：
>
> 1. 头文件包含：包含必要的NX Open C++库头文件。
> 2. UF_CALL宏定义：定义了一个宏，用于调用NX Open C++函数，并检查返回码，如果出错则打印错误信息。
> 3. report_error函数：用于打印NX Open C++函数的错误信息。
> 4. build_copy_partname函数：获取当前显示部件的文件路径，构建新文件路径，并复制当前部件到新文件路径。
> 5. ufsvas函数：初始化NX Open C++环境，调用build_copy_partname函数，设置返回码，然后终止NX Open C++环境。
> 6. ufusr_ask_unload函数：返回卸载类型，表示立即卸载二次开发应用程序。
>
> 通过以上代码，可以实现复制当前显示部件到新文件的功能，并返回新文件路径。
>
