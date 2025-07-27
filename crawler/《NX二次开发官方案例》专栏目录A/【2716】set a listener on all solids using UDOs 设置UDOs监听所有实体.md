### 【2716】set a listener on all solids using UDOs 设置UDOs监听所有实体

#### 代码

```cpp
    /*  
    A UDO's update callback is only invoked once per update cycle.    
    If you want the update callback to be invoked for each solid that is changed when a part modification effects multiple solids,   
    you need to create a separate UDO for each body you want to watch.    
    As per the documentation: the UDO's registered callback is called only once per update cycle, regardless of how many associated objects are modified.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
    #include <uf_part.h>  
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
    static UF_UDOBJ_class_t MY_class_id;  
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void)  
    {  
        return MY_class_id;  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    /* qq3123197280 */  
    DllExport extern void update_func(tag_t update_udo,  
        UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        WRITE_D(update_udo);  
        WRITE_D(update_cause->assoc_ug_tag);  
        WRITE_D(update_cause->link_type);  
        UF_terminate();  
    }  
    static void register_udo_class()  
    {  
        ECHO("registering UDO class...\n");  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &MY_class_id));  
        UF_CALL(UF_UDOBJ_register_update_cb(MY_class_id, update_func));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
        UF_terminate();  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            link_udo,  
            part;  
        UF_UDOBJ_link_t  
            link_def = { 3, NULL_TAG, FALSE };  
        if (!get_my_class_id()) register_udo_class();  
        if ((part = UF_PART_ask_display_part()) == NULL_TAG) return;  
        while ((link_def.assoc_ug_tag = ask_next_body(part, link_def.assoc_ug_tag))  
                != NULL_TAG)  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &link_udo));  
            WRITE_D(link_udo);  
            WRITE_D(link_def.assoc_ug_tag);  
            UF_CALL(UF_UDOBJ_add_links(link_udo, 1, &link_def));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个自定义数据对象(UDO)类，并将其注册到NX中。这个UDO类名为"MY_test"，用于跟踪NX部件中的实体变化。
> 2. 为该UDO类注册了一个更新回调函数update_func，当UDO关联的实体发生变化时，该回调函数会被调用。
> 3. 在ufsta函数中，注册了UDO类。这是NX二次开发中的初始化函数。
> 4. 在ufusr函数中，遍历当前显示部件中的所有实体，并为每个实体创建一个UDO实例，并将其与实体关联。
> 5. update_func回调函数会在其关联的实体发生变化时被调用，打印出变化的实体标识。
> 6. 使用宏定义了UF_CALL来简化UF函数的调用，并在出错时报告错误。
> 7. 使用ECHO宏来打印日志信息。
> 8. ask_next_body函数用于遍历部件中的所有实体。
> 9. do_it函数实现了遍历实体并创建UDO实例的逻辑。
>
> 这段代码演示了如何在NX中自定义数据对象，并利用NX提供的二次开发接口来跟踪和响应实体变化，为NX的二次开发提供了有用的示例。
>
