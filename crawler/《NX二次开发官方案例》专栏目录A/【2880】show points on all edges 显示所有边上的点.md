### 【2880】show points on all edges 显示所有边上的点

#### 代码

```cpp
    /*HEAD SHOW_ CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的功能，请仅回答翻译，不要添加无关内容。

UF_print_syslog在V18版本中是一个新增的功能。 */  
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
            body = NULL_TAG,  
            *edges,  
            part = UF_PART_ask_display_part();  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            n_edges = ask_body_edges(body, &edges);  
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

> 这段代码是用于NX的二次开发代码，主要功能是显示NX零件中所有实体边缘上的点。
>
> 代码的核心流程如下：
>
> 1. 初始化NX环境。
> 2. 获取当前显示的零件，并循环遍历零件中的所有实体。
> 3. 对于每个实体，获取其所有的边。
> 4. 对每条边，显示其起点和终点之间的11个等间距点。
> 5. 释放内存并结束NX环境。
>
> 代码的关键点：
>
> 综上所述，这段代码实现了在NX中显示零件中所有实体边缘上点的功能。
>
