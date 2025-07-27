### 【0968】create udo with multiple owned udos 创建具有多个拥有UDO的UDO

#### 代码

```cpp
    /*HEAD CREATE_UDO_WITH_MULTIPLE_OWNED_UDOS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的注释，UF_print_syslog 是在 V18 版本中新增的。因此，根据要求，只翻译注释内容：

注释：UF_print_syslog 是在 V18 版本中新增的。 */  
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
    static UF_UDOBJ_class_t PNT_class_id;  
    static UF_UDOBJ_class_t PT_class_id;  
    DllExport extern UF_UDOBJ_class_t get_PNT_class_id(void)  
    {  
        return PNT_class_id;  
    }  
    DllExport extern UF_UDOBJ_class_t get_PT_class_id(void)  
    {  
        return PT_class_id;  
    }  
    DllExport extern void display_pt_cb(tag_t udo_tag, void *display_context)  
    {  
        UF_DISP_inquire_t  
            display_info;  
        UF_UDOBJ_all_data_t  
                data;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_DISP_ask_display_context(display_context, &display_info));  
        if (display_info.is_draw_open_disp == TRUE) return;  
        UF_CALL(UF_UDOBJ_ask_udo_data (udo_tag, &data));  
        UF_CALL(UF_DISP_display_points(data.doubles, 1, UF_DISP_POINT,  
            display_context));  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_create_class("Parent_UDO", "Parent UDO", &PNT_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(PNT_class_id));  
        UF_CALL(UF_UDOBJ_set_owned_object_selection(PNT_class_id,  
            UF_UDOBJ_ALLOW_SELECTION));  
        UF_CALL(UF_UDOBJ_create_class("Point_UDO", "Point UDO", &PT_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(PT_class_id));  
        UF_CALL(UF_UDOBJ_register_attn_pt_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_display_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_fit_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_select_cb(PT_class_id, display_pt_cb));  
        UF_terminate();  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            pnt_udo,  
            pt_udo;  
        double  
            loc[3];  
        UF_CALL(UF_UDOBJ_create_udo(get_PNT_class_id(), &pnt_udo));  
        while (specify_point("Indicate UDO position", loc))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_PT_class_id(), &pt_udo));  
            UF_CALL(UF_UDOBJ_add_owning_links(pnt_udo, 1, &pt_udo));  
            UF_CALL(UF_UDOBJ_add_doubles(pt_udo, 3, loc));  
            UF_CALL(UF_DISP_add_item_to_display(pt_udo));  
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

> 这段代码是NX二次开发代码，其主要功能是创建一个名为“Parent_UDO”的UDO类，用于表示父UDO，并创建一个名为“Point_UDO”的子UDO类，用于表示点。在执行时，用户可以指定多个点的位置，并为每个点创建一个Point_UDO UDO对象，将其添加到Parent_UDO对象的所有者列表中。此外，该代码还注册了Point_UDO的显示、拟合、选择等回调函数，以在图形窗口中显示这些点。总体而言，该代码实现了在NX中创建具有父子关系的多个UDO对象，并提供了交互式的点指定功能。
>
