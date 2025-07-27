### 【1642】modify selected objects align position 修改选定对象的对齐位置

#### 代码

```cpp
    /*HEAD MODIFY_SELECTED_OBJECTS_ALIGN_POSITION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。该函数的作用是打印系统日志。 */  
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
    static logical prompt_for_entity_site(char *prompt, int *selection)  
    {  
        int  
            resp;  
        char  
            options[9][38] = { "TOP LEFT",  
                               "TOP CENTER",  
                               "TOP RIGHT",  
                               "MID LEFT",  
                               "MID CENTER",  
                               "MID RIGHT",  
                               "BOTTOM LEFT",  
                               "BOTTOM CENTER",  
                               "BOTTOM RIGHT" };  
        resp = *selection;  
        resp = uc1603(prompt, resp, options, 9);  
        if ((resp > 4) && (resp < 19))  
        {  
            *selection = resp - 4;  
            return TRUE;  
        }  
        else return FALSE;  
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
    static void ask_annotation_text_box_abs(tag_t obj, double lower_left_corner[3],  
        double lower_right_corner[3], double upper_left_corner[3],  
        double upper_right_corner[3])  
    {  
        tag_t  
            matrix;  
        double  
            height,  
            length;  
        UF_CALL(UF_DRF_ask_annotation_text_box(obj, upper_left_corner, &length,  
            &height));  
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
    }  
    static void display_drf_object_origin_and_text_box(tag_t a_note)  
    {  
        double  
            llc[3],  
            lrc[3],  
            origin[3],  
            ulc[3],  
            urc[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DRF_ask_origin(a_note, origin));  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            origin, &props, UF_DISP_POINT));  
        ask_annotation_text_box_abs(a_note, llc, lrc, ulc, urc);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, lrc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            lrc, urc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, ulc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            ulc, llc, &props));  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            obj;  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        while ((obj = select_a_dim_or_draft("Modify align position")) != NULL_TAG)  
        {  
            display_drf_object_origin_and_text_box(obj);  
            UF_CALL(UF_DRF_ask_object_preferences(obj, mpi, mpr, rad, dia));  
            if (prompt_for_entity_site("New align position", &mpi[30]))  
            {  
                UF_CALL(UF_DRF_set_object_preferences(obj, mpi, mpr, rad, dia));  
                UF_DISP_refresh();  
                display_drf_object_origin_and_text_box(obj);  
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

> 这段代码是一个NX二次开发应用程序，其主要功能是修改选定注释对象的对齐位置。下面是该代码的主要功能：
>
> 1. 错误处理函数：定义了一个错误处理函数report_error，用于在发生错误时打印错误信息。
> 2. 选择注释对象：定义了一个函数select_a_dim_or_draft，用于选择一个注释对象，可以是尺寸或绘图辅助元素。
> 3. 提示用户选择位置：定义了一个函数prompt_for_entity_site，用于提示用户选择注释对象的新对齐位置。
> 4. 获取注释对象信息：定义了函数ask_annotation_text_box_abs，用于获取注释对象的文本框信息。
> 5. 显示注释对象信息：定义了函数display_drf_object_origin_and_text_box，用于显示注释对象的文本框和原点。
> 6. 主函数：定义了函数do_it，用于主程序逻辑。在循环中，选择注释对象，显示其信息，提示用户选择新位置，并更新注释对象信息。
> 7. NX初始化和终止：定义了函数ufusr，用于初始化和终止NX，并调用do_it函数执行主逻辑。
> 8. 卸载函数：定义了函数ufusr_ask_unload，用于立即卸载应用程序。
>
> 综上所述，这段代码实现了选择注释对象、显示其信息、提示用户修改位置并更新注释对象的功能。
>
