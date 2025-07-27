### 【3001】trim selected curve to two points 将所选曲线修剪为两点

#### 代码

```cpp
    /*HEAD TRIM_SELECTED_CURVE_TO_TWO_POINTS CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
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
    static void display_temporary_asterisk(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_point_on_curve(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, cp);  
            display_temporary_asterisk(cp);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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
    /*  Transformed copy will not be in group of original 里海译:翻译：经过转换的副本将不会在原始组中。 */  
        UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
        if (group != NULL_TAG)  
            UF_CALL(UF_GROUP_add_member_to_group(new_object, group));  
    /*  Transformed copy will not be in reference sets of original 里海译:翻译：转换后的副本不会出现在原始的参考集中。 */  
        UF_CALL(UF_ASSEM_ask_ref_sets(object, &n_ref_sets, &ref_sets));  
        if (n_ref_sets > 0)  
        {  
            for (ii = 0; ii < n_ref_sets; ii++)  
                UF_CALL(UF_ASSEM_add_ref_set_members(ref_sets[ii], 1, &new_object));  
            UF_free(ref_sets);  
        }  
        return new_object;  
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
            middle = FALSE,  
            ok = TRUE;  
        int  
            closed,  
            status;  
        tag_t  
            curve,  
            extra_piece,  
            joined,  
            new_curve;  
        double  
            junk[3],  
            parm[3],  
            pos[3],  
            pos1[3],  
            pos2[3];  
        uf_list_p_t  
            curve_list;  
        while (((curve = select_point_on_curve("Trim curve", pos)) != NULL_TAG) &&  
                specify_point("select 1st trim point", pos1) &&  
                specify_point("select 2nd trim point", pos2))  
        {  
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
            UF_CALL(UF_MODL_ask_curve_parm(curve,  pos, &parm[0],junk));  
            UF_CALL(UF_MODL_ask_curve_parm(curve, pos1, &parm[1],junk));  
            UF_CALL(UF_MODL_ask_curve_parm(curve, pos2, &parm[2],junk));  
            if (parm[2] < parm[1])  
            {  
                UF_VEC3_copy(pos1, junk);  
                UF_VEC3_copy(pos2, pos1);  
                UF_VEC3_copy(junk, pos2);  
            }  
            if ((parm[0] > parm[1]) && (parm[0] < parm[2])) middle = TRUE;  
            closed = UF_MODL_ask_curve_closed(curve);  
            new_curve = subdivide_curve(curve, pos1);  
            extra_piece = subdivide_curve(new_curve, pos2);  
            if (middle)  
            {  
                UF_CALL(UF_OBJ_delete_object(new_curve));  
                if (closed)  
                {  
                    UF_CALL(UF_MODL_create_list(&curve_list));  
                    UF_CALL(UF_MODL_put_list_item(curve_list, curve));  
                    UF_CALL(UF_MODL_put_list_item(curve_list, extra_piece));  
                    UF_CALL(UF_CURVE_create_joined_curve(curve_list, 2,  
                        &joined, &status));  
                    UF_CALL(UF_OBJ_replace_object_data(curve, joined));  
                    UF_CALL(UF_OBJ_delete_object(extra_piece));  
                    UF_CALL(UF_MODL_delete_list(&curve_list));  
                }  
            }  
            else  
            {  
                UF_CALL(UF_OBJ_delete_object(curve));  
                UF_CALL(UF_OBJ_delete_object(extra_piece));  
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

> 根据代码内容，这是段NX的二次开发代码，主要实现的功能是裁剪曲线为两个点。以下是代码的主要功能介绍：
>
> 1. 选择曲线：用户可以通过对话框选择一条曲线，代码会在曲线上显示一个星号标记选择的点。
> 2. 裁剪曲线：用户可以指定两个点作为裁剪的起点和终点。代码会在这两个点之间裁剪曲线，生成新的曲线段。
> 3. 删除原始曲线：如果起点和终点将曲线分为三部分，则代码会删除中间的部分，并将两端合并为新曲线。如果起点和终点在曲线两端，则直接删除原始曲线。
> 4. 删除SO特征：如果曲线有SO特征，代码会提示用户是否删除定义点和相关尺寸。如果用户确认，则会删除这些特征。
> 5. 裁剪中间部分：如果用户指定的两点将曲线分为三部分，则代码会裁剪中间部分，并将两端合并为新曲线。
> 6. 删除临时对象：代码会删除创建的临时对象，如临时点等。
> 7. 错误处理：代码中有完善的错误处理机制，会在出错时给出提示。
> 8. 视图变换：代码中有视图变换相关的函数，用于将绝对坐标转换为视图坐标。
>
> 综上所述，该代码实现了一个简单的曲线裁剪功能，用户可以通过选择曲线和指定两个点来裁剪曲线。代码考虑了多种情况，包括SO特征、曲线闭合等，并具有完善的错误处理机制。
>
