### 【1511】list all referenced parts 列出所有引用的部件

#### 代码

```cpp
    /*HEAD LIST_ALL_REFERENCED_PARTS CCC UFUN */  
    /*  
        There really is no easy way to get all of the parts which any given  
        part contains references to.  The ug_edit_part_names utility does  
        this nicely, so its use from an application is demonstrated here.  
        See ER 1800820.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_ugmgr.h>  
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
    static void write_file_contents_to_listing_window(char *fspec)  
    {  
        char  
            a_line[UF_UI_MAX_STRING_LEN+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL)  
                ECHO("%s", a_line);  
            fclose(the_file);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            cpn[MAX_FSPEC_SIZE+1],  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            tfn[MAX_FSPEC_SIZE+1],  
            *prog,  
            *shell;  
    #if defined(_WIN32)  
        prog = "%UGII_ROOT_DIR%\\ug_edit_part_names";  
        shell = "cmd.exe /c";  
    #else  
        prog = "$UGII_ROOT_DIR/ug_edit_part_names";  
        shell = "/bin/ksh -c";  
    #endif  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
        {  
            ECHO("This program only works in Native NX mode.\n");  
            return;  
        }  
        UF_CALL(UF_PART_ask_part_name(part, cpn));  
        build_unique_temp_name(tfn, 79);  
        sprintf(cmd, "%s \"\"%s\" -list \"%s\" > \"%s\"\"", shell, prog, cpn, tfn);  
        system(cmd);  
        write_file_contents_to_listing_window(tfn);  
        uc4561(tfn, 0);  
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

> 这段代码是NX的二次开发代码，其主要功能是获取当前显示的部件所引用的所有部件名称，并将其输出到窗口和日志中。代码的主要步骤包括：
>
> 1. 判断当前是否处于NX Manager模式，如果是则退出，因为此代码只能在原生NX模式下运行。
> 2. 获取当前显示部件的名称。
> 3. 生成一个临时文件路径。
> 4. 构造一个系统命令，调用NX提供的ug_edit_part_names工具，并将当前部件的名称作为参数传递，将输出重定向到临时文件。
> 5. 执行该系统命令，ug_edit_part_names工具会将当前部件引用的所有部件名称输出到临时文件。
> 6. 读取临时文件的内容，并将其输出到NX的列表窗口和日志文件。
> 7. 在结束时删除临时文件。
> 8. 通过ufusr函数调用实现NX的初始化、执行主要逻辑，以及终止。
> 9. 提供ufusr_ask_unload函数，以便立即卸载用户函数。
>
> 总体来说，这段代码利用NX提供的工具ug_edit_part_names，实现了获取部件引用关系的功能，并提供了友好的输出方式。
>
