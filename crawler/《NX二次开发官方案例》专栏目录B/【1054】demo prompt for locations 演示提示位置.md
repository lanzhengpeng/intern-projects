### 【1054】demo prompt for locations 演示提示位置

#### 代码

```cpp
    /*HEAD CONSTRUCT_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_vec.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
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
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void report_modes(int modes[2])  
    {  
        switch (modes[0])  
        {  
            case 0:  
                ECHO("modes[0] = 0 Show Menu For User Selection (Inferred)\n");  
                break;  
            case 1:  
            case 16:  
                ECHO("modes[0] = %d Cursor Location\n", modes[0]);  
                break;  
            case 4:  
                ECHO("modes[0] = 4 Existing Point\n");  
                break;  
            case 5:  
                ECHO("modes[0] = 5 End Point\n");  
                break;  
            case 6:  
                ECHO("modes[0] = 6 Control Point\n");  
                break;  
            case 7:  
            case 11:  
                ECHO("modes[0] = %d Intersection Point\n", modes[0]);  
                break;  
            case 8:  
                ECHO("modes[0] = 8 Arc/Ellipse/Sphere Center\n");  
                break;  
            case 9:  
                ECHO("modes[0] = 9 Pos On Arc/Ellipse\n");  
                break;  
            case 12:  
                ECHO("modes[0] = 12 Quadrant Point\n");  
                break;  
            case 13:  
                ECHO("modes[0] = 13 Point on Curve/Edge\n");  
                break;  
            case 14:  
                ECHO("modes[0] = 14 Point on Surface\n");  
                break;  
            default:  
                ECHO("modes[0] = %d This value is ignored\n", modes[0]);  
                break;  
        }  
        switch (modes[1])  
        {  
            case 0:  
                ECHO("modes[1] = 0 No Offset\n");  
                break;  
            case 1:  
                ECHO("modes[1] = 1 Rect Abs\n");  
                break;  
            case 3:  
                ECHO("modes[1] = 3 Cylindrical\n");  
                break;  
            case 4:  
                ECHO("modes[1] = 4 Spherical\n");  
                break;  
            case 5:  
            case 6:  
                ECHO("modes[1] = %d 3D Vector\n", modes[1]);  
                break;  
            default:  
                ECHO("modes[1] = %d This value is ignored\n", modes[1]);  
                break;  
        }  
    }  
    static void convert_drawing_to_part_units(tag_t member_view, double *loc)  
    {  
        int  
            part_units;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            sheet;  
        UF_DRAW_info_t  
            info;  
        UF_CALL(UF_PART_ask_units(part, &part_units));  
        UF_CALL(UF_DRAW_ask_drawing_of_view(member_view, &sheet));  
        UF_CALL(UF_DRAW_ask_drawing_info(sheet, &info));  
        if (part_units == info.units) return;  
        if (part_units == UF_PART_ENGLISH)  
            UF_VEC3_scale(1/25.4, loc, loc);  
        else  
            UF_VEC3_scale(25.4, loc, loc);  
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
            *choice = resp - 4;  /* returns 1 or 2 里海译:根据上下文，returns 1 or 2 应翻译为“返回1或2”。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static int ask_major_ug_version_number(void)  
    {  
        int  
            rel_no;  
        char  
            *major;  
        UF_CALL(UF_get_release(&major));  
        sscanf(major, "NX V%d", &rel_no);  
        if (rel_no == 0) sscanf(major, "V%d", &rel_no);  
        UF_free(major);  
        return rel_no;  
    }  
    static void do_it(void)  
    {  
        int  
            modes[2] = { 0,0 },  
            resp = 0,  
            nx_version = ask_major_ug_version_number(),  
            which,  
            work_only;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_VIEW_type_t  
            type;  
        tag_t  
            sel_view,  
            work_view;  
        double  
            down[3] = { 0,-1,0 },  
            dwg_loc[3] = { 0,0,0 },  
            loc[3] = { 0,0,0 },  
            up[3] = { 0, 1, 0 };  
        char  
            selected_in[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_UI_ask_cursor_view(&work_only));  
        which = work_only + 1;  
        while (pick_one_of_two_choices("Cursor View", "Any View", "Work View Only",  
            &which))  
        {  
            UF_CALL(UF_UI_set_cursor_view(which - 1));  
            while ((resp = uc1616("Specify Point", modes, 0, loc)) == 5)  
            {  
                UF_DISP_refresh();  
                UF_CALL(UF_UI_ask_cursor_view(&work_only));  
                WRITE_L(work_only);  
                WRITE_D(resp);  
                report_modes(modes);  
                WRITE3F(loc);  
                UF_DISP_labeled_conehead(UF_DISP_WORK_VIEW_ONLY, loc,  
                    up, 1, "RETURNED");  
                UF_CALL(uc1653(selected_in));  
                WRITE_S(selected_in);  
                UF_CALL(UF_VIEW_ask_tag_of_view_name(selected_in, &sel_view));  
                UF_CALL(UF_VIEW_ask_type(sel_view, &type, &subtype));  
                UF_CALL(UF_VIEW_ask_work_view(&work_view));  
            // See PRs 2143977 and 2120553!  
                if (((nx_version > 5) || (modes[0] == 9)) &&  
                    (sel_view != work_view) && (type == UF_VIEW_DRAWING_MEMBER_TYPE))  
                {  
                    ECHO("Selected in a member view without <RMB>Expand first\n");  
                    UF_CALL(UF_VIEW_map_model_to_drawing(sel_view, loc, dwg_loc));  
                    WRITE3F(dwg_loc);  
                    convert_drawing_to_part_units(sel_view, dwg_loc);  
                    UF_DISP_labeled_conehead(UF_DISP_WORK_VIEW_ONLY, dwg_loc,  
                            down, 1, "SELECTED HERE");  
                }  
                else  
                    UF_DISP_labeled_conehead(UF_DISP_WORK_VIEW_ONLY, loc,  
                        down, 1, "SELECTED HERE");  
                ECHO("\n");  
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

> 这是一段用于在NX中拾取点的二次开发代码。代码的主要功能是允许用户在任意视图或工作视图中拾取点，并显示拾取到的点的信息。具体来说：
>
> 1. 通过UF_UI_ask_cursor_view函数获取当前光标视图，并允许用户选择在任意视图还是仅工作视图中拾取点。
> 2. 使用uc1616函数让用户指定点，并循环直到用户拾取到点为止。在每次循环中，会刷新屏幕并显示提示信息。
> 3. 当用户拾取到点时，获取点的坐标、模式和选中的视图名称等信息，并显示这些信息。
> 4. 如果在部件视图中拾取点，且NX版本大于5，则会映射点到绘图坐标，并转换为部件单位。然后显示在部件视图中的拾取点位置。
> 5. 在工作视图中，使用UF_DISP_labeled_conehead函数绘制一个圆锥头标注拾取到的点。
> 6. 最后，使用UF_terminate函数结束NX的初始化状态。
>
> 总体来说，这段代码实现了在NX中拾取点并显示相关信息的功能，提供了友好的交互提示，考虑到了不同视图和NX版本的情况，并进行了适当的单位转换和显示。
>
