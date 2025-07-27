### 【3091】create udo linking2points 创建UDO连接两个点的程序代码

#### 代码

```cpp
    /*HEAD CREATE_UDO_LINKING2POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18中新增的，只回答翻译，不要废话。 */  
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
        UF_UDOBJ_all_data_t  
            data;  
        UF_CURVE_line_t  
            line_coords;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        UF_CALL(UF_CURVE_ask_point_data(data.link_defs[0].assoc_ug_tag,  
            line_coords.start_point));  
        UF_CALL(UF_CURVE_ask_point_data(data.link_defs[1].assoc_ug_tag,  
            line_coords.end_point));  
        UF_CALL(UF_CURVE_edit_line_data(data.link_defs[2].assoc_ug_tag,  
            &line_coords));  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        UF_terminate();  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &MY_class_id));  
        UF_CALL(UF_UDOBJ_register_update_cb(MY_class_id, update_func));  
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
            ii;  
        tag_t  
            ents[3],  
            udo;  
        char  
            *name[3] = { "POINT_1", "POINT_2", "LINE" };  
        UF_CURVE_line_t  
            line_coords;  
        UF_UDOBJ_link_t  
            link_defs[3];  
        while ((specify_point("Ind. 1st point", line_coords.start_point)) &&  
               (specify_point("Ind. 2nd point", line_coords.end_point)))  
        {  
            UF_CALL(UF_CURVE_create_point(line_coords.start_point, &ents[0]));  
            UF_CALL(UF_CURVE_create_point(line_coords.end_point, &ents[1]));  
            UF_CALL(UF_CURVE_create_line(&line_coords, &ents[2]));  
            for (ii = 0; ii < 3; ii++)  
            {  
                UF_CALL(UF_OBJ_set_name(ents[ii], name[ii]));  
                link_defs[ii].link_type = 1;  
                link_defs[ii].assoc_ug_tag = ents[ii];  
                link_defs[ii].object_status = FALSE;  
            }  
            UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &udo));  
            UF_CALL(UF_UDOBJ_add_links(udo, 3, link_defs));  
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

> 这段NX二次开发代码主要实现了创建用户定义对象(UDO)的功能，通过连接两个点来生成线。以下是代码的主要功能：
>
> 1. 定义了错误处理函数report_error，用于输出错误信息。
> 2. 定义了UDO的类ID MY_class_id，并在函数get_my_class_id中返回。
> 3. 定义了UDO的更新函数update_func，用于根据两个点的变化更新连接的线。
> 4. ufsta函数用于初始化UDO类，并注册更新回调函数。
> 5. specify_point函数用于提示用户指定点的坐标。
> 6. do_it函数用于创建两个点，生成连接线，并创建UDO对象，同时将点与线连接到UDO中。
> 7. ufusr函数用于初始化并调用do_it函数来执行创建UDO的逻辑。
>
> 总的来说，这段代码通过UDO实现了两点确定一条线的功能，用户可以指定两个点来动态生成连接线，并在两点变化时自动更新线的位置。
>
