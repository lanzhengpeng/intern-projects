### 【2355】report selected objects owning parts attributes 报告所选对象所属部件的属性

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_OBJECTS_OWNING_PARTS_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_attr.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的，仅回答翻译，无需添加其他内容。

翻译：
UF_print_syslog是V18新增的。 */  
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
            UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL, NULL, &resp, &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            num;  
        tag_t  
            object,  
            proto,  
            parent;  
        UF_ATTR_part_attr_p_t  
            attrs;  
        while ( (object = select_anything("Select an object")) != NULL_TAG)  
        {  
             UF_CALL(UF_OBJ_ask_owning_part(object, &parent));  
            if (UF_ASSEM_is_occurrence(object))  
            proto = UF_ASSEM_ask_prototype_of_occ(object);  
            else  
                proto = object;  
            UF_CALL(UF_OBJ_ask_owning_part(proto, &parent));  
            UF_CALL( UF_ATTR_ask_part_attrs( parent, &num, &attrs));  
            printf("\n\nNumber of attributes found: %d\n", num);  
            for( ii = 0; ii < num; ii++)  
            {  
                printf("Attribute title: %s\n", attrs[ii].title);  
                printf("Attribute value: %s\n", attrs[ii].string_value);  
            }  
            UF_free( attrs);  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是一段用于NX Open C++ API的二次开发代码，主要实现了以下功能：
>
> 1. 定义了宏UF_CALL，用于调用NX API函数，并检查返回码，如果失败则打印错误信息。
> 2. 实现了函数report_error，用于打印错误信息。
> 3. 实现了函数select_anything，用于打开选择对话框，让用户选择一个对象，并返回该对象的tag。
> 4. 实现了函数do_it，用于循环调用select_anything获取用户选择的对象，然后查询该对象所属的组件，并打印组件的属性。
> 5. 实现了ufusr函数，是程序的入口函数，用于初始化NX Open C++环境，调用do_it执行功能，然后终止环境。
> 6. 实现了ufusr_ask_unload函数，用于卸载动态库时调用。
>
> 整体来看，该代码实现了在NX中查询组件属性的功能，通过选择对话框让用户选择对象，然后获取对象所属的组件，并打印组件的属性，以实现查询组件属性的目的。
>
