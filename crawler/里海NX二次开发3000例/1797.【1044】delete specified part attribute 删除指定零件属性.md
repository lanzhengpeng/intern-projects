### 【1044】delete specified part attribute 删除指定零件属性

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
    static logical prompt_for_text_item(char *prompt, char *item, char *text)  
    {  
        int  
            ia[1],  
            ip[1] = { 301 },  
            resp;  
        double  
            da[1];  
        char  
            ca[1][31],  
            menu[1][16];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][16] = '\0';  
        strncpy(&ca[0][0], text, 30);  
        ca[0][31] = '\0';  
        resp = uc1613(prompt, menu, 1, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            if (!strcmp(&ca[0][0], text)) return FALSE;  
            strcpy(text, &ca[0][0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #include <uf_undo.h>  
    static UF_UNDO_mark_id_t set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
        return mark_id;  
    }  
    static void do_it(void)  
    {  
        logical  
            locked;  
        int  
            type_found;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            title[UF_ATTR_MAX_TITLE_LEN+1] = { "" };  
        set_undo_mark("Delete part attributes");  
        while (prompt_for_text_item("Delete part attribute", "Title", title))  
        {  
            UF_CALL(UF_ATTR_find_attribute(part, UF_ATTR_any, title, &type_found));  
            if (type_found == 0)  
            {  
                ECHO("%s = Not Found\n", title);  
                continue;  
            }  
            UF_CALL(UF_ATTR_ask_locked(part, title, &locked));  
            if (locked)  
                ECHO("%s locked = TRUE\n", title);  
            else  
                ECHO("%s locked = FALSE\n", title);  
            if (!locked)  
            {  
                UF_CALL(UF_ATTR_delete(part, type_found, title));  
                UF_CALL(UF_ATTR_find_attribute(part, UF_ATTR_any, title,  
                    &type_found));  
                if (type_found == 0)  
                    ECHO("%s was successfully deleted\n", title);  
                else  
                    ECHO("%s was not deleted?\n", title);  
            }  
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

> 根据代码内容，这是一段用于NX Open的二次开发代码，主要功能如下：
>
> 1. 包含了必要的头文件，用于调用NX的API函数。
> 2. 定义了ECHO函数，用于在列表窗口和系统日志中输出信息。
> 3. 定义了UF_CALL宏，用于调用NX的API函数，并在出错时输出错误信息。
> 4. 定义了report_error函数，用于处理API调用返回的错误码，并输出错误信息。
> 5. 定义了prompt_for_text_item函数，用于在图形界面中提示用户输入文本，并返回用户输入。
> 6. 定义了set_undo_mark函数，用于设置撤销标记点。
> 7. 定义了do_it函数，是主要功能函数。该函数会提示用户输入要删除的部件属性标题，然后调用NX的API删除属性。在删除前会检查属性是否被锁定，若被锁定则不会删除。删除后，会检查属性是否真的被删除。
> 8. 定义了ufusr函数，是程序的入口函数。该函数会初始化NX，调用do_it函数执行功能，然后终止NX。
> 9. 定义了ufusr_ask_unload函数，用于在NX卸载时立即卸载该程序。
>
> 总的来说，这段代码实现了在NX中删除指定部件属性的功能，具有错误处理和撤销功能。
>
