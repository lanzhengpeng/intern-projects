### 【2879】show points on all edges of selected face 显示选定面的所有边上的点

#### 代码

```cpp
    /*HEAD SHOW_POINTS_ON_ALL_EDGES_OF_SELECTED_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的。

请只回答译文，不要添加其他废话。 */  
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
    static int ask_face_edges(tag_t face, tag_t **face_edges)  
    {  
        int  
            err,  
            ii,  
            n;  
        tag_t  
            *edges;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_face_edges(face, &edge_list));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n));  
        *face_edges = (edges = (tag_t *)UF_allocate_memory(n*sizeof(tag_t), &err));  
        if (UF_CALL(err)) return 0;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(edge_list, ii, &edges[ii]));  
            if (UF_ASSEM_is_occurrence(face))  
                edges[ii] = UF_ASSEM_find_occurrence(  
                    UF_ASSEM_ask_part_occurrence(face), edges[ii]);  
        }  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
        return n;  
    }  
    static void show_edge_points(tag_t edge, int n_pts, double sp, double ep)  
    {  
        int  
            ii;  
        double  
            limits[2],  
            p,  
            point[3],  
            tep,  
            tsp;  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(edge, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        tsp = limits[0] + sp * (limits[1] - limits[0]);  
        tep = limits[0] + ep * (limits[1] - limits[0]);  
        for (ii = 0; ii < n_pts; ii++)  
        {  
            p = tsp + ii * ((tep - tsp)/(n_pts - 1));  
            UF_CALL(UF_EVAL_evaluate(eval, 0, p, point, NULL));  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                UF_DISP_USE_ACTIVE_PLUS, point, &attrib, UF_DISP_POINT));  
        }  
        UF_CALL(UF_EVAL_free(eval));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_edges;  
        tag_t  
            face = NULL_TAG,  
            *edges;  
        while ((face = select_a_face("Select face")) != NULL_TAG)  
        {  
            n_edges = ask_face_edges(face, &edges);  
            for (ii = 0; ii < n_edges; ii++)  
                show_edge_points(edges[ii], 11, 0, 1);  
            if (n_edges > 0) UF_free(edges);  
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

> 这是段用于NX的二次开发代码，其主要功能是在NX中选择一个面，并在该面的所有边线上显示点。代码的主要部分包括：
>
> 1. 错误报告函数：用于在函数调用失败时打印错误信息。
> 2. 选择面：使用UF_UI函数来实现选择面的功能。
> 3. 查询面的边：使用UF_MODL函数来查询面的所有边，并将边的数组返回。
> 4. 显示点：使用UF_EVAL函数来计算边线上的点，并使用UF_DISP函数显示这些点。
> 5. 主函数：循环选择面，并在每个面上显示点。
> 6. NX初始化和结束：在ufusr函数中进行NX的初始化和结束操作。
> 7. 卸载函数：返回立即卸载标志。
>
> 通过以上代码，实现了在NX中选择一个面，并在该面的所有边线上显示点，从而帮助用户更直观地查看面的边界情况。
>
