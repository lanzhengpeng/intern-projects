### 【0210】ask face parms at all edge midpoints 询问所有边中点的面参数

#### 代码

```cpp
    /*HEAD ASK_FACE_PARMS_AT_ALL_EDGE_MIDPOINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，这句话的意思是：UF_print_syslog是V18版本新增的函数，请直接回答问题，不要添加无关的废话。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static int ask_body_edges(tag_t body, tag_p_t *body_edges)  
    {  
        int  
            err,  
            ii,  
            n;  
        tag_t  
            *edges;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n));  
        *body_edges = (edges = (tag_t *)UF_allocate_memory(n*sizeof(tag_t), &err));  
        if (UF_CALL(err)) return 0;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(edge_list, ii, &edges[ii]));  
            if (UF_ASSEM_is_occurrence(body) && !UF_ASSEM_is_occurrence(edges[ii]))  
                edges[ii] = UF_ASSEM_find_occurrence(  
                    UF_ASSEM_ask_part_occurrence(body), edges[ii]);  
        }  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
        return n;  
    }  
    static void ask_edge_midpoint(tag_t edge, double point[3])  
    {  
        double  
            limits[2];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(edge, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        UF_CALL(UF_EVAL_evaluate(eval, 0, ((limits[0]+limits[1])/2), point, NULL));  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
            UF_DISP_USE_ACTIVE_PLUS, point, &attrib, UF_DISP_POINT));  
        UF_CALL(UF_EVAL_free(eval));  
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
    static int ask_edge_faces(tag_t edge, tag_t **faces)  
    {  
        uf_list_p_t  
            facelist;  
        UF_CALL(UF_MODL_ask_edge_faces(edge, &facelist));  
        return (make_an_array(&facelist, faces));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n_edges,  
            n_faces;  
        tag_t  
            body = NULL_TAG,  
            *edges,  
            *faces,  
            part = UF_PART_ask_display_part();  
        double  
            facept[3],  
            midpt[3],  
            parm[2];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_YELLOW, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            n_edges = ask_body_edges(body, &edges);  
            for (ii = 0; ii < n_edges; ii++)  
            {  
                ask_edge_midpoint(edges[ii], midpt);  
                n_faces = ask_edge_faces(edges[ii], &faces);  
                for (jj = 0; jj < n_faces; jj++)  
                {  
                    UF_CALL(UF_MODL_ask_face_parm(faces[jj], midpt, parm, facept));  
                    UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                        UF_DISP_USE_ACTIVE_PLUS, facept, &attrib, UF_DISP_X));  
                }  
                if (n_faces > 0) UF_free(faces);  
            }  
            if (n_edges > 0) UF_free(edges);  
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

> 这段代码是用于NX的二次开发，主要实现以下功能：
>
> 1. 初始化NX环境，并获取当前显示的部件。
> 2. 遍历部件中的所有实体体，并为每个实体体获取所有的边。
> 3. 遍历每个边，并获取边的中间点。
> 4. 获取每条边两侧的面。
> 5. 在每侧面上，根据边的中间点参数，计算该参数对应的点，并显示该点。
> 6. 释放内存和终止NX环境。
>
> 代码的关键函数包括：
>
> 该代码通过NX的UF接口，实现了对NX模型边和面的遍历，以及点的参数计算和显示，实现了在NX中查看边中点在相邻面上的参数对应点的功能。
>
