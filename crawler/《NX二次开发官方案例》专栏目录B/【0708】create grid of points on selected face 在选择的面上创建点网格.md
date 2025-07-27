### 【0708】create grid of points on selected face 在选择的面上创建点网格

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_evalsf.h>  
    #include <uf_modl.h>  
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
    static void create_dumb_point_grid_on_face(tag_t face, int nu, int nv)  
    {  
        int  
            ii, jj, type;  
        tag_t  
            point;  
        double  
            du,  
            dv,  
            minmax[4],  
            uv[2];  
        UF_EVALSF_p_t  
            evaluator;  
        UF_MODL_SRF_VALUE_t  
            surf_eval;  
        set_undo_mark("Grid of points");  
        UF_CALL(UF_MODL_ask_face_type(face, &type));  
        UF_CALL(UF_EVALSF_initialize(face, &evaluator));  
        UF_CALL(UF_EVALSF_ask_face_uv_minmax(evaluator, minmax));  
        du = (minmax[1] - minmax[0]) / (nu - 1);  
        dv = (minmax[3] - minmax[2]) / (nv - 1);  
        for (ii = 0, uv[0] = minmax[0]; ii < nu; ii++, uv[0] = uv[0] + du)  
        {  
            for (jj = 0, uv[1] = minmax[2]; jj < nv; jj++, uv[1] = uv[1] + dv)  
            {  
                if (type == UF_MODL_BLENDING_FACE)  
                    UF_CALL(UF_MODL_evaluate_face(face, UF_MODL_EVAL, uv,  
                        &surf_eval));  
                else  
                    UF_CALL(UF_EVALSF_evaluate(evaluator, UF_MODL_EVAL, uv,  
                        &surf_eval));  
                UF_CALL(UF_CURVE_create_point(surf_eval.srf_pos, &point));  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            nu = 10,  
            nv = 5;  
        tag_t  
            face;  
        while ((face = select_a_face("Grid of points")) != NULL_TAG)  
        {  
            show_face_parm_dirs(face);  
            if (prompt_for_two_integers("Grid of points", "No. U",  
                "No. V", &nu, &nv))  
            create_dumb_point_grid_on_face(face, nu, nv);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发程序，主要实现了以下功能：
>
> 1. 定义了ECHO宏，用于输出信息到日志窗口。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并捕获错误码，输出错误信息。
> 3. 实现了mask_for_faces函数，用于设置选择器以选择面特征。
> 4. 实现了select_a_face函数，用于弹出对话框，让用户选择一个面。
> 5. 实现了show_face_parm_dirs函数，用于在面上显示U向和V向参数方向。
> 6. 实现了prompt_for_two_integers函数，用于提示用户输入两个整数。
> 7. 实现了set_undo_mark函数，用于设置撤销标记。
> 8. 实现了create_dumb_point_grid_on_face函数，用于在面上创建一个二维点网格。
> 9. 实现了do_it函数，作为主函数，循环选择面，提示用户输入网格参数，然后在面上创建点网格。
> 10. 实现了ufusr函数，作为程序入口，初始化NX系统，调用do_it函数，然后终止NX系统。
> 11. 实现了ufusr_ask_unload函数，用于在程序卸载时立即卸载。
>
> 综上所述，这个程序实现了在NX中通过用户交互选择面，然后在该面上创建一个二维点网格的功能。
>
