### 【1062】demo work around for PR 6525934 演示PR 6525934的解决方法

#### 代码

```cpp
    /*  
        This example demonstrates a work around for PR 6525934.  When adding a  
        modeling view containing labels to a drawing, the labels are no longer  
        copied to the new drawing member view (notes still are.)  If the  
        original program was creating labels in the modeling view and then  
        creating a drawing and adding the view, the steps to work around are:  
     - make a duplicate label at the same time you create the original one  
     - create the drawing (as before)  
     - add the view  (as before)  
     - temporarily convert the duplicate label to model mode (shows in all views)  
     - convert the duplicate label to be view dependent (shown only) in the member view  
    */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_curve.h>  
    #include <uf_view.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_part.h>  
    #include <uf_draw.h>  
    #include <uf_undo.h>  
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
    /* qq3123197280 */  
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_curve_pos(char *prompt, UF_DRF_object_p_t curve,  
        double cp[3])  
    {  
        int  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
            &curve->object_tag, cp, &curve->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(curve->object_tag, cp);  
            UF_CALL(UF_DISP_set_highlight(curve->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void map_abs2wcs(double cc[3])  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static tag_t create_drawing_and_add_current_view(tag_t *drawing)  
    {  
        tag_t  
            member_view,  
            original_view;  
        double  
            center[3],  
            middle[2] = { 11, 8.5 },  
            scale;  
        UF_DRAW_info_t  
            drawing_info = { UF_DRAW_ENGLISH_SIZE, UF_DRAW_C, 1.0, UF_PART_ENGLISH,  
                UF_DRAW_THIRD_ANGLE_PROJECTION };  
        UF_DRAW_view_info_t  
            view_info;  
    /*  UF_DRAW_initialize_view_info sets these defaults:  
        view_info->view_status = UF_DRAW_ACTIVE_VIEW;  
        view_info->anchor_point = NULL_TAG;  
        view_info->view_scale = 1.0;  
        view_info->use_ref_pt = FALSE;  
        view_info->transfer_annotation = TRUE;  
        view_info->inherit_boundary = FALSE;  
        view_info->model_name[0] = '\0';  
        view_info->arrangement_name[0] = '\0';  
    */  
        UF_DRAW_initialize_view_info(&view_info);  
        view_info.use_ref_pt = TRUE;  
        view_info.transfer_annotation = FALSE;  
        UF_CALL(UF_VIEW_ask_work_view(&original_view));  
        WRITENZ(uc6432("", 1));               /*  Fit view 里海译:Fit view的中文翻译为“适应视图”。 */  
        WRITENZ(uc6430("", center, &scale));  /*  Read current center point 里海译:Read current center point */  
        WRITENZ(uc6443("", center, 0.0));     /*  Set it as reference point 里海译:将其设置为参考点 */  
        WRITENZ(uc6450("", "", 0, 0));        /*  Save these changes 里海译:翻译：保存这些更改 */  
        UF_CALL(UF_DRAW_create_drawing("SH1", &drawing_info, drawing));  
        UF_CALL(UF_DRAW_import_view(*drawing, original_view, middle, &view_info,  
            &member_view));  
        return member_view;  
    }  
    static void convert_to_model(tag_t object)  
    {  
        int  
            status;  
        tag_t  
            view_tag;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE];  
        if (!uc6409(object, &status, view_name) && (status == 1))  
        {  
        /*  Try the new call first, if that fails - try the old call too 里海译:首先尝试新的调用，如果失败，也尝试旧的调用。 */  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
            if (UF_CALL(UF_VIEW_convert_to_model(view_tag, object)))  
            {  
                UF_UI_write_listing_window("Using uc6401 instead...\n");  
                UF_CALL(uc6401(view_name, object));  
                UF_UI_write_listing_window("\n");  
            }  
        }  
    }  
    static void convert_to_view(tag_t object, tag_t view)  
    {  
        int  
            status;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE];  
        if (!uc6409(object, &status, view_name) && (status == 0))  
        {  
        /*  Try the new call first, if that fails - try the old call too 里海译:尝试新的调用，如果失败，再尝试旧的调用。 */  
            if (UF_CALL(UF_VIEW_convert_to_view(view, object)))  
            {  
                UF_UI_write_listing_window("Using uc6402 instead...\n");  
                UF_CALL(UF_OBJ_ask_name(view, view_name));  
                UF_CALL(uc6402(view_name, object));  
                UF_UI_write_listing_window("\n");  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            i = 0,  
            mpi[100],  
            part_units;  
        UF_VIEW_type_t  
            vtype;  
        UF_VIEW_subtype_t  
            vstype;  
        tag_t  
            drawing,  
            labels[2],  
            member_view,  
            part = UF_PART_ask_display_part(),  
            work_view;  
        double  
            cp[3],  
            dp[3],  
            mpr[70],  
            pos[3],  
            sf[2] = { 1/25.4, 25.4 };  
        char  
            dia[27],  
            label_text[1][MAX_LINE_LENGTH + 1],  
            rad[27];  
        UF_DRF_object_t  
            curve;  
        UF_DRAW_info_t  
            info;  
        set_undo_mark("TEST IR6526896");  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, rad, dia));  
        UF_CALL(UF_DRF_init_object_structure(&curve));  
        if (select_drf_curve_pos("Select item to label", &curve, cp))  
        {  
            if (select_pos("Indicate origin for label", pos))  
            {  
                map_abs2wcs(pos);  /* required if view is model view 里海译:如果视图是模型视图，则需要翻译。 */  
                UF_CALL(UF_VIEW_ask_work_view(&work_view));  
                UF_CALL(UF_VIEW_ask_type(curve.object_view_tag, &vtype, &vstype));  
                if ((work_view != curve.object_view_tag) &&  
                    (vtype == UF_VIEW_DRAWING_MEMBER_TYPE))  
                {  
                    UF_CALL(UF_VIEW_map_model_to_drawing(curve.object_view_tag,  
                        cp, dp));  
                /*  In case drawing and part units do not match 里海译:如果图纸和部件单位不匹配，该怎么办？ */  
                    UF_CALL(UF_DRAW_ask_drawing_info(NULL_TAG, &info));  
                    UF_CALL(UF_PART_ask_units(part, &part_units));  
                    if (info.units != part_units)  
                        UF_VEC3_scale(sf[2-part_units], dp, dp);  
                }  
                else  
                {  
                    UF_VEC2_copy(cp, dp);  
                }  
                if (dp[0] < pos[0])  
                    mpi[1] = 1;  
                else  
                    mpi[1] = 2;  
                UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad, dia));  
                sprintf(&label_text[0][0], "Label %d", ++i);  
                UF_CALL(UF_DRF_create_label(1, label_text, pos,  
                    UF_DRF_leader_attach_object, &curve, cp, &labels[0]));  
                UF_CALL(UF_DRF_create_label(1, label_text, pos,  
                    UF_DRF_leader_attach_object, &curve, cp, &labels[1]));  
            }  
        }  
        member_view = create_drawing_and_add_current_view(&drawing);  
        convert_to_model(labels[1]);  
        convert_to_view(labels[1], member_view);  
        UF_CALL(UF_DRAW_update_one_view(drawing, member_view));  
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

> 这段代码是NX的二次开发代码，主要实现的功能是在NX中创建一个标注，并在创建绘图时将其添加到绘图视图中。代码的关键步骤包括：
>
> 1. 选择一个曲线，获取其位置信息，并创建一个标注，同时创建标注的副本。
> 2. 创建一个新的绘图，并将当前视图添加到绘图中。
> 3. 将标注的副本转换为模型模式，使其在所有视图中可见。
> 4. 将标注的副本转换为仅在新增的绘图视图成员中可见。
> 5. 更新绘图视图，使新创建的标注显示在绘图视图中。
>
> 通过这个流程，成功实现了在创建绘图时将标注添加到绘图视图中的目标。
>
