### 【2929】subdivide cylindrical face 细分圆柱面

#### 代码

```cpp
    /*HEAD SUBDIVIDE_CYLINDRICAL_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
    /* qq3123197280 */  
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void display_temporary_colored_text(char *text, double loc[3], int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_list_objects(uf_list_p_t objects)  
    {  
        int  
            ii = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        uf_list_p_t  
            tmp;  
        for (tmp = objects; tmp != NULL; tmp = tmp->next)  
        {  
            if (UF_OBJ_ask_name_origin(tmp->eid, loc))  
            {  
                UF_CALL(UF_OBJ_set_name(tmp->eid, "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(tmp->eid, loc));  
                UF_CALL(UF_OBJ_delete_name(tmp->eid));  
            }  
            sprintf(msg, "%d", ++ii);  
            display_temporary_colored_text(msg, loc, UF_OBJ_BLUE);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj, kk,  
            n_crvs,  
            n_isos[2];  
        tag_t  
            *crvs,  
            div_face,  
            *isos[2],  
            face;  
        double  
            distol,  
            uvs[4];  
        uf_list_p_t  
            face_list;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_MODL_ask_distance_tolerance(&distol);  
        while ((face = select_cylindrical_face("Select cyl face to subdivide"))  
            != NULL_TAG)  
        {  
             UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
             if ((uvs[0] == 0.0) && (uvs[1] == TWOPI))  
             {  
                 UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Create isocurves",  
                    &mark_id));  
                 UF_CALL(UF_MODL_create_isocurve(face, 1, 0, distol, &(isos[0]),  
                    &n_isos[0]));  
                 UF_CALL(UF_MODL_create_isocurve(face, 1, PI, distol, &(isos[1]),  
                    &n_isos[1]));  
                 UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Subdivide face",  
                    &mark_id));  
                n_crvs = n_isos[0]+n_isos[1];  
                UF_CALL(allocate_memory(n_crvs * sizeof(tag_t), (void **)&crvs));  
                for (ii = 0, kk = 0; ii < 2; ii++)  
                    for (jj = 0; jj < n_isos[ii]; jj++)  
                        crvs[kk++] = isos[ii][jj];  
                 if (!UF_CALL(UF_MODL_create_subdiv_face(crvs, n_crvs, face, 1,  
                    NULL, &div_face)))  
                {  
                    UF_CALL(UF_MODL_ask_feat_faces(div_face, &face_list));  
                    number_list_objects(face_list);  
                    UF_CALL(UF_MODL_delete_list(&face_list));  
                }  
                else  
                {  
                    UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
                    UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
                }  
                UF_free(crvs);  
            }  
            else  
                 uc1601("Not a full cylinder", TRUE);  
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

> 这段代码是用于在NX中通过二次开发进行圆柱面划分的一个程序。
>
> 主要功能包括：
>
> 1. 选择圆柱面：使用对话框让用户选择一个圆柱面，并检查其是否是一个完整的圆柱面（即u方向范围是0到2π）。
> 2. 创建等参数线：在圆柱面上创建两条等参数线，一条在u=0处，另一条在u=π处。
> 3. 划分圆柱面：使用创建的等参数线作为边界，将圆柱面划分成多个小面。
> 4. 标记编号：对新划分的小面进行编号标记，以区分不同的面。
> 5. 撤销功能：提供撤销功能，如果划分失败，可以撤销到划分前的状态。
> 6. 内存管理：动态分配内存，并在划分完成后释放内存。
> 7. 错误处理：具有错误处理机制，能够捕获函数调用失败并输出错误信息。
>
> 总体来说，这段代码实现了对圆柱面进行划分的功能，并包含了选择、划分、标记编号、撤销等完整的流程。
>
