### 【2135】report full nx version including MP 报告完整的NX版本，包括多处理器

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
    }  
    static void do_it(void)  
    {  
        char  
            cmd[MAX_LINE_SIZE+1],  
            tver[MAX_LINE_SIZE+1],  
            tspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *temp;  
        build_unique_temp_name(tspec, 87);  
        sprintf(cmd, "env_print -m > %s", tspec);  
        system(cmd);  
        if ((temp = fopen(tspec, "r")) != NULL)  
        {  
            fgets(tver, MAX_LINE_SIZE, temp);  
            fclose(temp);  
            uc4561(tspec, 87);  
            strcpy(tver, strtok(tver, ","));  
        }  
        if (strlen(tver) < 5)  
        {  
            sprintf(cmd, "env_print -n > %s", tspec);  
            system(cmd);  
            if ((temp = fopen(tspec, "r")) != NULL)  
            {  
                fgets(tver, MAX_LINE_SIZE, temp);  
                fclose(temp);  
                uc4561(tspec, 87);  
            }  
        }  
        ECHO("%s\n", tver);  
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

> 这段NX二次开发代码主要实现以下功能：
>
> 1. 定义了一个ECHO宏，用于在NX的日志窗口打印信息。
> 2. 定义了一个UF_CALL宏，用于调用NX的函数，并检查函数返回码，如果失败则打印错误信息。
> 3. 定义了一个build_unique_temp_name函数，用于生成一个唯一的临时文件路径。
> 4. 定义了一个do_it函数，该函数执行以下操作：调用build_unique_temp_name生成临时文件路径。执行系统命令"env_print -m > tspec"，并将环境变量信息写入临时文件。读取临时文件的第一行，提取NX版本号，并打印。如果版本号太短，则执行系统命令"env_print -n > tspec"，再次打印环境变量信息。
> 5. 调用build_unique_temp_name生成临时文件路径。
> 6. 执行系统命令"env_print -m > tspec"，并将环境变量信息写入临时文件。
> 7. 读取临时文件的第一行，提取NX版本号，并打印。
> 8. 如果版本号太短，则执行系统命令"env_print -n > tspec"，再次打印环境变量信息。
> 9. 定义了ufusr函数，该函数是NX二次开发的入口函数，首先调用UF_initialize初始化，然后调用do_it函数执行上述功能，最后调用UF_terminate终止。
> 10. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示二次开发模块可以立即卸载。
>
> 总体来说，这段代码通过调用NX的API，实现了获取和打印NX版本号的功能。代码结构清晰，功能明确。
>
