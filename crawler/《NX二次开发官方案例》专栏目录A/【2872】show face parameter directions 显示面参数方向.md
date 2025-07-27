### 【2872】show face parameter directions 显示面参数方向

#### 代码

```cpp
    /*HEAD SHOW_FACE_PARAMETER_DIRECTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，我了解到UF_print_syslog是V18版本中的新功能。 */  
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
        display_colored_conehead(origin, u_dir, UF_OBJ_BLUE);  
        display_colored_conehead(origin, v_dir, UF_OBJ_GREEN);  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        while ((face = select_a_face("Show U & V directions")) != NULL_TAG)  
            show_face_parm_dirs(face);  
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

> 这段代码是NX Open C++开发的二次开发代码，主要功能是显示NX模型面上U向和V向参数方向。
>
> 主要逻辑包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个显示圆锥头的函数display_colored_conehead，用于在指定位置显示不同颜色的圆锥头，用于表示参数方向。
> 3. 定义了一个显示面上U向和V向参数方向的函数show_face_parm_dirs，通过获取面的参数范围和参数方向，然后调用display_colored_conehead函数显示。
> 4. 定义了一个用于选择面的函数select_a_face，使用对话框让用户选择一个面。
> 5. 主函数do_it中循环调用select_a_face选择面，然后调用show_face_parm_dirs显示参数方向。
> 6. 定义了NX初始化和退出的ufusr函数。
> 7. 定义了立即卸载的ufusr_ask_unload函数。
>
> 总体来说，这段代码实现了选择模型面，并显示该面的U向和V向参数方向，以便用户更直观地查看模型参数方向。
>
