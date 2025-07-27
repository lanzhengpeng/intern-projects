### 【2930】subdivide selected curve 选择曲线，在指定点处将其一分为二

#### 代码

```cpp
    /*HEAD SUBDIVIDE_SELECTED_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_curve.h>  
    #include <uf_trns.h>  
    #include <uf_group.h>  
    #include <uf_assem.h>  
    #include <uf_vec.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是一个在V18版本中新增的函数。它用于打印系统日志。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t create_duplicate_copy_of_object(tag_t object)  
    {  
        int  
            ii,  
            n_ref_sets,  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            group,  
            new_object,  
            *ref_sets;  
        double  
            mx[12],  
            zero_delta[3] = { 0, 0, 0 };  
        FTN(uf5943)(zero_delta, mx);  
        FTN(uf5947)(mx, &object, &one, &two, &zero, &two, &new_object, NULL, &resp);  
        if (resp != 0)  
        {  
            return NULL_TAG;  
        }  
    /*  Transformed copy will not be in group of original 里海译:Transformed copy will not be in group of original */  
        UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
        if (group != NULL_TAG)  
            UF_CALL(UF_GROUP_add_member_to_group(new_object, group));  
    /*  Transformed copy will not be in reference sets of original 里海译:翻译：变体副本不会出现在原版的参考集中。

 */  
        UF_CALL(UF_ASSEM_ask_ref_sets(object, &n_ref_sets, &ref_sets));  
        if (n_ref_sets > 0)  
        {  
            for (ii = 0; ii < n_ref_sets; ii++)  
                UF_CALL(UF_ASSEM_add_ref_set_members(ref_sets[ii], 1, &new_object));  
            UF_free(ref_sets);  
        }  
        return new_object;  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            junk[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk,  
            junk, junk));  
    }  
    static tag_t subdivide_curve(tag_t curve, double *loc)  
    {  
        tag_t  
            new_curve = create_duplicate_copy_of_object(curve),  
            point;  
        double  
            curve_pnt[3],  
            end[3],  
            junk[3],  
            parm,  
            start[3];  
        UF_CALL(UF_CURVE_create_point(loc, &point));  
        UF_CALL(UF_MODL_ask_curve_parm(curve, loc, &parm, curve_pnt));  
        UF_CALL(UF_MODL_ask_curve_props(curve, parm/2, start, junk, junk,  
            junk, junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, (parm + 1)/2, end, junk, junk,  
            junk, junk, junk));  
        UF_CALL(UF_CURVE_edit_trim_curve(curve, point, end, loc,  
            UF_CURVE_NATURAL_SHAPE));  
        if (new_curve)  
            UF_CALL(UF_CURVE_edit_trim_curve(new_curve, point, start, loc,  
                UF_CURVE_NATURAL_SHAPE));  
        UF_CALL(UF_OBJ_delete_object(point));  
        return new_curve;  
    }  
    static int ask_curve_close_end(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            parm;  
        UF_CALL(UF_MODL_ask_curve_parm(curve, pos, &parm, pos));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, parm, UF_MODL_UNITS_PART,  
            &dist[0]));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, parm, 1.0, UF_MODL_UNITS_PART,  
            &dist[1]));  
        if (dist[0] < dist[1]) return 1;  
        else return 2;  
    }  
    static void trim_curve_to_two_points(tag_t curve, double *loc1, double *loc2)  
    {  
        tag_t  
            point1,  
            point2;  
        double  
            end[3],  
            start[3];  
        ask_curve_ends(curve, start, end);  
        UF_CALL(UF_CURVE_create_point(loc1, &point1));  
        UF_CALL(UF_CURVE_create_point(loc2, &point2));  
        if (ask_curve_close_end(curve, loc1) == 1)  
        {  
            UF_CALL(UF_CURVE_edit_trim_curve(curve, point1, start, loc1,  
                UF_CURVE_NATURAL_SHAPE));  
            UF_CALL(UF_CURVE_edit_trim_curve(curve, point2, end, loc2,  
                UF_CURVE_NATURAL_SHAPE));  
        }  
        else  
        {  
            UF_CALL(UF_CURVE_edit_trim_curve(curve, point2, start, loc2,  
                UF_CURVE_NATURAL_SHAPE));  
            UF_CALL(UF_CURVE_edit_trim_curve(curve, point1, end, loc1,  
                UF_CURVE_NATURAL_SHAPE));  
        }  
        UF_CALL(UF_OBJ_delete_object(point1));  
        UF_CALL(UF_OBJ_delete_object(point2));  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void display_temporary_asterisk(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            is_so,  
            ok = TRUE;  
        int  
            subtype,  
            status,  
            type;  
        tag_t  
            curve,  
            extra_piece,  
            joined,  
            new_curve;  
        double  
            junk[3],  
            parm[2],  
            pos[3],  
            pos2[3];  
        uf_list_p_t  
            curve_list;  
        while (((curve = select_a_curve("Select curve to divide")) != NULL_TAG)  
            && specify_point("Select split point", pos))  
        {  
            display_temporary_asterisk(pos);  
            if (!UF_CALL(UF_SO_is_so(curve, &is_so)) && is_so)  
            {  
                uc1601("Defining points and associated dimensions will be deleted",  
                    TRUE);  
                if (ask_yes_or_no(  
                    "Creation parameters will be removed from curve - OK?", &ok)  
                    && ok)  
                {  
                    UF_CALL(UF_MODL_create_list(&curve_list));  
                    UF_CALL(UF_MODL_put_list_item(curve_list, curve));  
                    UF_CALL(UF_MODL_delete_object_parms(curve_list));  
                    UF_CALL(UF_MODL_delete_list(&curve_list));  
                }  
                else continue;  
            }  
            if (!UF_MODL_ask_curve_closed(curve))  
                new_curve = subdivide_curve(curve, pos);  
            else  
            {  
                if (specify_point("Closed curve - select 2nd split point", pos2))  
                {  
                    display_temporary_asterisk(pos2);  
                    UF_CALL(UF_OBJ_ask_type_and_subtype(curve, &type, &subtype));  
                    if (type == UF_spline_type)  
                    {  
                        UF_CALL(UF_MODL_ask_curve_parm(curve,pos, &parm[0],junk));  
                        UF_CALL(UF_MODL_ask_curve_parm(curve,pos2, &parm[1],junk));  
                        if (parm[1] < parm[0])  
                        {  
                            UF_VEC3_copy(pos, junk);  
                            UF_VEC3_copy(pos2, pos);  
                            UF_VEC3_copy(junk, pos2);  
                        }  
                        new_curve = subdivide_curve(curve, pos);  
                        extra_piece = subdivide_curve(new_curve, pos2);  
                        UF_CALL(UF_MODL_create_list(&curve_list));  
                        UF_CALL(UF_MODL_put_list_item(curve_list, curve));  
                        UF_CALL(UF_MODL_put_list_item(curve_list, extra_piece));  
                        UF_CALL(UF_CURVE_create_joined_curve(curve_list, 2,  
                            &joined, &status));  
                        UF_CALL(UF_OBJ_replace_object_data(curve, joined));  
                        UF_CALL(UF_OBJ_delete_object(extra_piece));  
                        UF_CALL(UF_MODL_delete_list(&curve_list));  
                    }  
                    else  
                    {  
                        new_curve = subdivide_curve(curve, pos);  
                        trim_curve_to_two_points(curve, pos, pos2);  
                        trim_curve_to_two_points(new_curve, pos, pos2);  
                    }  
                }  
            }  
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

> 这段代码实现了NX中的曲线分割功能。主要步骤如下：
>
> 1. 选择曲线：用户选择要分割的曲线。
> 2. 指定分割点：用户指定分割曲线的位置点。
> 3. 创建分割曲线：在分割点处创建新的曲线，原始曲线和新曲线分别被分割。
> 4. 处理封闭曲线：若原始曲线为封闭曲线，则用户需要指定第二个分割点，并调整分割点顺序。然后分别创建两条新曲线，并连接它们以替代原始曲线。
> 5. 处理SO曲线：若原始曲线为SO曲线，则删除其定义点和相关参数，然后进行分割。
> 6. 循环执行：重复上述步骤，直到用户不再选择曲线为止。
>
> 此外，代码还实现了错误处理、提示信息显示、临时点显示等功能。整体流程清晰，实现了基本的曲线分割功能。
>
