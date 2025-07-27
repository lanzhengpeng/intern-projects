### 【1946】report all part attributes 报告所有零件属性

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_attr.h>  
    #include <uf_part.h>  
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
    static void report_object_attributes(char *name, tag_t object)  
    {  
        int  
            indx = 0,  
            n;  
        char  
            dstr[21],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_any, &n));  
        ECHO("%s (%d) has %d attributes\n", name, object, n);  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && (indx != 0))  
        {  
            switch ( att.type )  
            {  
                case UF_ATTR_integer:  
                    ECHO( "%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    ECHO( "%s = %f (FLOATING POINT)\n", title, att.value.real);  
                break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(att.value.time, 2, dstr, tstr));  
                    ECHO( "%s = %s %s (DATE & TIME)\n", title, dstr, tstr);  
                break;  
                case UF_ATTR_null:  
                    ECHO( "%s = (NULL)\n", title);  
                break;  
                case UF_ATTR_string:  
                    ECHO( "%s = %s (STRING)\n", title, att.value.string);  
                    UF_free(att.value.string);  
                break;  
                case UF_ATTR_reference:  
                    ECHO( "%s = %s (REFERENCE)\n", title, att.value.reference);  
                    UF_free(att.value.reference);  
                break;  
                default:  
                break;  
            }  
        }  
        ECHO("\n");  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        report_object_attributes("part", part);  
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

> 这段代码是NX的二次开发代码，主要功能是遍历当前显示零件的所有属性并打印出来。以下是代码的主要逻辑：
>
> 1. 定义了ECHO和UF_CALL宏，用于打印信息和调用NX API，同时报告错误。
> 2. 定义了report_error函数，用于在调用NX API出错时打印错误信息。
> 3. 定义了report_object_attributes函数，用于遍历对象的所有属性并打印。属性类型包括整型、浮点型、日期时间型、字符串型、引用等。
> 4. 定义了do_it函数，用于获取当前显示的零件对象，并调用report_object_attributes打印其所有属性。
> 5. ufusr函数是二次开发的入口函数，初始化NX后调用do_it函数，然后终止NX。
> 6. ufusr_ask_unload函数用于立即卸载二次开发程序。
>
> 总体来说，这段代码实现了遍历当前显示零件的所有属性并打印的功能，使用了NX的UF_PART和UF_ATTR相关API。
>
