### 【0961】create udo and owning link to selected object 创建用户定义对象，并建立到选定对象的拥有链接

#### 代码

```cpp
    /*HEAD CREATE_UDO_AND_OWNING_LINK_TO_SELECTED_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能。 */  
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
    static UF_UDOBJ_class_t MY_class_id;  
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void)  
    {  
        return MY_class_id;  
    }  
    static void register_udo_class()  
    {  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &MY_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(MY_class_id));  
        UF_CALL(UF_UDOBJ_set_owned_object_selection(MY_class_id,  
            UF_UDOBJ_ALLOW_SELECTION));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
        UF_terminate();  
    }  
    static tag_t select_an_object_and_location(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            owned_object,  
            link_udo;  
        double  
            pos[3];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        if (!get_my_class_id()) register_udo_class();  
        while ((owned_object =  
            select_an_object_and_location("Link to udo", pos)) != NULL_TAG)  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &link_udo));  
            sprintf(name, "UDO_%d", ++ii);  
            UF_CALL(UF_OBJ_set_name(link_udo, name));  
            UF_CALL(UF_OBJ_set_name_origin(link_udo, pos));  
            UF_CALL(UF_DISP_add_item_to_display(link_udo));  
            UF_CALL(UF_UDOBJ_add_owning_links(link_udo, 1, &owned_object));  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个自定义数据对象(UDO)类，并将其注册到NX中。
> 2. 提供了一个函数，用于在NX中选择一个对象，并创建一个UDO实例链接到该对象，同时设置名称和位置。
> 3. 在NX启动时注册UDO类，并在执行时创建UDO实例并链接到选定的对象。
> 4. 使用UF_initialize()和UF_terminate()初始化和关闭NX。
> 5. 使用UF_call宏来调用NX函数，并处理错误。
> 6. 使用UF_print_syslog和UF_UI_write_listing_window输出错误信息。
> 7. 使用UF_UI_select_with_single_dialog选择对象。
> 8. 使用UF_UDOBJ_create_udo创建UDO实例。
> 9. 使用UF_OBJ_set_name和UF_OBJ_set_name_origin设置名称和位置。
> 10. 使用UF_DISP_add_item_to_display添加到显示。
> 11. 使用UF_UDOBJ_add_owning_links添加拥有关系。
>
> 综上所述，这段代码通过NX二次开发，实现了选择对象并创建链接到该对象的UDO实例的功能。
>
