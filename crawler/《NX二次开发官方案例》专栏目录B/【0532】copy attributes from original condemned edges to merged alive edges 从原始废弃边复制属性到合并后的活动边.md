### 【0532】copy attributes from original condemned edges to merged alive edges 从原始废弃边复制属性到合并后的活动边

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
    #include <uf_modl.h>  
    #include <uf_attr.h>  
    #include <uf_obj.h>  
    #include <uf_eval.h>  
    #include <uf_vec.h>  
    #include <uf_mtx.h>  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
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
    static int ask_all_features(tag_t part, tag_p_t *feats)  
    {  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
        return (make_an_array(&feat_list, feats));  
    }  
    static int ask_all_of_type_and_subtype(tag_t part, int type, int subtype,  
        tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_edges(tag_t part, tag_t **edges)  
    {  
        return ask_all_of_type_and_subtype(part, UF_solid_type,  
            UF_solid_edge_subtype, edges);  
    }  
    static int ask_all_condemned_objects_of_type_and_subtype(tag_t part, int type,  
        int subtype, tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if ((this_type == type) && (this_subtype == subtype) &&  
                (UF_OBJ_ask_status(object) == UF_OBJ_CONDEMNED))  
                    UF_MODL_put_list_item(object_list, object);  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_condemned_edges(tag_t part, tag_t **edges)  
    {  
        return ask_all_condemned_objects_of_type_and_subtype(part, UF_solid_type,  
            UF_solid_edge_subtype, edges);  
    }  
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    static void copy_object_attributes(tag_t from_object, tag_t to_object)  
    {  
        int  
            ii = 0;  
        char  
            title[UF_ATTR_MAX_TITLE_LEN+1];  
        UF_ATTR_value_t  
            att;  
        while (!UF_CALL(UF_ATTR_cycle(from_object, &ii, UF_ATTR_any, title, &att))  
            && (ii != 0))  
        {  
            UF_CALL(UF_ATTR_assign(to_object, title, att));  
            if (att.type == UF_ATTR_string) UF_free(att.value.string);  
            if (att.type == UF_ATTR_reference) UF_free(att.value.reference);  
        }  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static logical curve_curve_deviation(tag_t curve1, tag_t curve2, int n_pts,  
        double dist_tol, double angl_tol)  
    {  
        logical  
            is_equal,  
            overlays = TRUE;  
        int  
            ii,  
            n = 0,  
            over_dist = 0,  
            over_angl = 0;  
        double  
            a_angl1,  
            a_angl2,  
            angle1[3],  
            angle2[3],  
            avg_dist = 0.0,  
            avg_angl = 0.0,  
            d_angl,  
            d_dist,  
            limits[2],  
            max_angl = 0.0,  
            max_dist = 0.0,  
            min_angl = TWOPI,  
            min_dist = 1000000,  
            mtx[9],  
            p1,  
            p2,  
            p3,  
            p_step,  
            point1[3],  
            point2[3],  
            point3[3],  
            t_angl = 0,  
            t_dist = 0;  
        UF_EVAL_p_t  
            eval_p1,  
            eval_p2;  
        UF_CALL(UF_EVAL_initialize(curve1, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(curve2, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        if (!is_equal)  
        {  
            UF_CALL(UF_EVAL_ask_limits(eval_p1, limits));  
            p_step = ((limits[1] - limits[0])/(n_pts - 1));  
            for (ii = 0; ii < n_pts; ii++)  
            {  
                p1 = limits[0] + ii * p_step;  
                UF_CALL(UF_EVAL_evaluate(eval_p1, 1, p1, point1, angle1));  
                UF_CALL(UF_EVAL_evaluate_closest_point(eval_p2,point1,&p2,point2));  
            /*  the proximity must be reciprocal to within a half step 译:必须互为半步之遥。 */  
                UF_CALL(UF_EVAL_evaluate_closest_point(eval_p1,point2,&p3,point3));  
                if ((fabs(p1 - p3)) > (p_step/2))  
                {  
                    overlays = FALSE;  
                    continue;  
                }  
                n = n + 1;  
                UF_CALL(UF_EVAL_evaluate(eval_p2, 1, p2, point2, angle2));  
                UF_VEC3_distance(point1, point2, &d_dist);  
                t_dist = t_dist + d_dist;  
                if (d_dist > max_dist) max_dist = d_dist;  
                if ((d_dist < min_dist) || (ii == 0)) min_dist = d_dist;  
                if (d_dist > dist_tol) over_dist = over_dist + 1;  
                UF_CALL(UF_MTX3_initialize_x(angle1, mtx));  
                UF_VEC3_angle_between(angle1, angle2, &mtx[6], &d_angl);  
                a_angl1 = fabs(d_angl - PI);  
                a_angl2 = fabs(d_angl - TWOPI);  
                if (a_angl1 < d_angl) d_angl = a_angl1;  
                if (a_angl2 < d_angl) d_angl = a_angl2;  
                d_angl = d_angl * RADEG;  
                t_angl = t_angl + d_angl;  
                if (d_angl > angl_tol) over_angl = over_angl + 1;  
                if (d_angl > max_angl) max_angl = d_angl;  
                if ((d_angl < min_angl) || (ii == 0)) min_angl = d_angl;  
                if ((d_dist > dist_tol) || (d_angl > angl_tol)) overlays = FALSE;  
            }  
            avg_dist = t_dist / n;  
            avg_angl = t_angl / n;  
        }  
        UF_CALL(UF_EVAL_free(eval_p1));  
        UF_CALL(UF_EVAL_free(eval_p2));  
        return overlays;  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_atts,  
            n_curves,  
            n_edges,  
            n_feats;  
        tag_t  
            alive_edge,  
            curve,  
            *curves,  
            *edges,  
            *feats,  
            part = UF_PART_ask_display_part();  
        double  
            atol,  
            dtol;  
        uf_list_p_t  
            curve_list,  
            edge_list;  
        n_feats = ask_all_features(part, &feats);  
        if (n_feats == 0) return;  
        set_undo_mark("Retrieve old edge attributes");  
    /*  This works for merged edges 译:对于合并的边，这个方法有效。 */  
        n_edges =  ask_all_condemned_edges(part, &edges);  
        for (ii = 0; ii < n_edges; ii++)  
        {  
            UF_CALL(UF_MODL_ask_alive_edge(edges[ii], &alive_edge));  
            if (alive_edge != NULL_TAG)  
                copy_object_attributes(edges[ii], alive_edge);  
        }  
        if (n_edges > 0) UF_free(edges);  
    /*  This works for replaced edges - ie. Sew 译:对于替换的边，即缝合，这种方法是有效的。 */  
        UF_MODL_ask_distance_tolerance(&dtol);  
        UF_MODL_ask_angle_tolerance(&atol);  
        UF_CALL(UF_MODL_create_list(&edge_list));  
        UF_CALL(UF_MODL_create_list(&curve_list));  
        for (ii = 0; ii < n_feats; ii++)  
        {  
            UF_CALL(UF_MODL_set_current_feature(feats[ii]));  
            n_edges = ask_all_edges(part, &edges);  
            for (jj = 0; jj < n_edges; jj++)  
            {  
                if (!is_item_on_list(edge_list, edges[jj]))  
                {  
                    UF_CALL(UF_MODL_put_list_item(edge_list, edges[jj]));  
                    UF_CALL(UF_ATTR_count_attributes(edges[jj], UF_ATTR_any,  
                        &n_atts));  
                    if (n_atts > 0)  
                    {  
                        UF_CALL(UF_MODL_create_curve_from_edge(edges[jj], &curve));  
                        UF_CALL(UF_MODL_put_list_item(curve_list, curve));  
                        copy_object_attributes(edges[jj], curve);  
                    }  
                }  
            }  
            if ((ii < (n_feats-1)) && (n_edges > 0)) UF_free(edges);  
        }  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
        n_curves = make_an_array(&curve_list, &curves);  
        for (ii = 0; ii < n_edges; ii++)  
            for (jj = 0; jj < n_curves; jj++)  
            {  
                if (curve_curve_deviation(edges[ii], curves[jj], 10, dtol, atol))  
                    copy_object_attributes(curves[jj], edges[ii]);  
            }  
        if (n_edges > 0) UF_free(edges);  
        UF_CALL(UF_OBJ_delete_array_of_objects(n_curves, curves, NULL));  
        if (n_curves > 0) UF_free(curves);  
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

> 这段NX Open C++代码的主要功能包括：
>
> 1. 获取当前显示部件的所有特征，并将这些特征存储在数组中。
> 2. 获取部件中所有被删除的边，并将这些边的属性复制到对应的活边。
> 3. 创建边的列表，并为每条边创建一个曲线，并将边的属性复制到曲线。
> 4. 比较每条活边和曲线，如果它们足够相似，则将曲线的属性复制回活边。
> 5. 删除创建的曲线，释放相关内存。
>
> 这段代码的目的是在NX特征建模过程中，通过比较和复制属性来恢复被删除或修改的边的原始属性信息。通过设置撤销标记，用户可以随时撤销操作。代码中包含错误处理、内存分配和列表操作等辅助功能。
>
