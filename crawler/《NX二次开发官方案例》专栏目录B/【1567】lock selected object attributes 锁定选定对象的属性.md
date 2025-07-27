### 【1567】lock selected object attributes 锁定选定对象的属性

#### 代码

```cpp
    //File -> Utilities -> Customer Defaults -> Gateway -> User Attributes -> "Attribute Locking via NX Open" is tuned ON  
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
        int  
            indx = 0,  
            n;  
        char  
            dstr[21],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_ATTR_iterator_t   
            attrIte;  
        logical locked = true;  
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
            UF_CALL(UF_ATTR_init_user_attribute_iterator( &attrIte ));  
            attrIte.type = att.type;  
            UF_CALL(UF_ATTR_set_user_attribute_locks(object, &attrIte, locked));  
            if (locked) ECHO(" %s is locked\n", title);  
            else ECHO(" %s is not locked\n", title);  
        }  
        ECHO("\n");  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_anything("Lock attributes")) != NULL_TAG)  
            ROA(object);  
    }  
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

> 这段代码是一个NX Open的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个ECHO宏，用于在列表窗口和系统日志中输出信息，方便调试。
> 2. 定义了一个UF_CALL宏，用于调用NX的UF函数，并捕获错误，输出错误信息。
> 3. 实现了一个mask_add_faces_and_edges函数，用于设置选择掩码，以便在选择时只选择面和边。
> 4. 实现了一个select_anything函数，用于显示一个选择对话框，让用户选择一个对象。
> 5. 实现了一个report_object_attributes函数，用于遍历一个对象的所有属性，并输出属性信息。在遍历过程中，该函数还会尝试锁定每个属性。
> 6. 实现了一个do_it函数，用于循环调用select_anything选择对象，然后调用report_object_attributes报告属性信息。
> 7. 实现了ufusr主函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 实现了ufusr_ask_unload函数，用于卸载用户库。
>
> 总的来说，这段代码提供了一个简单的NX二次开发示例，通过选择对象并输出属性信息，演示了NX Open的基础操作。
>
