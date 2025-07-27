### 【1399】highlight points in part on selected faces 高亮选中面上的零件点

#### 代码

```cpp
    /*HEAD HIGHLIGHT_POINTS_IN_PART_ON_SELECTED_FACES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中新增的函数，其功能是打印系统日志消息。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
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
            ng,  
            n_points;  
        tag_t  
            face,  
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
        while ((face = select_a_face("Select face")) != NULL_TAG)  
        {  
            cnt = 0;  
            UF_CALL(UF_DISP_set_highlight(face, TRUE));  
            for (ii = 0; ii < n_points; ii++)  
            {  
                UF_CALL(UF_CURVE_ask_point_data(points[ii], loc));  
            /*  The new way 里海译:对不起，我无法提供该句子的翻译，因为您未提供原始文本。请提供需要翻译的文本，我将为您翻译。 */  
                UF_CALL(UF_MODL_ask_point_containment(loc, face, &flag));  
            /*  The old way  
                FTN(uf6541)(loc, &face, &ng, &flag);  
                if (!ng)  
            */  
                if (flag != 2)  
                {  
                    cnt++;  
                    UF_CALL(UF_DISP_set_highlight(points[ii], TRUE));  
                }  
            }  
            if (cnt > 0)  
            {  
                sprintf(msg, "%d points on face", cnt);  
                uc1601(msg, TRUE);  
                for (ii = 0; ii < n_points; ii++)  
                    UF_CALL(UF_DISP_set_highlight(points[ii], FALSE));  
            }  
            else  
                uc1601("No points are on the face", TRUE);  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
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

> 这段代码是用于NX的二次开发，其主要功能是在选择的面上高亮显示所有点。具体步骤如下：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了选择面的函数select_a_face，使用UF_UI选择一个面，并返回面的tag。
> 3. 定义了在部件内循环选择点的函数ask_next_point，用于在部件内循环选择下一个点。
> 4. 定义了内存分配函数allocate_memory，用于分配内存。
> 5. 定义了从列表中获取对象数组并释放列表的函数make_an_array。
> 6. 定义了在部件内选择所有点的函数ask_all_points，用于在部件内选择所有点。
> 7. 定义了主体函数do_it，用于执行主要功能。
> 8. 在主体函数中，首先获取当前显示的部件，然后选择部件内的所有点，接着循环选择面，对每个面，判断所有点是否在其上，并在其上的点上高亮显示，最后输出结果。
> 9. 定义了ufusr函数，用于在NX中调用主体函数。
> 10. 定义了卸载函数ufusr_ask_unload。
>
> 通过这段代码，可以在NX中选择一个面，然后在面上高亮显示所有点，实现了对面上点的查询功能。
>
