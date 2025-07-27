### 【0455】chain edges 选择一个边链，然后连续高亮显示链中的边

#### 代码

```cpp
    /*HEAD CHAIN_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void ask_pos_on_obj(tag_t obj, double loc[3])  
    {  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        map_abs2view(loc);  
        lp.start_point[0] = loc[0];  
        lp.start_point[1] = loc[1];  
        lp.start_point[2] = loc[2] + 1000;  
        lp.end_point[0] = loc[0];  
        lp.end_point[1] = loc[1];  
        lp.end_point[2] = loc[2] - 1000;  
        map_view2abs(lp.start_point);  
        map_view2abs(lp.end_point);  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
    }  
    static void ask_edge_close_endpoint(tag_t edge, double pos[3])  
    {  
        int  
            cnt;  
        double  
            parm[3],  
            end[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_edge_verts(edge, start, end, &cnt));  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &edge));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, start, &parm[0], start));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, end, &parm[2], end));  
        UF_CALL(UF_OBJ_delete_object(edge));  
        if (fabs(parm[1] - parm[0]) < fabs(parm[2] - parm[1]))  
             UF_VEC3_copy(start, pos);  
        else UF_VEC3_copy(end, pos);  
    }  
    static void ask_edge_far_endpoint(tag_t edge, double pos[3])  
    {  
        int  
            cnt;  
        double  
            parm[3],  
            end[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_edge_verts(edge, start, end, &cnt));  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &edge));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, start, &parm[0], start));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, end, &parm[2], end));  
        UF_CALL(UF_OBJ_delete_object(edge));  
        if (fabs(parm[1] - parm[0]) > fabs(parm[2] - parm[1]))  
             UF_VEC3_copy(start, pos);  
        else UF_VEC3_copy(end, pos);  
    }  
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
    static tag_t select_an_edge_endpoint(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, cp);  
            ask_edge_close_endpoint(object, cp);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void chain_next_edge(tag_t edge, double pos[3])  
    {  
        int  
            cnt;  
        uf_list_p_t  
            last,  
            next;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_ask_display_properties(edge, &disp_props));  
        if (!disp_props.highlight_status)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, TRUE));  
            UF_CALL(UF_MODL_ask_cnnc_edges(edge, pos, &next, &last));  
            if (next)  
            {  
                UF_CALL(UF_MODL_ask_list_count(next, &cnt));  
                if (cnt)  
                {  
                    UF_CALL(UF_MODL_ask_list_item(next, 0, &edge));  
                    ask_edge_far_endpoint(edge, pos);  
                    chain_next_edge(edge, pos);  
                }  
                UF_free(next);  
                UF_free(last);  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            edge;  
        double  
            pos[3];  
        while ((edge=select_an_edge_endpoint("Select chain start", pos))!=NULL_TAG)  
            chain_next_edge(edge, pos);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，其主要功能是实现一个链式选择功能，允许用户选择一个边线，并自动高亮显示与其相连的后续边线。
>
> 具体来说，代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败的信息。
> 2. 定义了两个坐标转换函数map_abs2view和map_view2abs，用于将绝对坐标系下的点转换到当前视图坐标系下，以及反向转换。
> 3. 定义了ask_pos_on_obj函数，用于获取对象上的一个点位置。
> 4. 定义了ask_edge_close_endpoint和ask_edge_far_endpoint函数，用于获取边线靠近或远离给定点的端点。
> 5. 定义了mask_for_edges函数，用于设置选择过滤器，仅允许选择边线。
> 6. 定义了select_an_edge_endpoint函数，用于选择一个边线的端点。
> 7. 定义了chain_next_edge函数，用于递归调用，高亮显示与给定边线相连的后续边线。
> 8. 定义了do_it函数，用于循环调用select_an_edge_endpoint和chain_next_edge，实现链式选择。
> 9. ufusr函数是NX二次开发的入口函数，在这里调用do_it函数实现链式选择。
> 10. ufusr_ask_unload函数用于处理插件卸载。
>
> 综上所述，这段代码实现了链式选择边线的功能，并提供了错误处理、坐标转换、对象选择等辅助功能。
>
