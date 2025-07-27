### 【2233】report object attributes 报告对象属性

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_attr.h>  
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
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define ROA(X) report_object_attributes(#X, X)  
    static void report_object_attributes(char *name, tag_t object)  
    {  
        logical  
            locked;  
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
                case UF_ATTR_bool:  
                    if (att.value.boolean)  
                        ECHO("%s = True (BOOLEAN)\n", title, att.value.boolean);  
                    else  
                        ECHO("%s = False (BOOLEAN)\n", title, att.value.boolean);  
                break;  
                case UF_ATTR_integer:  
                    ECHO("%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    ECHO("%s = %f (FLOATING POINT)\n", title, att.value.real);  
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
        //  UF_ATTR_ask_locked and UF_ATTR_set_locked are new in NX8.0, but they  
        //  do not seem to work - see PR 1867721  
            locked = FALSE;  
            UF_CALL(UF_ATTR_ask_locked(object, title, &locked));  
            if (locked) ECHO(" %s is locked\n", title);  
            else ECHO(" %s is not locked\n", title);  
        }  
        ECHO("\n");  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_anything("Report attributes")) != NULL_TAG)  
            ROA(object);  
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

> 这是一段用于NX的二次开发代码，主要实现以下功能：
>
> 1. 定义了ECHO宏，用于打印信息到列表窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用NX的UF函数，并捕获错误。
> 3. 定义了report_error函数，用于打印函数调用错误信息。
> 4. 定义了mask_add_faces_and_edges函数，用于设置选择掩码，以便在选择时只选择面和边。
> 5. 定义了select_anything函数，用于打开选择对话框，并返回用户选择的对象。
> 6. 定义了ROA宏，用于打印一个对象的所有属性信息。
> 7. 定义了report_object_attributes函数，用于遍历对象的所有属性并打印。
> 8. 定义了do_it函数，用于循环调用select_anything和ROA，实现选择一个对象并打印其属性的功能。
> 9. 定义了ufusr函数，作为NX二次开发的入口函数，调用do_it函数。
> 10. 定义了ufusr_ask_unload函数，用于卸载二次开发应用程序。
>
> 该代码主要用于选择NX模型中的任意对象，并打印出该对象的所有属性信息，从而实现了模型属性的查询功能。
>
