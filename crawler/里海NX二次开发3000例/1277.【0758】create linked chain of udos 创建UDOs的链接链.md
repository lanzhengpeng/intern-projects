### 【0758】create linked chain of udos 创建UDOs的链接链

#### 代码

```cpp
    /*HEAD CREATE_LINKED_CHAIN_OF_UDOS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* 里海 */  
    DllExport extern void update_one_func(tag_t update_udo,  
        UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        WRITE_D(update_udo);  
        UF_terminate();  
    }  
    /* 里海 */  
    DllExport extern void delete_two_func(tag_t delete_udo,  
        UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        WRITE_D(delete_udo);  
        UF_terminate();  
    }  
    static void register_udo_classes()  
    {  
        UF_CALL(UF_UDOBJ_create_class("ONE_test", "top UDO", &ONE_class_id));  
        UF_CALL(UF_UDOBJ_create_class("TWO_test", "middle UDO", &TWO_class_id));  
        UF_CALL(UF_UDOBJ_create_class("THREE_test", "bottom UDO", &THREE_class_id));  
        UF_CALL(UF_UDOBJ_register_update_cb(ONE_class_id, update_one_func));  
        UF_CALL(UF_UDOBJ_register_delete_cb(TWO_class_id, delete_two_func));  
        UF_CALL(UF_UI_add_to_class_sel(THREE_class_id));  
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
            WRITE_D(top_udo);  
            UF_CALL(UF_UDOBJ_create_udo(get_two_class_id(), &mid_udo));  
            sprintf(name, "MID_%d", ii);  
            UF_CALL(UF_OBJ_set_name(mid_udo, name));  
            pos[1] = pos[1] - 0.25;  
            UF_CALL(UF_OBJ_set_name_origin(mid_udo, pos));  
            UF_CALL(UF_DISP_add_item_to_display(mid_udo));  
            WRITE_D(mid_udo);  
            link_def.assoc_ug_tag = mid_udo;  
            UF_CALL(UF_UDOBJ_add_links(top_udo, 1, &link_def));  
            UF_CALL(UF_UDOBJ_create_udo(get_three_class_id(), &low_udo));  
            sprintf(name, "LOW_%d", ii);  
            UF_CALL(UF_OBJ_set_name(low_udo, name));  
            pos[1] = pos[1] - 0.25;  
            UF_CALL(UF_OBJ_set_name_origin(low_udo, pos));  
            UF_CALL(UF_DISP_add_item_to_display(low_udo));  
            WRITE_D(low_udo);  
            link_def.assoc_ug_tag = low_udo;  
            UF_CALL(UF_UDOBJ_add_links(mid_udo, 1, &link_def));  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了三个用户自定义对象(UDO)类：ONE、TWO、THREE，分别代表顶层、中层和底层的UDO。ONE类注册了更新回调函数update_one_func，TWO类注册了删除回调函数delete_two_func。THREE类被添加到NX选择菜单中。
> 2. ufsta函数用于注册UDO类。
> 3. do_it函数用于创建UDO链。首先获取用户输入的UDO位置，然后创建TOP、MID、LOW三个UDO，将它们的位置依次降低。接着为TOP和MID UDO添加关联的底层UDO，形成链式结构。最后将这些UDO添加到NX显示中。
> 4. ufusr函数是NX的二次开发入口函数，调用do_it函数实现UDO链的创建。
> 5. update_one_func和delete_two_func是ONE和TWO UDO类的回调函数，当这些UDO被更新或删除时，会调用这些函数。
> 6. report_error函数用于打印错误信息。
> 7. WRITE_D宏用于将UDO的tag写入NX日志窗口。
> 8. specify_position函数用于获取用户输入的UDO位置。
>
> 总的来说，这段代码实现了一个NX UDO链的创建和显示，通过回调函数可以监控UDO的更新和删除操作。
>
