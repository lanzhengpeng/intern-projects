### 【2421】report view dependent data of selected object 报告选定对象的视图相关数据

#### 代码

```cpp
    /*HEAD REPORT_VIEW_DEPENDENT_DATA_OF_SELECTED_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_layout.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的。 */  
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
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
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
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
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
    static logical is_view_active(char *view_name)  
    {  
        char  
            this_view[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (!uc6473("", this_view) && strcmp(this_view, ""))  
        {  
            if (!strcmp(this_view, view_name)) return TRUE;  
        }  
        while (!uc6499("", this_view) && strcmp(this_view, ""))  
        {  
            if (!strcmp(this_view, view_name)) return TRUE;  
        }  
        return FALSE;  
    }  
    static void report_vde_data(tag_t object)  
    {  
        int  
            ii,  
            n_edits;  
        double  
            end_point[3],  
            junk[3],  
            start_point[3];  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_VIEW_vde_data_t  
            *vde_data;  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        WRITE_D(object);  
        if (UF_CALL(UF_VIEW_ask_vde_data(object, &n_edits, &vde_data))) return;  
        WRITE_D(n_edits);  
        for (ii = 0; ii < n_edits; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(vde_data[ii].view_tag);  
            UF_CALL(UF_OBJ_ask_name(vde_data[ii].view_tag, view_name));  
            WRITE_S(view_name);  
            WRITE_F(vde_data[ii].start_parameter);  
            WRITE_F(vde_data[ii].end_parameter);  
            UF_CALL(UF_MODL_ask_curve_props(object, vde_data[ii].start_parameter,  
                start_point, junk, junk, junk, junk, junk));  
            WRITE3F(start_point);  
            if (is_view_active(view_name))  
            {  
                attrib.color = UF_OBJ_GREEN;  
                UF_CALL(UF_DISP_display_temporary_point(vde_data[ii].view_tag,  
                    UF_DISP_USE_VIEW_TAG, start_point, &attrib, UF_DISP_POINT));  
            }  
            UF_CALL(UF_MODL_ask_curve_props(object, vde_data[ii].end_parameter,  
                end_point, junk, junk, junk, junk, junk));  
            WRITE3F(end_point);  
            if (is_view_active(view_name))  
            {  
                attrib.color = UF_OBJ_RED;  
                UF_CALL(UF_DISP_display_temporary_point(vde_data[ii].view_tag,  
                    UF_DISP_USE_VIEW_TAG, end_point, &attrib, UF_DISP_POINT));  
            }  
            WRITE_D(vde_data[ii].color);  
            WRITE_D(vde_data[ii].font);  
            WRITE_D(vde_data[ii].width);  
        }  
        if (n_edits > 0) UF_free(vde_data);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_a_curve_or_edge("Report VDE data")) != NULL_TAG)  
            report_vde_data(object);  
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

> 这是段NX Open C++代码，用于报告选定曲线或边在各个视图中的VDE数据。
>
> 主要功能包括：
>
> 1. 使用对话框让用户选择一个曲线或边，并获取其tag。
> 2. 获取选定对象在各个视图中的VDE数据，包括视图tag、起点和终点参数、颜色、字体、线宽等。
> 3. 在活动视图中，根据VDE数据在起点和终点参数处显示临时点，以直观展示。
> 4. 将VDE数据写入列表窗口，以供查看。
> 5. 提供了错误报告函数，在函数调用失败时报告错误。
> 6. 提供了整数、浮点数、字符串、数组等写入列表窗口的辅助函数。
> 7. 提供了判断视图是否活动的函数。
> 8. 主函数ufusr用于初始化、调用功能函数、终止NX Open。
> 9. 提供了卸载函数，使加载项可以立即卸载。
>
> 通过这个程序，用户可以方便地获取和查看曲线或边在各个视图中的显示属性，以便进行进一步处理。
>
