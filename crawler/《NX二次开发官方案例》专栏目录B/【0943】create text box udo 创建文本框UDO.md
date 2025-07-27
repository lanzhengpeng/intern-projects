### 【0943】create text box udo 创建文本框UDO

#### 代码

```cpp
    /*HEAD CREATE_TEXT_BOX_UDO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
    #include <uf_csys.h>  
    #include <uf_drf.h>  
    #include <uf_vec.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog是V18版本新增的。 */  
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
    static UF_UDOBJ_class_t TB_class_id;  
    DllExport extern UF_UDOBJ_class_t get_TB_class_id(void)  
    {  
        return TB_class_id;  
    }  
    static void map_point_to_matrix(tag_t matrix, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static void map_point_from_matrix(tag_t matrix, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static logical ask_annotation_text_box_abs(tag_t obj,  
        double lower_left_corner[3], double lower_right_corner[3],  
        double upper_left_corner[3], double upper_right_corner[3])  
    {  
        tag_t  
            matrix;  
        double  
            height,  
            length;  
        if (UF_CALL(UF_DRF_ask_annotation_text_box(obj, upper_left_corner,  
            &length, &height))) return FALSE;  
        UF_CALL(UF_CSYS_ask_matrix_of_object(obj, &matrix));  
        map_point_to_matrix(matrix, upper_left_corner);  
        UF_VEC3_copy(upper_left_corner, lower_left_corner);  
        lower_left_corner[1] = upper_left_corner[1] - height;  
        UF_VEC3_copy(lower_left_corner, lower_right_corner);  
        lower_right_corner[0] = lower_left_corner[0] + length;  
        UF_VEC3_copy(lower_right_corner, upper_right_corner);  
        upper_right_corner[1] = lower_right_corner[1] + height;  
        map_point_from_matrix(matrix, lower_left_corner);  
        map_point_from_matrix(matrix, lower_right_corner);  
        map_point_from_matrix(matrix, upper_right_corner);  
        map_point_from_matrix(matrix, upper_left_corner);  
        return TRUE;  
    }  
    static void create_or_edit_curve_rectangle(tag_t lines[4],  
        double lower_left_corner[3], double lower_right_corner[3],  
        double upper_left_corner[3], double upper_right_corner[3])  
    {  
        UF_CURVE_line_t  
            line_data;  
        UF_VEC3_copy(lower_left_corner, line_data.start_point);  
        UF_VEC3_copy(lower_right_corner, line_data.end_point);  
        if (lines[0] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[0]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[0], &line_data));  
        UF_VEC3_copy(lower_right_corner, line_data.start_point);  
        UF_VEC3_copy(upper_right_corner, line_data.end_point);  
        if (lines[1] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[1]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[1], &line_data));  
        UF_VEC3_copy(upper_right_corner, line_data.start_point);  
        UF_VEC3_copy(upper_left_corner, line_data.end_point);  
        if (lines[2] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[2]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[2], &line_data));  
        UF_VEC3_copy(upper_left_corner, line_data.start_point);  
        UF_VEC3_copy(lower_left_corner, line_data.end_point);  
        if (lines[3] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[3]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[3], &line_data));  
    }  
    /* qq3123197280 */  
    DllExport extern void update_func(tag_t update_udo,  
        UF_UDOBJ_link_p_t update_cause)  
    {  
        int  
            n_lines;  
        tag_t  
            *lines;  
        double  
            llc[3],  
            lrc[3],  
            ulc[3],  
            urc[3];  
        if (UF_CALL(UF_initialize())) return;  
        if (ask_annotation_text_box_abs(update_cause->assoc_ug_tag, llc, lrc,  
            ulc, urc))  
        {  
            UF_CALL(UF_UDOBJ_ask_owned_objects(update_udo, &n_lines, &lines));  
            create_or_edit_curve_rectangle(lines, llc, lrc, ulc, urc);  
            UF_free(lines);  
        }  
        UF_terminate();  
    }  
    static void register_udo_class()  
    {  
        UF_CALL(UF_UDOBJ_create_class("TB_test", "Text Box UDO", &TB_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(TB_class_id));  
        UF_CALL(UF_UDOBJ_register_update_cb(TB_class_id, update_func));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select dimension or drafting aid",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            lines[4],  
            udo;  
        double  
            llc[3],  
            lrc[3],  
            ulc[3],  
            urc[3];  
        UF_UDOBJ_link_t  
            link_def = { 3, NULL_TAG, FALSE };  
        if (!get_TB_class_id()) register_udo_class();  
        while ((link_def.assoc_ug_tag =  
            select_a_dim_or_draft("Add Text Box UDO")) != NULL_TAG)  
        {  
            if (!ask_annotation_text_box_abs(link_def.assoc_ug_tag, llc, lrc,  
                ulc, urc)) continue;  
            UF_CALL(UF_UDOBJ_create_udo(get_TB_class_id(), &udo));  
            UF_CALL(UF_UDOBJ_add_links(udo, 1, &link_def));  
            for (ii = 0; ii < 4; ii++) lines[ii] = NULL_TAG;  
            create_or_edit_curve_rectangle(lines, llc, lrc, ulc, urc);  
            UF_CALL(UF_UDOBJ_add_owning_links(udo, 4, lines));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这是段用于在NX中创建文本框用户定义对象(UDO)的代码，主要功能包括：
>
> 1. 定义一个名为"TB_test"的UDO类，并注册到NX中。
> 2. 实现update_func函数，用于在UDO关联的尺寸或注释文本框更新时，同步更新UDO的4条边界线。
> 3. 实现map_point_to_matrix和map_point_from_matrix函数，用于将点从一个坐标系转换到另一个坐标系。
> 4. 实现ask_annotation_text_box_abs函数，用于获取文本框的绝对位置信息。
> 5. 实现create_or_edit_curve_rectangle函数，用于创建或更新4条边界线。
> 6. 在ufsta函数中注册UDO类。
> 7. 实现mask_for_dim_or_draft和select_a_dim_or_draft函数，用于选择尺寸或注释对象。
> 8. 实现do_it函数，用于创建UDO并关联到选中的尺寸或注释对象，同时绘制4条边界线。
> 9. 在ufusr函数中调用do_it函数来执行主功能。
>
> 总体来说，这段代码实现了在NX中创建与注释对象同步更新的文本框UDO的功能。
>
