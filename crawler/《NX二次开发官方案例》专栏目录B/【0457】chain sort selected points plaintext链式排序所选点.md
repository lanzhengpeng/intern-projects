### 【0457】chain sort selected points plaintext链式排序所选点

#### 代码

```cpp
    /*HEAD CHAIN_SORT_SELECTED_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_points(char *prompt, tag_t **points)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select points", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_points, NULL, &resp, &cnt, points));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*points)[ii], FALSE));  
        return cnt;  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void chain_sort_points(tag_t start, tag_t end, tag_t *pts, int n_pts)  
    {  
        int  
            ii;  
        tag_t  
            best_point;  
        double  
            best_dist,  
            last_point[3],  
            this_dist,  
            this_point[3];  
        uf_list_p_t  
            chain_list,  
            point_list,  
            temp;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        UF_CALL(UF_MODL_create_list(&chain_list));  
        UF_CALL(UF_MODL_put_list_item(chain_list, start));  
        UF_CALL(UF_CURVE_ask_point_data(start, last_point));  
        for (ii = 0; ii < n_pts; ii++)  
            if ((pts[ii] != start) && (pts[ii] != end))  
                UF_CALL(UF_MODL_put_list_item(point_list, pts[ii]));  
        while ((n_pts > 2) && (point_list != NULL))  
        {  
            best_dist = -1;  
            for (temp = point_list; temp != NULL; temp = temp->next)  
            {  
                UF_CALL(UF_CURVE_ask_point_data(temp->eid, this_point));  
                UF_VEC3_distance(last_point, this_point, &this_dist);  
                if ((best_dist == -1) || (this_dist < best_dist))  
                {  
                    best_point = temp->eid;  
                    best_dist = this_dist;  
                }  
            }  
            UF_CALL(UF_MODL_put_list_item(chain_list, best_point));  
            UF_CALL(UF_CURVE_ask_point_data(best_point, last_point));  
            UF_CALL(UF_MODL_delete_list_item(&point_list, best_point));  
        }  
        UF_CALL(UF_MODL_put_list_item(chain_list, end));  
        for (ii = 0, temp = chain_list; ii < n_pts; temp = temp->next, ii++)  
            pts[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(&chain_list));  
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
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
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
            n_points;  
        tag_t  
            end,  
            *points,  
            start;  
        while (((n_points = select_points("Chain sort points", &points)) > 0) &&  
               ((start = select_a_point("Select Start Point")) != NULL_TAG) &&  
               ((end = select_a_point("Select End Point")) != NULL_TAG))  
        {  
            chain_sort_points(start, end, points, n_points);  
            UF_DISP_refresh();  
            number_objects(points, n_points);  
            UF_free(points);  
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

> 这段代码是一个NX Open C++应用程序，用于实现链式排序选择点的功能。
>
> 主要功能包括：
>
> 1. 选择点：使用UI函数实现选择点的功能，可以批量选择点或者单独选择起点和终点。
> 2. 链式排序：根据点到起点的距离，对选择的点进行链式排序，从起点到终点依次连接。
> 3. 高亮显示和编号：对排序后的点进行高亮显示和编号，以便用户查看排序结果。
> 4. 错误处理：使用UF_CALL宏进行错误处理，将错误代码、文件和行号打印到系统日志和对话框中。
> 5. 资源释放：在退出前释放资源，如删除临时列表。
> 6. 卸载函数：定义了卸载函数，在应用程序退出时立即卸载。
>
> 总体来说，这是一个比较完整的NX二次开发应用程序，实现了链式排序选择点的功能，并具有完善的错误处理和资源管理。
>
