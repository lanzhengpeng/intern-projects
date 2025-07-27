### 【2996】trim body 使用刀具面修剪目标体

#### 代码

```cpp
    /*HEAD TRIM_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能，用于打印系统日志。 */  
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
    /* qq3123197280 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_faces_and_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                       { UF_plane_type, 0, 0 },  
                       { UF_datum_plane_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face_or_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select face or plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces_and_planes, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        logical  
            rev_dir = FALSE;  
        int  
            subtype,  
            type;  
        tag_t  
            body,  
            feat,  
            tool,  
            trim;  
        double  
            dir[3],  
            junk[6],  
            parm[2],  
            point[3],  
            uvs[4];  
        char  
            *angle,  
            *offset;  
        if (((body = select_a_body("Target body")) != NULL_TAG) &&  
            ((tool = select_a_face_or_plane("Trim tool")) != NULL_TAG))  
        {  
            set_undo_mark("Trim Body");  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tool, &type, &subtype));  
            switch (type)  
            {  
                case UF_plane_type:  
                    FTN(uf5375)(&tool, point, dir);  
                    break;  
                case UF_datum_plane_type:  
                    UF_CALL(UF_MODL_ask_object_feat(tool, &feat));  
                    UF_CALL(UF_MODL_ask_datum_plane_parms(feat, point, dir,  
                        &offset, &angle));  
                    UF_free(offset);  
                    UF_free(angle);  
                    break;  
                default: /* face 里海译:脸 */  
                    UF_CALL(UF_MODL_ask_face_uv_minmax(tool, uvs));  
                    parm[0] = (uvs[0] + uvs[1]) / 2;  
                    parm[1] = (uvs[2] + uvs[3]) / 2;  
                    UF_CALL(UF_MODL_ask_face_props(tool, parm, point, junk, junk,  
                        junk, junk, dir, junk));  
                    break;  
            }  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, point, dir, 0);  
            rev_dir = ask_yes_or_no("Trim Body", "Reverse Default Direction");  
            UF_CALL(UF_MODL_trim_body(body, tool, rev_dir, &trim));  
        }  
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

> 这段代码是一个用于在NX中进行二次开发的UF程序，其主要功能是实现对实体进行修剪的功能。以下是该程序的主要组成部分：
>
> 1. 错误处理函数：用于处理UF函数调用时的错误，并输出错误信息。
> 2. 选择实体函数：用于选择目标实体和修剪工具。
> 3. 询问用户函数：用于询问用户是否需要反转默认的修剪方向。
> 4. 设置撤销标记函数：用于在修剪前设置撤销标记，以便可以撤销操作。
> 5. 修剪实体函数：用于根据选择的实体和工具进行修剪。
> 6. UFusr函数：程序的入口函数，用于初始化和调用修剪函数。
> 7. 卸载函数：用于卸载该UF程序。
>
> 在执行流程方面，用户首先选择目标实体，然后选择修剪工具，系统会提示是否需要反转方向，设置撤销标记，最后进行修剪操作。如果用户需要，还可以撤销修剪操作。
>
> 该程序利用了NX提供的UF函数进行二次开发，实现了选择、提示、设置撤销和修剪实体等基本功能。
>
