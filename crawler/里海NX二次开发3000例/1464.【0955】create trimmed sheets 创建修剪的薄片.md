### 【0955】create trimmed sheets 创建修剪的薄片

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_undo.h>  
    #include <uf_evalsf.h>  
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
    /* qq3123197280 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_sheet(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static int select_curves_or_edges(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves or edges", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves_and_edges, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void display_temporary_points(int n_pts, double *coords)  
    {  
        int  
            ii;  
        char  
            no[15];  
        for (ii = 0; ii < n_pts; ii++)  
        {  
            display_temporary_point(&coords[ii*3]);  
            sprintf(no, "gap %d", ii+1);  
            display_temporary_text(no, &coords[ii*3]);  
        }  
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
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:返回1或2 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            flag = 1,  
            n_gaps,  
            n;  
        tag_t  
            *curves,  
            sheet,  
            trim;  
        double  
            distol,  
            dist,  
            *gap_pts,  
            pos[3],  
            pos_on_sheet[3];  
        UF_MODL_vector_t  
            vector = { 0, UF_MODL_VECTOR_FACE_NORMALS, 0.0 };  
        UF_UNDO_mark_id_t  
            mark;  
        UF_MODL_ask_distance_tolerance(&distol);  
        while (((sheet = select_a_sheet("Select sheet to trim")) != NULL_TAG) &&  
               ((n = select_curves_or_edges("Select bounding curves", &curves)) != 0) &&  
               specify_point("Indicate Region Point", pos) &&  
               pick_one_of_two_choices("Point indicates?", "Keep", "Discard", &flag) &&  
               prompt_for_a_number("Tolerance", "Distance Tolerance", &distol))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Trim Sheet", &mark));  
            UF_CALL(UF_MODL_ask_minimum_dist(NULL_TAG, sheet, TRUE, pos, FALSE, NULL, &dist,  
                pos, pos_on_sheet));  
            if (UF_CALL(UF_MODL_create_trimmed_sheet(sheet, n, curves, &vector, (flag - 1),  
                1, pos_on_sheet, distol, &n_gaps, &gap_pts, &trim)))  
            {  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
            }  
            if (n_gaps)  
            {  
                display_temporary_points(n_gaps, gap_pts);  
                UF_free(gap_pts);  
            }  
            UF_free(curves);  
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

> 这段代码是NX Open C++的二次开发示例，实现了选择钣金面、选择边界曲线、指定点、选择保留或丢弃区域以及输入公差后创建修剪后的钣金面的功能。
>
> 主要功能包括：
>
> 1. 选择钣金面：使用UF_MODL_ask_body_type函数判断实体类型，仅接受钣金面类型。
> 2. 选择边界曲线：接受线、圆、锥曲线、样条线类型的曲线和实体的边。
> 3. 指定点：提示用户指定一个点，用于确定保留或丢弃的区域。
> 4. 选择保留或丢弃：提供两个选项，让用户选择指定点代表保留区域还是丢弃区域。
> 5. 输入公差：提示用户输入距离公差。
> 6. 创建修剪后的钣金面：使用UF_MODL_create_trimmed_sheet函数，根据边界曲线、指定点、保留/丢弃标志和公差创建新的修剪后的钣金面。
> 7. 显示间隙点：如果存在间隙点，则显示它们。
> 8. 使用UNDO：在创建修剪后的钣金面前设置UNDO标记，如果失败则撤销到标记。
> 9. 资源释放：释放临时分配的内存。
> 10. 初始化和终止：在代码开始和结束时分别调用UF_initialize和UF_terminate。
>
> 总的来说，这是一个典型的NX Open C++二次开发示例，实现了钣金面修剪的功能，具有较好的用户体验和错误处理。
>
