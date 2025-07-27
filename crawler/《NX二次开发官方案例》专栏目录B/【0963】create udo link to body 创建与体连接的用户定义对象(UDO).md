### 【0963】create udo link to body 创建与体连接的用户定义对象(UDO)

#### 代码

```cpp
    /*HEAD CREATE_UDO_LINK_TO_BODY CCC UFUN */  
    #include <uf_object_types.h>  
    #include <uf_udobj.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    DllExport extern void update_func(tag_t udo, UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        uc1601("Body has been updated", 1);  
        UF_terminate();  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &MY_class_id));  
        UF_CALL(UF_UDOBJ_register_update_cb(MY_class_id, update_func));  
        UF_terminate();  
    }  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            udo;  
        UF_UDOBJ_link_t  
            link_defs[3];  
        while ( body = select_a_body("Select body"))  
        {  
            link_defs[0].link_type = 1;  
            link_defs[0].assoc_ug_tag = body;  
            link_defs[0].object_status = FALSE;  
            UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &udo));  
            UF_CALL(UF_UDOBJ_add_links(udo, 1, link_defs));  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是将NX中的实体关联到一个自定义数据对象(UDO)中。
>
> 主要步骤包括：
>
> 1. 定义了一个自定义的UDO类MY_test，并注册了更新回调函数update_func，当UDO关联的实体发生更新时，会调用这个回调函数。
> 2. ufsta函数负责初始化自定义UDO类，并注册更新回调函数。
> 3. mask_for_bodies函数用于设置选择过滤条件，只允许选择实体。
> 4. select_a_body函数用于提示用户选择一个实体。
> 5. do_it函数用于创建UDO对象，并将用户选择的实体关联到UDO对象中。通过调用UF_UDOBJ_add_links函数实现关联。
> 6. ufusr函数是主入口函数，调用do_it函数执行主要功能。
> 7. 使用宏UF_CALL来简化错误处理。
>
> 总的来说，这段代码实现了自定义UDO类，并将NX中的实体关联到这个UDO对象中，当实体更新时可以调用回调函数进行处理。
>
