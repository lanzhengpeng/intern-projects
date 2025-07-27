### 【0967】create udo sheet between2lines 在两线之间创建UDO板

#### 代码

```cpp
    /*HEAD CREATE_UDO_SHEET_BETWEEN2LINES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的信息，可以翻译为：

注意：UF_print_syslog是V18版本新增的函数，请仅回答翻译，不要添加任何额外内容。 */  
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
        UF_CALL(UF_CURVE_edit_line_data(data.link_defs[4].assoc_ug_tag,  
            &line_coords));  
        UF_CALL(UF_CURVE_ask_point_data(data.link_defs[2].assoc_ug_tag,  
            line_coords.start_point));  
        UF_CALL(UF_CURVE_ask_point_data(data.link_defs[3].assoc_ug_tag,  
            line_coords.end_point));  
        UF_CALL(UF_CURVE_edit_line_data(data.link_defs[5].assoc_ug_tag,  
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
            alignment = 1,  
            body_type = UF_MODL_SOLID_BODY,  
            c_flag[2] = { 0,0 },  
            ii,  
            patch = 1,  
            vdegree = 3,  
            vstatus = 0;  
        tag_t  
            c_face[2] = { NULL_TAG, NULL_TAG },  
            ents[7],  
            udo;  
        double  
            tol[3] = { 0, 0, 0 },  
            value[6] = { 0,0,0, 0,0,0 };  
        char  
            *name[7] = { "POINT_1", "POINT_2", "POINT_3", "POINT_4",  
                         "LINE_1", "LINE_2", "SHEET" };  
        UF_CURVE_line_t  
            line_coords[2];  
        UF_STRING_t  
            s_section,  
            s_spine;  
        UF_UDOBJ_link_t  
            link_defs[7];  
        while ((specify_point("Ind. 1st point", line_coords[0].start_point)) &&  
               (specify_point("Ind. 2nd point", line_coords[0].end_point)) &&  
               (specify_point("Ind. 3rd point", line_coords[1].start_point)) &&  
               (specify_point("Ind. 4th point", line_coords[1].end_point)))  
        {  
            UF_CALL(UF_CURVE_create_point(line_coords[0].start_point, &ents[0]));  
            UF_CALL(UF_CURVE_create_point(line_coords[0].end_point, &ents[1]));  
            UF_CALL(UF_CURVE_create_point(line_coords[1].start_point, &ents[2]));  
            UF_CALL(UF_CURVE_create_point(line_coords[1].end_point, &ents[3]));  
            UF_MODL_init_string_list(&s_section);  
            UF_MODL_create_string_list(2, 2, &s_section);  
            s_section.string[0] = 1;  
            s_section.dir[0] = 1;  
            s_section.string[1] = 1;  
            s_section.dir[1] = 1;  
            UF_CALL(UF_CURVE_create_line(&line_coords[0], &ents[4]));  
            s_section.id[0] = ents[4];  
            UF_CALL(UF_CURVE_create_line(&line_coords[1], &ents[5]));  
            s_section.id[1] = ents[5];  
            UF_MODL_init_string_list(&s_spine);  
            UF_CALL(UF_MODL_create_thru_curves(&s_section, &s_spine, &patch,  
                &alignment, value, &vdegree, &vstatus, &body_type, UF_NULLSIGN,  
                        tol, c_face, c_flag, &ents[6] ));  
            UF_MODL_free_string_list(&s_section);  
            UF_MODL_free_string_list(&s_spine);  
            for (ii = 0; ii < 7; ii++)  
            {  
                UF_CALL(UF_OBJ_set_name(ents[ii], name[ii]));  
                link_defs[ii].link_type = 1;  
                link_defs[ii].assoc_ug_tag = ents[ii];  
                link_defs[ii].object_status = FALSE;  
            }  
            UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &udo));  
            UF_CALL(UF_UDOBJ_add_links(udo, 7, link_defs));  
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

> 这段代码是一个NX Open C++ API编写的NX UDO(用户定义对象)示例代码，用于在两条线之间创建一个面。
>
> 主要功能包括：
>
> 1. 定义一个UDO类MY_test，并注册了update_func函数作为该类的更新回调函数。
> 2. update_func函数会在UDO创建后，根据UDO关联的4个点和2条线更新UDO关联的2条线的终点，从而改变UDO关联的面。
> 3. ufusr函数用于创建UDO。首先，它会提示用户指定4个点，然后创建这4个点，并创建连接这4个点的2条线，接着使用这2条线创建一个面。最后，它会创建一个MY_test类的UDO，并将这4个点、2条线和面关联到该UDO。
> 4. ufsta函数用于在NX启动时初始化UDO类。
> 5. specify_point函数用于提示用户指定一个点。
> 6. do_it函数用于执行创建面和UDO的逻辑。
> 7. report_error函数用于在出错时打印错误信息。
>
> 总体来说，该代码提供了一个在两条线之间创建面的简单示例，并且将创建的面与UDO关联，以便在更新线时自动更新面。
>
