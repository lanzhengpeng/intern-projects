### 【0966】create udo linked to point 创建与点关联的UDO

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
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
    static UF_UDOBJ_class_t PT_class_id = 0;  
    DllExport extern UF_UDOBJ_class_t get_PT_class_id(void)  
    {  
        return PT_class_id;  
    }  
    DllExport extern void display_pt_cb(tag_t udo_tag, void *display_context)  
    {  
        int  
            ii;  
        double  
            loc[3];  
        UF_DISP_inquire_t  
            display_info;  
        UF_UDOBJ_all_data_t  
            data;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_DISP_ask_display_context(display_context, &display_info));  
        if (display_info.is_draw_open_disp == TRUE) return;  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo_tag, &data));  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_point_data(data.link_defs[ii].assoc_ug_tag, loc));  
            UF_CALL(UF_DISP_display_points(loc,1,UF_DISP_CIRCLE,display_context));  
        }  
        UF_UDOBJ_free_udo_data(&data);  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    DllExport extern void update_pt_cb(tag_t udo, UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        uc1601("Point has been changed", 1);  
        UF_terminate();  
    }  
    static void register_udo_class(void)  
    {  
        UF_CALL(UF_UDOBJ_create_class("Point_UDO", "Point UDO", &PT_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(PT_class_id));  
        UF_CALL(UF_UDOBJ_register_attn_pt_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_display_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_fit_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_select_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_update_cb(PT_class_id, update_pt_cb));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
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
            pt_udo;  
        double  
            loc[3];  
        UF_UDOBJ_link_t  
            link_def = { 3, NULL_TAG, FALSE };  
        UF_UDOBJ_class_t  
            udo_class_id = get_PT_class_id();  
        if (udo_class_id == 0)  
        {  
            register_udo_class();  
            udo_class_id = get_PT_class_id();  
        }  
        while (specify_point("Indicate UDO position", loc))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(udo_class_id, &pt_udo));  
            UF_CALL(UF_CURVE_create_point(loc, &link_def.assoc_ug_tag));  
            UF_CALL(UF_UDOBJ_add_links(pt_udo, 1, &link_def));  
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

> 这段NX二次开发代码的主要功能如下：
>
> 1. 定义了一个名为"Point_UDO"的用户定义对象(UDO)类，用于创建和管理点类型的UDO。
> 2. 在"Point_UDO"类上注册了多个回调函数，包括显示回调(display_cb)、更新回调(update_cb)等。这些回调函数用于控制UDO的显示和更新行为。
> 3. 实现了 ufsta 和 ufusr 两个函数，分别用于启动和运行UDO的创建和管理功能。
> 4. ufsta 函数用于注册UDO类，而 ufusr 函数则实现了创建UDO的逻辑。它会提示用户指定UDO的位置，然后创建相应的UDO实例并添加到显示中。
> 5. 在 display_cb 回调函数中，实现了UDO的显示逻辑，通过获取UDO关联的曲线点数据，在指定位置绘制圆形标记。
> 6. 在 update_cb 回调函数中，实现了UDO更新时的行为，即打印一条提示信息。
> 7. 通过 ufusr 函数的调用，用户可以交互式地创建多个UDO实例，并在图形区显示出来。
> 8. 整个代码通过回调机制实现了对UDO生命周期的管理，使用户可以方便地创建、显示和管理自定义的点对象。
>
