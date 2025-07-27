### 【1675】number chained collection of points 编号链式点集

#### 代码

```cpp
    /*HEAD NUMBER_CHAINED_COLLECTION_OF_POINTS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，请只回答翻译内容，不要添加任何废话。 */  
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
    static int ask_chain_from_all_points(tag_t start, tag_t end, tag_t **chain)  
    {  
        int  
            n_pts;  
        tag_t  
            best_point,  
            part = UF_PART_ask_display_part(),  
            point = NULL_TAG;  
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
        while ((point = ask_next_point(part, point)) != NULL_TAG)  
            if ((point != start) && (point != end))  
                UF_CALL(UF_MODL_put_list_item(point_list, point));  
        UF_CALL(UF_MODL_ask_list_count(point_list, &n_pts));  
        while ((n_pts > 0) && (point_list != NULL))  
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
        return (make_an_array(&chain_list, chain));  
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
        while (((start = select_a_point("Select Start Point")) != NULL_TAG) &&  
               ((end = select_a_point("Select End Point")) != NULL_TAG))  
        {  
            n_points = ask_chain_from_all_points(start, end, &points);  
            UF_DISP_refresh();  
            number_objects(points, n_points);  
            UF_free(points);  
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

> 这段代码是一个NX Open C++ API的示例程序，其主要功能如下：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX API函数出错时输出错误信息。
> 2. 点选择掩码函数：定义了一个点选择掩码函数mask_for_points，用于在选择点时设置选择掩码。
> 3. 选择点函数：定义了一个选择点函数select_a_point，用于让用户选择一个点。
> 4. 内存分配函数：定义了一个内存分配函数allocate_memory，用于动态分配内存。
> 5. 创建数组函数：定义了一个创建数组函数make_an_array，用于将链表转换为数组。
> 6. 询问链表函数：定义了一个询问链表函数ask_chain_from_all_points，用于在所有点中询问一个从开始点到结束点的最短链表。
> 7. 显示临时文本函数：定义了一个显示临时文本函数display_temporary_text，用于在点位置显示临时文本。
> 8. 编号对象函数：定义了一个编号对象函数number_objects，用于给对象编号。
> 9. 主函数：定义了一个主函数do_it，用于实现从开始点到结束点的最短链表询问和编号。
> 10. UFusr函数：定义了一个UFusr函数，是程序的入口函数，用于初始化和调用主函数。
> 11. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于卸载程序。
>
> 综合来看，该程序利用NX的几何查询功能，实现了从用户指定的开始点到结束点的最短链表询问和编号功能。
>
