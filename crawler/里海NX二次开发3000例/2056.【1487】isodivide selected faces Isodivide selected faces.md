### 【1487】isodivide selected faces Isodivide selected faces

#### 代码

```cpp
    /*HEAD ISODIVIDE_SELECTED_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void show_face_parm_dirs(tag_t face)  
    {  
        double  
            junk[3],  
            origin[3],  
            parm[2],  
            uvs[4],  
            u_dir[3],  
            v_dir[3];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = uvs[0];  
        parm[1] = uvs[2];  
        UF_CALL(UF_MODL_ask_face_props(face, parm, origin, u_dir, v_dir,  
            junk, junk, junk, junk));  
        display_colored_conehead(origin, u_dir, UF_OBJ_RED);  
        display_colored_conehead(origin, v_dir, UF_OBJ_GREEN);  
    }  
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:返回1或2 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            div_dir,  
            resp = 1;  
        tag_t  
            face,  
            sheet;  
        while ((face = select_a_face("Select face to isodivide")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_extract_face(face, 2, &face));  
            show_face_parm_dirs(face);  
            if (pick_one_of_two_choices("Direction", "U (red)", "V (green)", &resp))  
            {  
            /*  See PR 4120972 (0 = U direction, 1 = V direction) 里海译:翻译如下：

请参阅PR 4120972 (0 = U方向，1 = V方向)。 */  
                div_dir = resp - 1;  
                if (!UF_CALL(UF_MODL_isodivide_face(&face, 0.5, div_dir, &sheet)))  
                /*  The following line is a work around to PR 4283984 里海译:以下是解决PR 4283984的临时解决方法。 */  
                    UF_CALL(UF_DISP_add_item_to_display(sheet));  
            }  
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

> 这段代码是用于在NX中实现一个简单的用户界面，以选择一个面进行等分操作。代码的主要功能包括：
>
> 1. 提供一个对话框，允许用户选择一个面进行等分。
> 2. 提取用户选择的面，并显示其参数方向，用红色和绿色圆锥体表示U方向和V方向。
> 3. 提供一个对话框，让用户选择等分方向(U或V方向)。
> 4. 对用户选择的面向U或V方向进行等分，创建等分曲线。
> 5. 添加等分曲线到显示中，以解决一个bug。
> 6. 提供一个函数，在程序卸载时调用，以便立即卸载。
> 7. 使用UF_print_syslog函数在系统日志中打印错误信息。
> 8. 定义了一个错误报告函数，用于打印错误代码、文件、行号等信息。
> 9. 使用UF_CALL宏简化了UF函数的调用。
>
> 总体来说，这段代码实现了在NX中选择面并沿U或V方向进行等分的功能，并包含了错误处理和用户交互。
>
