### 【1398】highlight points in part on selected edges 高亮选中边上的零件点

#### 代码

```cpp
    /*HEAD HIGHLIGHT_POINTS_IN_PART_ON_SELECTED_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能，请只回答翻译后的内容，无需添加任何额外信息。 */  
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
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_next_point(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_point_type, &object));  
        return (object);  
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
    static int ask_all_points(tag_t part, tag_t **points)  
    {  
        tag_t  
            point = NULL_TAG;  
        uf_list_p_t  
            point_list;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        while ((point = ask_next_point(part, point)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(point_list, point));  
        return (make_an_array(&point_list, points));  
    }  
    static void do_it(void)  
    {  
        int  
            cnt,  
            flag,  
            ii,  
            n_points;  
        tag_t  
            edge,  
            part = UF_PART_ask_display_part(),  
            *points;  
        double  
            loc[3];  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        n_points = ask_all_points(part, &points);  
        if (n_points == 0)  
        {  
            uc1601("No points found", TRUE);  
            return;  
        }  
        while ((edge = select_an_edge("Select edge")) != NULL_TAG)  
        {  
            cnt = 0;  
            UF_CALL(UF_DISP_set_highlight(edge, TRUE));  
            for (ii = 0; ii < n_points; ii++)  
            {  
                UF_CALL(UF_CURVE_ask_point_data(points[ii], loc));  
                UF_CALL(UF_MODL_ask_point_containment(loc, edge, &flag));  
                if (flag != 2)  
                {  
                    cnt++;  
                    UF_CALL(UF_DISP_set_highlight(points[ii], TRUE));  
                }  
            }  
            if (cnt > 0)  
            {  
                sprintf(msg, "%d points on edge", cnt);  
                uc1601(msg, TRUE);  
                for (ii = 0; ii < n_points; ii++)  
                    UF_CALL(UF_DISP_set_highlight(points[ii], FALSE));  
            }  
            else  
                uc1601("No points are on the edge", TRUE);  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
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

> 这段代码是一个NX Open的二次开发示例，主要用于在NX中高亮显示与选择边相关的点。下面是代码的简要介绍：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 边选择函数：定义了一个边选择函数select_an_edge，使用对话框让用户选择一个边，并返回边的tag。
> 3. 点选择函数：定义了点选择函数ask_next_point和ask_all_points，用于获取部件中的所有点，并将点存储在数组中。
> 4. 内存分配函数：定义了内存分配函数allocate_memory，用于分配内存。
> 5. 点数组创建函数：定义了点数组创建函数make_an_array，用于从链表创建点数组。
> 6. 主要功能函数：定义了主要功能函数do_it，在该函数中循环让用户选择边，然后高亮显示与该边相关的点，并显示相关点的数量。
> 7. 用户函数：定义了用户函数ufusr，初始化NX Open，调用do_it函数，然后终止NX Open。
> 8. 卸载函数：定义了卸载函数ufusr_ask_unload，用于卸载用户函数。
>
> 整体来看，这段代码实现了在NX中高亮显示与选择边相关的点的功能，使用了NX Open提供的图形显示、选择和查询接口。
>
