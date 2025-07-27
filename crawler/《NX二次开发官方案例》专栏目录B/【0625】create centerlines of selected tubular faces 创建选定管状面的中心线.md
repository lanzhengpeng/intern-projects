### 【0625】create centerlines of selected tubular faces 创建选定管状面的中心线

#### 代码

```cpp
    /*HEAD CREATE_CENTERLINES_OF_SELECTED_TUBULAR_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
    #include <uf_eval.h>  
    #include <uf_vec.h>  
    #include <uf_undo.h>  
    #include <stdarg.h>  
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
    static int mask_for_tubular_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_SWEPT_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_TOROIDAL_FACE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static int select_tubular_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_tubular_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_arc;  
        int  
            ii,  
            n_arcs,  
            n_faces,  
            n_iso,  
            type;  
        tag_t  
            cline,  
            *faces,  
            *iso;  
        double  
            mid_point[3],  
            tol,  
            uvs[4];  
        uf_list_p_t  
            edge_list,  
            tmp;  
        UF_EVAL_p_t  
            eval;  
        UF_EVAL_arc_t  
            arc;  
        UF_CURVE_line_t  
            coords;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while ((n_faces = select_tubular_faces("Select faces to create centerline",  
            &faces)) != 0)  
        {  
            set_undo_mark("Create Tube Centerlines");  
            for (ii = 0; ii < n_faces; ii++)  
            {  
                UF_CALL(UF_MODL_ask_face_edges(faces[ii], &edge_list));  
                n_arcs = 0;  
                for (tmp = edge_list; tmp != NULL; tmp = tmp->next)  
                {  
                    UF_CALL(UF_EVAL_initialize(tmp->eid, &eval));  
                    UF_CALL(UF_EVAL_is_arc(eval, &is_arc));  
                    if (!is_arc) continue;  
                    n_arcs++;  
                    UF_CALL(UF_EVAL_ask_arc(eval, &arc));  
                    if (n_arcs == 1)  
                        UF_VEC3_copy(arc.center, coords.start_point);  
                    else if (n_arcs == 2)  
                        UF_VEC3_copy(arc.center, coords.end_point);  
                    UF_CALL(UF_EVAL_free(eval));  
                }  
                UF_CALL(UF_MODL_delete_list(&edge_list));  
                UF_CALL(UF_MODL_ask_face_type(faces[ii], &type));  
                if (type == UF_MODL_TOROIDAL_FACE)  
                {  
                    UF_CALL(UF_MODL_ask_face_uv_minmax(faces[ii], uvs));  
                    if (fabs((fabs(uvs[3] - uvs[2]) - TWOPI)) < tol)  
                        UF_CALL(UF_MODL_create_isocurve(faces[ii], 1,  
                            (uvs[0] + uvs[1])/2, tol, &iso, &n_iso));  
                    else  
                        UF_CALL(UF_MODL_create_isocurve(faces[ii], 2,  
                            (uvs[2] + uvs[3])/2, tol, &iso, &n_iso));  
                    UF_CALL(UF_EVAL_initialize(iso[0], &eval));  
                    UF_CALL(UF_EVAL_ask_arc(eval, &arc));  
                    UF_VEC3_copy(arc.center, mid_point);  
                    UF_CALL(UF_EVAL_free(eval));  
                    UF_CALL(UF_OBJ_delete_array_of_objects(n_iso, iso, NULL));  
                    UF_free(iso);  
                    UF_CALL(UF_CURVE_create_arc_thru_3pts(1, coords.start_point,  
                        mid_point, coords.end_point, &cline));  
                }  
                else  
                    UF_CALL(UF_CURVE_create_line(&coords, &cline));  
                UF_CALL(UF_OBJ_set_font(cline, UF_OBJ_FONT_CENTERLINE));  
                UF_CALL(UF_OBJ_set_line_width(cline, UF_OBJ_WIDTH_THIN));  
            }  
            UF_free(faces);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 选择管状面：通过自定义选择过滤器，让用户可以选择管状面，包括圆柱面、扫掠面和环面。
> 2. 创建中心线：对选中的管状面，创建对应的中心线。如果是环面，会先创建一个通过圆环中心点的等参数线，然后连接管子的两个端点，创建一个弧线作为中心线。如果是其他管状面，会直接连接两个端点创建直线作为中心线。
> 3. 设置中心线属性：将创建的中心线设置为细线，并添加中心线字体。
> 4. 提供撤销功能：在创建中心线前后添加撤销标记，以便用户可以撤销操作。
> 5. 释放资源：在选择完面后释放内存，避免内存泄露。
> 6. 初始化和终止NX：在代码执行前后，进行NX的初始化和终止操作。
> 7. 提供卸载函数：让NX在卸载这个代码模块时可以立即卸载，而不是等待。
>
> 整体来说，这段代码实现了在NX中创建管状面的中心线的功能，具有一定的实用性和扩展性。
>
