### 【2351】report selected note polyline data 报告选定注释多段线数据

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_NOTE_POLYLINE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog是V18版本新增的函数。因此，我的回答是：

UF_print_syslog是V18版本新增的函数。 */  
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
    int mask_for_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_note_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    tag_t select_a_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a note", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_notes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE(X)   (UF_UI_write_listing_window(X))  
    #define WRITE_C(X) (write_character_to_listing_window(#X, X))  
    void write_character_to_listing_window(char *title, char character)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %c\n", title, character);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, const char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    void write_double_array_to_listing_window(char *name, const double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t begin_line(void *client)  
    {  
        WRITE("Begin line\n");  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t end_line(void *client)  
    {  
        WRITE("End line\n");  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t set_to_position(double in_point[3],  
        void *client, double out_point[3], logical *out_status)  
    {  
        WRITE("Set to position\n");  
        WRITE3F(in_point);  
        *out_status = TRUE;  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t draw_to_position(double in_point[3],  
        double last_out_point[3], logical last_status, void *client,  
        double out_point[3], logical *out_status)  
    {  
        WRITE("Draw to position\n");  
        WRITE3F(in_point);  
        WRITE_D(last_status);  
        *out_status = TRUE;  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t draw_arc(double center[3], double radius,  
        double start_param, double end_param, void *client)  
    {  
        WRITE("Draw arc\n");  
        WRITE3F(center);  
        WRITE_F(radius);  
        WRITE_F(start_param * RADEG);  
        WRITE_F(end_param * RADEG);  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t draw_char(double origin[3],  
        unsigned char character, void *client)  
    {  
        WRITE("Draw char\n");  
        WRITE3F(origin);  
        WRITE_C(character);  
        return UF_DRF_RENDER_OK;  
    }  
    UF_DRF_render_text_status_t draw_standard_font_string(const char* textString,   
    const double origin[3] , int fontId, double textAngle, double fontSize,   
    double gapSize, double aspectRatio, logical suppressKerning, logical vertical,   
    logical bold, logical italic, void * client )  
    {  
        WRITE("Draw standard font string\n");  
        WRITE_S(textString);  
        WRITE3F(origin);  
        WRITE_D(fontId);  
        WRITE_F(textAngle);  
        WRITE_F(fontSize);  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t draw_user_symbol(char *symbol_name,  
        double *origin, void *symbol_params, void *client)  
    {  
        WRITE("Draw user symbol\n");  
        WRITE_S(symbol_name);  
        WRITE3F(origin);  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t set_cfw(UF_DRF_cfw_p_t cfw, void *client)  
    {  
        WRITE("Set cfw\n");  
        WRITE_D(cfw->color);  
        WRITE_D(cfw->font);  
        WRITE_D(cfw->width);  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t push_orientation(double matrix[9], void *client)  
    {  
        WRITE("Push orientation\n");  
        WRITE9F(matrix);  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t pop_orientation(void *client)  
    {  
        WRITE("Pop orientation\n");  
        return UF_DRF_RENDER_OK;  
    }  
    /* qq3123197280 */  
    UF_DRF_render_text_status_t fill_region(int num_points, double *points,  
        double fill_dist, double last_pt[3], logical *was_drawn, void *client)  
    {  
        WRITE("Fill region\n");  
        WRITE_D(num_points);  
        WRITE_F(fill_dist);  
        *was_drawn = FALSE;  
        return UF_DRF_RENDER_OK;  
    }  
    void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n_text,  
            resp;  
        tag_t  
            note,  
            part = UF_PART_ask_display_part();  
        char  
            **text;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        UF_DRF_render_table_t  
            render_table ={ begin_line,  
                            end_line,  
                            set_to_position,  
                            draw_to_position,  
                            draw_arc,  
                            draw_char,  
                            draw_standard_font_string, // Added this in NX8  
                            draw_user_symbol,  
                            set_cfw,  
                            push_orientation,  
                            pop_orientation,  
                            fill_region };  
        UF_CALL(UF_UI_open_listing_window());  
        while ((note = select_a_note("Render text")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(note, &n_text, &text_info));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                text = (char**) UF_allocate_memory(text_info[ii].num_lines * sizeof(char *),  
                    &resp);  
                for (jj = 0; jj < text_info[ii].num_lines; jj++)  
                {  
                    text[jj] = (char*) UF_allocate_memory(  
                        (text_info[ii].text[jj].full_num_chars + 1) * sizeof(char),  
                        &resp);  
                    strcpy(text[jj], text_info[ii].text[jj].full_string);  
                }  
                UF_CALL(UF_DRF_render_text(part, note, text_info[ii].num_lines,  
                    text, &text_info[ii], &render_table, NULL));  
                UF_free_string_array(text_info[ii].num_lines, text);  
            }  
            UF_CALL(UF_DRF_free_text(n_text, &text_info));  
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

> 这段代码是用于NX的二次开发，其主要功能是渲染选定注释中的文本。
>
> 关键要点如下：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了函数mask_for_notes，用于设置选择掩码，仅选择注释类型。
> 3. 定义了函数select_a_note，用于选择一个注释对象。
> 4. 定义了各种写入函数，用于在列表窗口中输出信息。
> 5. 定义了渲染文本的各种回调函数，包括开始/结束行、设置位置、绘制位置、绘制弧线、绘制字符等，用于在渲染文本时调用。
> 6. 定义了主函数do_it，用于循环选择注释，获取文本信息，并调用UF_DRF_render_text函数进行渲染。
> 7. 定义了ufusr函数，作为NX二次开发的入口函数。
> 8. 定义了ufusr_ask_unload函数，用于卸载二次开发。
>
> 总体来说，该代码通过回调函数的方式实现了对NX注释文本的渲染，用户可以根据需要修改回调函数以实现自定义的渲染效果。
>
