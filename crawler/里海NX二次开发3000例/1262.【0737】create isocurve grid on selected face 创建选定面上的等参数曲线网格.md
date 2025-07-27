### 【0737】create isocurve grid on selected face 创建选定面上的等参数曲线网格

#### 代码

```cpp
    /*HEAD CREATE_ISOCURVE_GRID_ON_SELECTED_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int create_isocurve_grid_on_face(tag_t face, int nu, int nv,  
        tag_t **isos)  
    {  
        int  
            ii, jj, n, type;  
        tag_t  
            *curves;  
        double  
            du, dv,  
            tol,  
            uv_minmax[4],  
            u, v;  
        uf_list_p_t  
            curve_list;  
        set_undo_mark("Isocurve Grid");  
        UF_CALL(UF_MODL_create_list(&curve_list));  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uv_minmax));  
        du = ((uv_minmax[1] - uv_minmax[0]) / (nu - 1));  
        dv = ((uv_minmax[3] - uv_minmax[2]) / (nv - 1));  
        for (ii = 0, u = uv_minmax[0]; ii < nu; ii++, u = u + du)  
        {  
            UF_CALL(UF_MODL_create_isocurve(face, 1, u, tol, &curves, &n));  
            for (jj = 0; jj < n; jj++)  
                UF_CALL(UF_MODL_put_list_item(curve_list, curves[jj]));  
            if (n > 0) UF_free(curves);  
        }  
        UF_CALL(UF_MODL_ask_face_type(face, &type));  
        if (type == UF_MODL_BLENDING_FACE)  
        {  
            ECHO("You cannot create isocurves of constant V on a blend face.\n");  
        }  
        else  
        {  
            for (ii = 0, v = uv_minmax[2]; ii < nv; ii++, v = v + dv)  
            {  
                UF_CALL(UF_MODL_create_isocurve(face, 2, v, tol, &curves, &n));  
                for (jj = 0; jj < n; jj++)  
                    UF_CALL(UF_MODL_put_list_item(curve_list, curves[jj]));  
                if (n > 0) UF_free(curves);  
            }  
        }  
        return make_an_array(&curve_list, isos);  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        UF_DISP_refresh();  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            nu = 10,  
            nv = 5;  
        tag_t  
            *curves,  
            face;  
        while ((face = select_a_face("Isocurve Grid")) != NULL_TAG)  
        {  
            UF_DISP_refresh();  
            show_face_parm_dirs(face);  
            if (prompt_for_two_integers("Grid of points", "No. in U",  
                "No. in V", &nu, &nv))  
            n = create_isocurve_grid_on_face(face, nu, nv, &curves);  
            number_objects(curves, n);  
            if (n > 0) UF_free(curves);  
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

> 这段代码实现了在NX中选择一个面，然后在该面上创建等参数线(等U线和等V线)网格的功能。
>
> 主要功能包括：
>
> 1. 使用UF_UI库中的函数，创建选择对话框，允许用户选择一个面。
> 2. 获取用户输入的U线和V线的数量，作为等参数线的数量。
> 3. 在选定的面上，根据输入的U线和V线数量，使用UF_MODL库中的函数，创建等U线和等V线的等参数线。
> 4. 使用UF_DISP库中的函数，高亮显示创建的等参数线，并为每条等参数线显示编号。
> 5. 使用UF_UNDO库中的函数，设置撤销标记，以便用户可以撤销操作。
> 6. 在函数 ufusr 中初始化和终止NX Open API。
> 7. 提供了 ufusr_ask_unload 函数，以便卸载用户自定义程序。
>
> 总的来说，这段代码通过NX Open API实现了交互式创建等参数线网格的功能，为用户提供了一个实用的工具。
>
