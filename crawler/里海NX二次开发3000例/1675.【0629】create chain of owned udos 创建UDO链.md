### 【0629】create chain of owned udos 创建UDO链

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
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
    static UF_UDOBJ_class_t ONE_class_id;  
    static UF_UDOBJ_class_t TWO_class_id;  
    static UF_UDOBJ_class_t THREE_class_id;  
    DllExport extern UF_UDOBJ_class_t get_one_class_id(void)  
    {  
        return ONE_class_id;  
    }  
    DllExport extern UF_UDOBJ_class_t get_two_class_id(void)  
    {  
        return TWO_class_id;  
    }  
    DllExport extern UF_UDOBJ_class_t get_three_class_id(void)  
    {  
        return THREE_class_id;  
    }  
    /* 里海 */  
    DllExport extern void edit_one_func(tag_t edit_udo)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        uc1601("Editting TOP UDO", TRUE);  
        UF_terminate();  
    }  
    DllExport extern void edit_two_func(tag_t edit_udo)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        uc1601("Editting MID UDO", TRUE);  
        UF_terminate();  
    }  
    DllExport extern void edit_three_func(tag_t edit_udo)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        uc1601("Editting LOW UDO", TRUE);  
        UF_terminate();  
    }  
    static void register_udo_classes()  
    {  
        UF_CALL(UF_UDOBJ_create_class("ONE_test", "top UDO", &ONE_class_id));  
        UF_CALL(UF_UDOBJ_create_class("TWO_test", "middle UDO", &TWO_class_id));  
        UF_CALL(UF_UDOBJ_create_class("THREE_test", "bottom UDO", &THREE_class_id));  
        UF_CALL(UF_UDOBJ_register_edit_cb(ONE_class_id, edit_one_func));  
        UF_CALL(UF_UDOBJ_register_edit_cb(TWO_class_id, edit_two_func));  
        UF_CALL(UF_UDOBJ_register_edit_cb(THREE_class_id, edit_three_func));  
        UF_CALL(UF_UI_add_to_class_sel(ONE_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(TWO_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(THREE_class_id));  
        UF_CALL(UF_UDOBJ_set_owned_object_selection(ONE_class_id, UF_UDOBJ_ALLOW_SELECTION));  
        UF_CALL(UF_UDOBJ_set_owned_object_selection(TWO_class_id, UF_UDOBJ_ALLOW_SELECTION));  
    }  
    /* 里海 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_classes();  
        UF_terminate();  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            low_udo,  
            mid_udo,  
            point,  
            top_udo;  
        double  
            pos[3];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_UDOBJ_link_t  
            link_def = { 3, NULL_TAG, FALSE };  
        if (!get_one_class_id()) register_udo_classes();  
        while (specify_position("position UDO", pos))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_one_class_id(), &top_udo));  
            sprintf(name, "TOP_%d", ++ii);  
            UF_CALL(UF_OBJ_set_name(top_udo, name));  
            UF_CALL(UF_OBJ_set_name_origin(top_udo, pos));  
            UF_CALL(UF_DISP_add_item_to_display(top_udo));  
            UF_CALL(UF_CURVE_create_point(pos, &point));  
            UF_CALL(UF_UDOBJ_add_owning_links(top_udo, 1, &point));  
            UF_CALL(UF_UDOBJ_create_udo(get_two_class_id(), &mid_udo));  
            sprintf(name, "MID_%d", ii);  
            UF_CALL(UF_OBJ_set_name(mid_udo, name));  
            pos[1] = pos[1] - 0.25;  
            UF_CALL(UF_OBJ_set_name_origin(mid_udo, pos));  
            UF_CALL(UF_DISP_add_item_to_display(mid_udo));  
            UF_CALL(UF_UDOBJ_add_owning_links(top_udo, 1, &mid_udo));  
            UF_CALL(UF_CURVE_create_point(pos, &point));  
            UF_CALL(UF_UDOBJ_add_owning_links(mid_udo, 1, &point));  
            UF_CALL(UF_UDOBJ_create_udo(get_three_class_id(), &low_udo));  
            sprintf(name, "LOW_%d", ii);  
            UF_CALL(UF_OBJ_set_name(low_udo, name));  
            pos[1] = pos[1] - 0.25;  
            UF_CALL(UF_OBJ_set_name_origin(low_udo, pos));  
            UF_CALL(UF_DISP_add_item_to_display(low_udo));  
            UF_CALL(UF_UDOBJ_add_owning_links(mid_udo, 1, &low_udo));  
            UF_CALL(UF_CURVE_create_point(pos, &point));  
            UF_CALL(UF_UDOBJ_add_owning_links(low_udo, 1, &point));  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了三个自定义对象(UDO)类ONE_test、TWO_test、THREE_test，分别表示顶层、中层和底层的UDO，并为它们注册了编辑回调函数。
> 2. 在ufsta函数中，注册了三个UDO类，并将它们添加到NX的类选择器中，允许用户选择它们。
> 3. 在ufusr函数中，实现了以下功能：循环提示用户输入UDO的位置。根据用户输入的位置，依次创建顶层、中层和底层的UDO，并设置它们的名称和位置。为每个UDO创建一个关联的点，并将该点设置为UDO的所有者。将创建的UDO和点添加到NX显示中。通过拥有关系链接三个UDO，形成一个树形结构。
> 4. 循环提示用户输入UDO的位置。
> 5. 根据用户输入的位置，依次创建顶层、中层和底层的UDO，并设置它们的名称和位置。
> 6. 为每个UDO创建一个关联的点，并将该点设置为UDO的所有者。
> 7. 将创建的UDO和点添加到NX显示中。
> 8. 通过拥有关系链接三个UDO，形成一个树形结构。
> 9. 在编辑回调函数中，简单输出了编辑的是哪个层的UDO。
> 10. 使用宏UF_CALL来调用NX API，并在出错时输出错误信息。
>
> 总的来说，这段代码实现了一个简单的树形结构UDO的创建和编辑功能，用户可以输入位置来创建一个包含三个层级的UDO树。
>
