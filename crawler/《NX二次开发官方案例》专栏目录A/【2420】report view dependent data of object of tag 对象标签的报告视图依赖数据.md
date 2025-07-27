### 【2420】report view dependent data of object of tag 对象标签的报告视图依赖数据

#### 代码

```cpp
    /*HEAD REPORT_VIEW_DEPENDENT_DATA_OF_OBJECT_OF_TAG CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，可以总结如下要点：

1. UF_print_syslog是V18版本新增的函数。

2. 它用于打印系统日志信息。

3. 可以通过调用UF_print_syslog函数来打印系统日志信息。

4. UF_print_syslog函数接受一个格式字符串作为参数，并可以接收多个参数来指定要打印的内容。

5. 该函数的作用是将指定的系统日志信息输出到系统日志文件中。

6. 它通常用于调试目的，以便了解系统运行时的详细信息。

7. UF_print_syslog函数只接受一个格式字符串参数，而不会返回任何值。

8. 该函数需要在UF_initialize函数之后调用。

9. UF_print_syslog函数是V18版本新增的，用于在UF系统中打印系统日志信息。

10. 只需回答翻译，无需添加任何额外内容。 */  
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
    static int prompt_for_an_integer(char *prompt, char *item, int number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            return da[0];  
        }  
        else return 0;  
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
            n = NULL_TAG;  
        while ((n = (tag_t)prompt_for_an_integer("Tag to check for VDE data", "Tag", (int) n)) != NULL_TAG)  
            report_vde_data(n);  
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

> 这段代码是一个NX二次开发代码，其主要功能是获取并显示NX对象在各个视图中的VDE数据。
>
> 以下是代码的主要功能和流程：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX API失败时打印错误信息。
> 2. 提示输入整数：定义了一个提示输入整数的函数prompt_for_an_integer，用于获取用户输入的对象Tag。
> 3. 曲线和边界的掩码：定义了一个曲线和边界的掩码mask_for_curves_and_edges，用于在选择对话框中仅显示曲线和边界。
> 4. 选择曲线或边界：定义了一个选择曲线或边界的函数select_a_curve_or_edge，使用户可以选取一个对象。
> 5. 写入不同类型数据到列表窗口：定义了一系列写入不同类型数据到列表窗口的宏，用于显示各种类型的数据。
> 6. 检查视图是否激活：定义了一个检查视图是否激活的函数is_view_active。
> 7. 报告VDE数据：定义了一个报告VDE数据的函数report_vde_data，用于获取对象在各个视图中的VDE数据并显示。
> 8. 主体函数：定义了一个主体函数do_it，循环获取用户输入的对象Tag，并调用report_vde_data函数。
> 9. 主函数：定义了一个主函数ufusr，用于初始化和终止NX，并调用主体函数。
> 10. 卸载函数：定义了一个卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码的功能是获取用户选择的NX对象，并获取其在各个视图中的VDE数据，包括起点、终点、颜色、字体等信息，并在列表窗口中显示这些信息，同时在激活的视图中显示起点和终点的临时点。
>
