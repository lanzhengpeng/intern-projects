### 【2324】report selected drf object origin 报告选定的DRF对象原点

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_DRF_OBJECT_ORIGIN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的功能，用于打印系统日志。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void map_matrix2abs(tag_t input_matrix, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_CSYS_ask_matrix_values(input_matrix, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_text;  
        tag_t  
            note,  
            matrix;  
        double  
            abs_origin[3],  
            wcs_origin[3];  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((note = select_a_dim_or_draft("Report origin")) != NULL_TAG)  
        {  
            WRITE("\nObject Origin:\n");  
            UF_CALL(UF_DRF_ask_origin(note, abs_origin));  
            WRITE("  ");  
            WRITE3F(abs_origin);  
            display_temporary_colored_point(abs_origin, UF_OBJ_GREEN);  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, abs_origin,  
                UF_CSYS_ROOT_WCS_COORDS, wcs_origin));  
            WRITE("  ");  
            WRITE3F(wcs_origin);  
            WRITE("\nText Origin:\n");  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(note, &n_text, &text_info));  
            UF_CALL(UF_CSYS_ask_matrix_of_object(note, &matrix));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                UF_VEC3_copy(text_info[ii].origin, abs_origin);  
                map_matrix2abs(matrix, abs_origin);  
                WRITE("  ");  
                WRITE3F(abs_origin);  
                display_temporary_colored_point(abs_origin, UF_OBJ_BLUE);  
                UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, abs_origin,  
                    UF_CSYS_ROOT_WCS_COORDS, wcs_origin));  
                WRITE("  ");  
                WRITE3F(wcs_origin);  
            }  
            if (n_text > 0) UF_DRF_free_text(n_text, &text_info);  
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

> 这段代码是用于NX的二次开发，其主要功能是获取选定标注或草图的原始坐标和文本坐标，并显示在图形窗口中。以下是代码的主要功能：
>
> 1. 错误报告函数：该函数用于在出错时打印错误信息，包括文件名、行号、错误码和错误描述。
> 2. 选择标注或草图：函数select_a_dim_or_draft用于提示用户选择一个标注或草图，并返回其tag。同时，会清除选择掩码并设置新的选择掩码。
> 3. 写入列表窗口：WRITE和WRITE3F宏用于向列表窗口写入文本或数组。
> 4. 数组写入列表窗口：write_double_array_to_listing_window函数用于将数组写入列表窗口。
> 5. 坐标转换：map_matrix2abs函数用于将对象坐标系下的坐标转换到绝对坐标系下。
> 6. 临时显示点：display_temporary_colored_point函数用于在图形窗口中临时显示一个点，并设置颜色。
> 7. 获取标注信息：do_it函数用于获取选定标注的原始坐标和文本坐标，并显示在图形窗口和列表窗口中。
> 8. 主函数：ufusr函数用于初始化和调用do_it函数。
> 9. 卸载函数：ufusr_ask_unload函数用于立即卸载用户函数。
>
> 总的来说，这段代码实现了选择标注或草图、获取原始坐标和文本坐标，并在图形窗口和列表窗口中显示这些信息的功能。
>
