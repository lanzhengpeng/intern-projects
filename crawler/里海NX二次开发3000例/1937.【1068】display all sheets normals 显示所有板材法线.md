### 【1068】display all sheets normals 显示所有板材法线

#### 代码

```cpp
    /*HEAD DISPLAY_ALL_SHEETS_NORMALS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog是V18版本中的新功能。 */  
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
    static tag_t ask_next_sheet_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SHEET_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    static tag_t ask_sheet_first_face(tag_t sheet)  
    {  
        tag_t  
            face;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(sheet, &face_list));  
        face = face_list->eid;  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }  
    static void display_mid_face_normal(tag_t face)  
    {  
        double  
            parm[2],  
            pnt[3],  
            junk[3],  
            norm_dir[3],  
            uvs[4];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = (uvs[0] + uvs[1]) / 2;  
        parm[1] = (uvs[2] + uvs[3]) / 2;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, junk, junk, junk,  
            junk, norm_dir, junk));  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, pnt, norm_dir, 0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            sheet = NULL_TAG;  
        while ((sheet = ask_next_sheet_body(part, sheet)) != NULL_TAG)  
            display_mid_face_normal(ask_sheet_first_face(sheet));  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容，这是一段NX的二次开发代码，主要功能是显示当前零件中的所有钣金体的法线。下面是代码的详细介绍：
>
> 1. 代码结构：代码定义了ufusr函数作为主入口点，并在初始化后调用do_it函数执行主体逻辑，最后进行终止。
> 2. 功能实现：ask_next_sheet_body函数用于遍历零件中的实体，并找到下一个钣金体。ask_sheet_first_face函数用于获取钣金体的第一个面。display_mid_face_normal函数用于计算钣金体第一个面的中点，并绘制法线。
> 3. ask_next_sheet_body函数用于遍历零件中的实体，并找到下一个钣金体。
> 4. ask_sheet_first_face函数用于获取钣金体的第一个面。
> 5. display_mid_face_normal函数用于计算钣金体第一个面的中点，并绘制法线。
> 6. 错误处理：report_error函数用于记录错误信息。在UF函数调用前加上UF_CALL宏，用于检查函数返回码，并调用report_error函数。
> 7. report_error函数用于记录错误信息。
> 8. 在UF函数调用前加上UF_CALL宏，用于检查函数返回码，并调用report_error函数。
> 9. 法线显示：使用UF_DISP_conehead函数在钣金体第一个面的中点绘制法线，以显示钣金体的法线方向。
> 10. 交互性：通过UF_UI_open_listing_window函数，代码实现了与用户的交互，将错误信息显示在NX的日志窗口中。
> 11. 代码规范：代码遵循了NX二次开发的基本规范，包括使用标准头文件，定义函数入口点，进行错误处理等。
> 12. 版本兼容性：代码适用于NX18及以上版本，并使用了NX18新增的UF_print_syslog函数。
>
> 综上所述，该代码实现了在NX中显示当前零件中所有钣金体的法线方向，遵循了NX二次开发的基本规范，并具有一定的交互性。
>
