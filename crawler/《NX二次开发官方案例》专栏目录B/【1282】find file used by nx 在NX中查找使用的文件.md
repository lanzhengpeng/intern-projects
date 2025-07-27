### 【1282】find file used by nx 在NX中查找使用的文件

#### 代码

```cpp
    /*  
        This is a useful utility when an NX session seems to be picking up the  
        wrong copy of a specific file, e.g. a dlg, dlx, dfa, etc.  
        It will search for the specified file in the specified subfolder of  
        the parent folders listed in UGII_UG_CUSTOM_DIRECTORY_FILE and  
        UGII_CUSTOM_DIRECTORY_FILE and return the full path to the first  
        copy it finds.  
    */  
    #include <stdio.h>  
    #include <string.h>  
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
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 里海译:Back的中文翻译是“背部”。 */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:Cancel 的翻译是“取消”。 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:选了一个。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void do_it(void)  
    {  
        int  
            which = 2;  
        char  
            filename[MAX_FSPEC_SIZE+1] = { "" },  
            subdir[MAX_FSPEC_SIZE+1] = { "" },  
            *subopts[5] = { "startup", "application", "dfa", "udf", "Other" },  
            *path;  
        which = choose_one_item("What subdirectory?", subopts, 5);  
        if (which == 4) prompt_for_text("Enter subdirectory name", subdir);  
        else strcpy(subdir, subopts[which]);  
        if (prompt_for_text("Enter file name", filename))  
        {  
            UF_CALL(UF_find_file(subdir, filename, &path));  
            WRITE_S(path);  
            if (path) UF_free(path);  
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

> 这段代码是一个NX Open应用程序，主要功能是搜索指定文件夹中的文件。以下是代码的主要功能介绍：
>
> 1. 错误处理：通过宏UF_CALL封装了NX API调用，用于记录错误信息。
> 2. 目录选择：使用choose_one_item函数实现目录选择，支持后退、取消和加载更多选项。
> 3. 文本输入：通过prompt_for_text函数获取用户输入的文件名和子目录名。
> 4. 文件查找：使用NX API UF_find_file在指定子目录下查找文件，并返回文件路径。
> 5. 路径打印：使用WRITE_S宏将找到的文件路径打印到列表窗口和系统日志。
> 6. 内存释放：在找到文件后，使用UF_free释放NX分配的内存。
> 7. 初始化和终止：在ufusr函数中调用NX初始化和终止。
> 8. 卸载处理：通过ufusr_ask_unload函数实现立即卸载。
>
> 总体来说，这段代码实现了一个简单的文件搜索功能，适用于NX环境下的二次开发。
>
