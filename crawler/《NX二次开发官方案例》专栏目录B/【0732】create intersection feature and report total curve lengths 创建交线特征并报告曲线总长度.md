### 【0732】create intersection feature and report total curve lengths 创建交线特征并报告曲线总长度

#### 代码

```cpp
    /*HEAD CREATE_INTERSECTION_FEATURE_AND_REPORT_TOTAL_CURVE_LENGTHS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的功能，用于打印系统日志。 */  
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
    static int mask_for_body_face_dplane(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_body_face_dplanes(char *prompt, tag_t **items)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select face/plane/datum", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_body_face_dplane, NULL, &resp, &cnt, items));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*items)[ii], FALSE));  
        return cnt;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void flag_object(tag_t object, char *msg)  
    {  
        double  
            loc[3];  
        if (UF_OBJ_ask_name_origin(object, loc))  
        {  
            UF_CALL(UF_OBJ_set_name(object, "X"));  
            UF_CALL(UF_OBJ_ask_name_origin(object, loc));  
            UF_CALL(UF_OBJ_delete_name(object));  
        }  
        display_temporary_text(msg, loc);  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static void report_curve_array_total_length(int n_curves, tag_t *curves)  
    {  
        int  
            ii;  
        double  
            length,  
            total_length = 0;  
        char  
            msg[133];  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_arc_length(curves[ii], 0.0, 1.0,  
                UF_MODL_UNITS_PART, &length));  
            sprintf(msg, "%f", length);  
            flag_object(curves[ii], msg);  
            total_length = total_length + length;  
        }  
        set_highlight_object_array(n_curves, curves, TRUE);  
        sprintf(msg, "Total length = %f", total_length);  
        uc1601(msg, TRUE);  
        set_highlight_object_array(n_curves, curves, FALSE);  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            n1,  
            n2;  
        tag_t  
            *curves,  
            *set1,  
            *set2,  
            int_feat;  
        UF_UNDO_mark_id_t  
            mark_id;  
        while (((n1 = select_body_face_dplanes("Intersect - set 1", &set1)) > 0) &&  
               ((n2 = select_body_face_dplanes("Intersect - set 2", &set2)) > 0))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Intersect Curve", &mark_id));  
            if (!UF_CALL(UF_CURVE_create_int_object(n1,set1, n2,set2, &int_feat)))  
            {  
                UF_CALL(UF_CURVE_ask_feature_curves(int_feat, &n, &curves));  
                report_curve_array_total_length(n, curves);  
                if (n > 0) UF_free(curves);  
            }  
            else  
            {  
                UF_CALL(UF_UNDO_undo_to_mark( mark_id, NULL));  
                UF_CALL(UF_UNDO_delete_mark( mark_id, NULL));  
            }  
            UF_free(set1);  
            UF_free(set2);  
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

> 这是NX的二次开发代码，其主要功能如下：
>
> 1. 创建了用户界面，用于选择两组实体(面、平面或基准面)进行求交运算。
> 2. 对每组实体进行求交运算，并生成曲线特征。
> 3. 计算生成的每条曲线的长度，并在曲线起点处显示长度信息。
> 4. 计算所有曲线的总长度，并在界面中显示。
> 5. 在求交运算失败时，可以撤销操作。
> 6. 使用了错误报告函数，用于在出错时打印错误信息。
> 7. 使用了UNDO标记，以便在出错时撤销操作。
> 8. 在实体上显示文本信息时，使用了函数display_temporary_text。
> 9. 在选择实体时，使用了自定义选择掩码函数mask_for_body_face_dplane。
> 10. 使用了UF_UNDO、UF_CURVE、UF_DISP等NX提供的API函数。
>
> 该代码实现了选择实体、求交运算、计算长度、显示信息等完整的功能，并且包含了错误处理和撤销机制。
>
