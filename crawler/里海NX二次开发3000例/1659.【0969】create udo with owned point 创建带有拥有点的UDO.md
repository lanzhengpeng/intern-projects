### 【0969】create udo with owned point 创建带有拥有点的UDO

#### 代码

```cpp
    /*HEAD CREATE_UDO_WITH_OWNED_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的，仅回答翻译，不要添加无关内容。 */  
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
    static UF_UDOBJ_class_t PT_class_id;  
    DllExport extern UF_UDOBJ_class_t get_PT_class_id(void)  
    {  
        return PT_class_id;  
    }  
    static void map_abs2view(double c[3])  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc6433("", &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    DllExport extern void display_pt_cb(tag_t udo_tag, void *display_context)  
    {  
        int  
            ii,  
            n_owned;  
        tag_t  
            *owned;  
        double  
            coords[3],  
            vm[9];  
        UF_DISP_inquire_t  
            display_info;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_DISP_ask_display_context(display_context, &display_info));  
        if (display_info.is_draw_open_disp == TRUE) return;  
        UF_CALL(UF_UDOBJ_ask_owned_objects(udo_tag, &n_owned, &owned));  
        for (ii = 0; ii < n_owned; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_point_data(owned[ii], coords));  
            UF_CALL(UF_DISP_display_points(coords, 1, UF_DISP_SQUARE,  
                display_context));  
            UF_CALL(uc6433("", vm));  
            map_abs2view(coords);  
            UF_CALL(UF_DISP_display_arc(vm,0.0,TWOPI,coords,0.5,display_context));  
        }  
        UF_free(owned);  
        UF_terminate();  
    }  
    DllExport extern void update_pt_cb(tag_t udo, UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        uc1601("Point has been changed", 1);  
        UF_terminate();  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_create_class("Point_UDO", "Point UDO", &PT_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(PT_class_id));  
        UF_CALL(UF_UDOBJ_register_attn_pt_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_display_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_fit_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_select_cb(PT_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_update_cb(PT_class_id, update_pt_cb));  
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
        int  
            ii = 0;  
        tag_t  
            point,  
            pt_udo;  
        double  
            loc[3];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while (specify_point("Indicate UDO position", loc))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_PT_class_id(), &pt_udo));  
            UF_CALL(UF_CURVE_create_point(loc, &point));  
            UF_CALL(UF_UDOBJ_add_owning_links(pt_udo, 1, &point));  
            UF_CALL(UF_DISP_add_item_to_display(pt_udo));  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 创建一个名为"Point_UDO"的用户定义对象(UDO)类，并注册了显示、更新等回调函数。
> 2. 在NX界面中添加了该UDO类到选择菜单。
> 3. 实现了一个"Indicate UDO position"提示框，用于用户指定UDO的位置。
> 4. 每当用户指定一个位置时，创建一个Point类型的NX对象，并将其作为该UDO的拥有对象。
> 5. UDO显示回调函数会在NX中显示该Point对象，并绘制一个圆弧表示UDO的位置。
> 6. UDO更新回调函数会在NX中提示"Point has been changed"。
> 7. 在NX启动时，会调用ufsta函数，完成上述初始化工作。
> 8. 用户执行该UDO命令时，会调用ufusr函数，进入一个循环，等待用户指定UDO的位置，直到用户取消。
> 9. 代码中包含错误处理机制，能够捕获并打印出函数调用失败的错误信息。
> 10. 使用了NX提供的UF函数接口。
>
> 总的来说，这段代码实现了一个NX的UDO，允许用户通过交互式方式在NX中创建和管理一系列点对象。
>
