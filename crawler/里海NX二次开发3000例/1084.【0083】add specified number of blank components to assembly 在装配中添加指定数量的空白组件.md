### 【0083】add specified number of blank components to assembly 在装配中添加指定数量的空白组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n = 10;  
        double  
           origin[3] = {0.0, 0.0, 0.0},  
           csys[6] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};  
        tag_t  
            part =  UF_PART_ask_display_part(),  
            instance;  
        char  
            fname[MAX_FSPEC_SIZE+1],  
            dspec[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1];  
        if (prompt_for_an_integer("Create new empty components", "How many", &n))  
        {  
            UF_CALL(UF_PART_ask_part_name(part, fspec));  
            uc4576(fspec, 2, dspec, fname);  
            for (ii = 0; ii < n; ii++)  
            {  
                uc4577(fname);  
                uc4575(dspec, 2, fname, fspec);  
                ECHO("Adding %s\n", fspec);  
                UF_CALL(UF_ASSEM_create_component_part(part, fspec, NULL, NULL,  
                    UF_PART_ENGLISH, 0, origin, csys, 0, NULL, &instance));  
            }  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX二次开发的示例，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于打印信息到NX的日志窗口和系统日志。
> 2. 定义了一个report_error函数，用于在函数调用失败时报告错误信息。
> 3. 定义了一个prompt_for_an_integer函数，用于弹出NX的对话框，提示用户输入一个整数，并返回用户输入的值。
> 4. do_it函数实现了以下功能：获取当前显示的零件part。获取part的名称，并构建新零件的文件名。循环创建指定数量的新零件，并添加到装配中。
> 5. 获取当前显示的零件part。
> 6. 获取part的名称，并构建新零件的文件名。
> 7. 循环创建指定数量的新零件，并添加到装配中。
> 8. ufusr函数是NX二次开发的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
>
> 整体来看，这段代码实现了在NX中创建指定数量的新零件并添加到装配的功能。使用了NX提供的API，通过对话框获取用户输入，利用零件名称构建新零件的文件名，并调用API函数创建新零件和添加到装配。
>
