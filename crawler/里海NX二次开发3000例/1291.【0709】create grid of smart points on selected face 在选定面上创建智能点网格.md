### 【0709】create grid of smart points on selected face 在选定面上创建智能点网格

#### 代码

```cpp
    /*HEAD CREATE_GRID_OF_SMART_POINTS_ON_SELECTED_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_point.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
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
    /* 里海 */  
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
        UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, u_dir, 0, "U");  
        UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, v_dir, 0, "V");  
    }  
    static logical prompt_for_two_integers(char *prompt, char *item1, char *item2,  
        int *number1, int *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        int  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1607(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void create_smart_point_grid_on_face(tag_t face, int nu, int nv)  
    {  
        int  
            ii, jj;  
        tag_t  
            point,  
            u_tag,  
            v_tag;  
        double  
            du, dv,  
            u, v;  
        set_undo_mark("Grid of points");  
        du = (1.0 / (nu - 1));  
        dv = (1.0 / (nv - 1));  
        for (ii = 0, u = 0; ii < nu; ii++, u = u + du)  
        {  
            for (jj = 0, v = 0; jj < nv; jj++, v = v + dv)  
            {  
                UF_CALL(UF_SO_create_scalar_double(face,  
                    UF_SO_update_within_modeling, u, &u_tag));  
                UF_CALL(UF_SO_create_scalar_double(face,  
                    UF_SO_update_within_modeling, v, &v_tag));  
                 UF_CALL(UF_POINT_create_on_surface(face, u_tag, v_tag,  
                    &point));  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            nu = 6,  
            nv = 6;  
        tag_t  
            face;  
        while ((face = select_a_face("Grid of points")) != NULL_TAG)  
        {  
            UF_DISP_refresh();  
            show_face_parm_dirs(face);  
            if (prompt_for_two_integers("Grid of points", "No. in U",  
                "No. in V", &nu, &nv))  
            create_smart_point_grid_on_face(face, nu, nv);  
        }  
    }  
    /* 里海 */  
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

> 这段NX二次开发代码的主要功能是在选定的面上创建一个智能点网格。下面是代码的主要功能和步骤：
>
> 1. 定义了一些辅助函数，如ECHO、report_error等，用于打印信息、报告错误等。
> 2. 定义了一个函数mask_for_faces，用于设置选择模式，使得只能选择面特征。
> 3. 定义了一个函数select_a_face，用于选择一个面。
> 4. 定义了一个函数show_face_parm_dirs，用于显示面的U向和V向参数方向。
> 5. 定义了一个函数prompt_for_two_integers，用于提示用户输入两个整数。
> 6. 定义了一个函数set_undo_mark，用于设置撤销标记。
> 7. 定义了一个函数create_smart_point_grid_on_face，用于在面上创建智能点网格。
> 8. 定义了一个函数do_it，用于执行整个流程：选择面 -> 显示面的参数方向 -> 提示输入U向和V向的点数 -> 在面上创建智能点网格。
> 9. 在ufusr函数中，初始化NX环境，调用do_it函数，然后结束NX环境。
> 10. 定义了一个询问卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码实现了在NX中选定面上创建智能点网格的功能，包括选择面、提示输入点数、创建点等功能。
>
