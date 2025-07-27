### 【1067】disallow using certain parts as components 禁止使用某些零件作为组件

#### 代码

```cpp
    /*  This program demonstrates using the "Add Existing Part" user exit to  
        prevent the user from adding certain parts as components to their assembly.  
        set USER_RCOMP=<path to the shared library>  
        This works well in both Native mode and when running NX with Teamcenter  
        (aka TCIN or NX Manager mode).  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_attr.h>  
    #include <uf_part.h>  
    #include <uf_exit.h>  
    #include <stdarg.h>  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static void do_it(char *param, int *retcode)  
    {  
        logical  
            unused;  
        int  
            resp;  
        tag_t  
            part = NULL_TAG;  
        char  
            attr_value[UF_ATTR_MAX_STRING_LEN+1] = { "" },  
            file_name[MAX_FSPEC_SIZE+1] = { "" };  
        UF_PART_load_status_t  
            status;  
        UF_ATTR_value_t  
            value;  
        value.value.string = attr_value;  
        UF_CALL(UF_UI_ask_open_part_filename(file_name, &unused, &resp));  
        if (resp == UF_UI_CANCEL) return;  
        if (!UF_PART_is_loaded(file_name))  
        {  
            UF_CALL(UF_PART_open_quiet(file_name, &part, &status));  
            report_load_status(&status);  
        }  
        else  
            part = UF_PART_ask_part_tag(file_name);  
        if (part == NULL_TAG) return;  
    /*  Here the application would verify something in the part to decide  
        if it is allowed to be a component or not.  In this case we are  
        checking the value of a specific attribute.  
    */  
        UF_CALL(UF_ATTR_read_value(part, "STATUS", UF_ATTR_string, &value));  
        if (!strcmp(value.value.string, "OK"))  
        {  
            strcpy(param, file_name);  
            *retcode = 3;  
        }  
        else  
        {  
            UF_CALL(UF_PART_close(part, 0, 1));  
            uc1601("That part cannot be used as a component!", TRUE);  
        }  
    }  
    void ufrcp(char *param, int *retcode, int rlen)  
    {  
        *retcode = 1;  
        if (UF_CALL(UF_initialize())) return;  
        do_it(param, retcode);  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open的二次开发示例，用于演示如何使用"Add Existing Part"用户出口来限制用户在装配中添加某些零件。
>
> 主要功能包括：
>
> 1. 定义了一个ECHO函数，用于输出信息到日志窗口和系统日志。
> 2. 定义了一个REPORT_ERROR函数，用于输出函数调用失败的错误信息。
> 3. 定义了一个REPORT_LOAD_STATUS函数，用于输出零件加载状态信息。
> 4. 定义了一个DO_IT函数，用于执行以下操作：获取用户要添加的零件文件名。如果零件尚未加载，则加载零件并输出加载状态。读取零件的"STATUS"属性。如果属性值为"OK"，则允许添加，否则不允许，并输出提示信息。
> 5. 获取用户要添加的零件文件名。
> 6. 如果零件尚未加载，则加载零件并输出加载状态。
> 7. 读取零件的"STATUS"属性。
> 8. 如果属性值为"OK"，则允许添加，否则不允许，并输出提示信息。
> 9. 定义了UFRCP函数，这是用户出口函数，用于初始化NX Open并调用DO_IT函数。
> 10. 定义了UFUSR_ASK_UNLOAD函数，用于设置退出时立即卸载。
>
> 该代码的主要目的是通过读取零件的特定属性，来控制是否允许该零件被添加到装配中，从而实现设计规范的控制。
>
