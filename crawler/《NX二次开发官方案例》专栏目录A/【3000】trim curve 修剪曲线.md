### 【3000】trim curve 修剪曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
    #include <uf_sket.h>  
    #include <uf_part.h>  
    #include <uf_group.h>  
    #include <uf_assem.h>  
    #include <uf_undo.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
        int  
            subtype,  
            type;  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
        if (type == UF_point_type)  
        {  
            UF_CALL(UF_CURVE_ask_point_data(obj, loc));  
            return;  
        }  
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
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[6] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 6, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static void display_temporary_asterisk(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    static tag_t select_point_or_point_on_curve(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
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
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static int ask_all_sketches_curves(tag_t **curves, tag_t **sketches)  
    {  
        int  
            cnt = 0,  
            ii,  
            n;  
        tag_t  
            *geoms,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_geoms_of_sketch(sketch, &n, &geoms));  
            if (n > 0)  
            {  
                *curves = realloc((void *)*curves, (cnt+n) * sizeof(tag_t));  
                *sketches = realloc((void *)*sketches, (cnt+n) * sizeof(tag_t));  
                for (ii = 0; ii < n; ii++)  
                {  
                    (*curves)[cnt + ii] = geoms[ii];  
                    (*sketches)[cnt + ii] = sketch;  
                }  
                UF_free(geoms);  
                cnt = cnt + n;  
            }  
        }  
        return cnt;  
    }  
    static tag_t ask_sketch_of_curve(tag_t curve, tag_t *sketches, tag_t *curves,  
        int n)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n; ii++)  
            if (curve == curves[ii]) return sketches[ii];  
        return NULL_TAG;  
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
            ECHO("uf5947 returned %d\n", resp);  
            return NULL_TAG;  
        }  
    /*  Transformed copy will not be in group of original 里海译:Transformed copy will not be in group of original */  
        UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
        if (group != NULL_TAG)  
            UF_CALL(UF_GROUP_add_member_to_group(new_object, group));  
    /*  Transformed copy will not be in reference sets of original 里海译:经过变换的副本将不会在原始参考集中。 */  
        UF_CALL(UF_ASSEM_ask_ref_sets(object, &n_ref_sets, &ref_sets));  
        if (n_ref_sets > 0)  
        {  
            for (ii = 0; ii < n_ref_sets; ii++)  
                UF_CALL(UF_ASSEM_add_ref_set_members(ref_sets[ii], 1, &new_object));  
            UF_free(ref_sets);  
        }  
        return new_object;  
    }  
    static UF_UNDO_mark_id_t set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
        return mark_id;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            bound,  
            curve,  
            curve_copy,  
            *curves = NULL,  
            sketch,  
            *sketches = NULL;  
        double  
            int_point[3],  
            ref_point[3];  
        char  
            sketch_name[MAX_ENTITY_NAME_SIZE+1];  
        n = ask_all_sketches_curves(&curves, &sketches);  
        while (((curve = select_point_on_curve("Select curve to trim", ref_point))  
                    != NULL_TAG) &&  
               ((bound = select_point_or_point_on_curve("Select bounding curve", int_point))  
                    != NULL_TAG))  
        {  
            set_undo_mark("Trim curve");  
            sketch = ask_sketch_of_curve(curve, sketches, curves, n);  
            if (sketch != NULL_TAG)  
            {  
                curve_copy = create_duplicate_copy_of_object(curve);  
                UF_CALL(UF_CURVE_edit_trim_curve(curve_copy, bound, ref_point,  
                    int_point, UF_CURVE_NATURAL_SHAPE));  
                UF_CALL(UF_OBJ_delete_object(curve));  
                UF_CALL(UF_OBJ_ask_name(sketch, sketch_name));  
                UF_CALL(UF_SKET_initialize_sketch(sketch_name, &sketch));  
                UF_CALL(UF_SKET_add_objects(sketch, 1, &curve_copy));  
                UF_CALL(UF_SKET_terminate_sketch());  
            }  
            else  
                UF_CALL(UF_CURVE_edit_trim_curve(curve, bound, ref_point,  
                    int_point, UF_CURVE_NATURAL_SHAPE));  
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

> 这段代码是NX Open API开发的二次开发代码，主要实现了以下功能：
>
> 1. 错误处理：使用了宏UF_CALL来包装UF函数调用，并在调用失败时打印错误信息。
> 2. 视图变换：实现了map_view2abs和map_abs2view两个函数，用于将坐标系从视图坐标系转换到绝对坐标系，反之亦然。
> 3. 选择点或曲线上的点：实现了select_point_or_point_on_curve函数，用于选择点或曲线上的点，并获取其坐标。
> 4. 选择曲线：实现了select_point_on_curve函数，用于选择曲线，并获取其上的一个点。
> 5. 遍历所有草图曲线：实现了ask_all_sketches_curves函数，用于遍历所有草图中的曲线。
> 6. 创建曲线副本：实现了create_duplicate_copy_of_object函数，用于创建曲线的副本。
> 7. 设置撤销标记：实现了set_undo_mark函数，用于设置撤销标记。
> 8. 曲线修剪：do_it函数实现了选择曲线和边界曲线，然后修剪曲线的功能。如果曲线在草图中，则先复制曲线，然后修剪复制后的曲线，最后删除原曲线，并将复制曲线添加到草图中。
> 9. 用户自定义函数：实现了ufusr和ufusr_ask_unload两个函数，用于程序的初始化、执行和卸载。
>
> 总体来说，这段代码通过NX Open API实现了曲线的修剪功能，并考虑了曲线是否在草图中的情况，通过撤销标记支持撤销操作。
>
