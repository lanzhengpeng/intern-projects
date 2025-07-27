### 【0597】create arc tangent tangent radius 创建切线切线半径圆弧

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** 2181818.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 译:Include files的译文是"包含文件"。 */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_trns.h>  
    #include <uf_ui.h>  
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
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
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
    static void display_temporary_asterisk(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
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
    static tag_t select_point_on_curve(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, cp);  
            display_temporary_asterisk(cp);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
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
    /* 里海 */  
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_ugopen(void)  
    {  
        tag_t  
            curve_1 = NULL_TAG,  
            curve_2 = NULL_TAG,  
            newArc = NULL_TAG,  
            support_plane = NULL_TAG;  
        logical  
            numProvided = FALSE;  
        double  
            firstHelpPt[3] = { 0., 0., 0. },  
            secondHelpPt[3] = { 0., 0., 0. },  
            arcRadius = 0.;  
        UF_CURVE_help_data_t  
            help_data[2];  
        UF_CURVE_limit_t  
            limit[2];  
        UF_CURVE_help_data_p_t      
            help_data_p[ 2 ] = { NULL, NULL };  
        UF_CURVE_limit_t      
            *limit_p [ 2 ] = { NULL, NULL };  
        curve_1 = select_point_on_curve("First Curve: ", firstHelpPt);  
        curve_2 = select_point_on_curve("Second Curve: ", secondHelpPt);  
        if(NULL_TAG == curve_1 || NULL_TAG == curve_2) return;  
        do  
        {  
            numProvided = prompt_for_a_number("Arc Radius:", "Radius", &arcRadius);  
        } while (arcRadius <= 0.);  
        support_plane = select_a_datum_plane("Support Plane:");  
        if(NULL_TAG == support_plane) return;  
        help_data[0].help_data_type = UF_CURVE_help_data_none;  
        help_data[1].help_data_type = UF_CURVE_help_data_none;  
        help_data_p[0] = &help_data[0];  
        help_data_p[1] = &help_data[1];  
        limit[0].help_data.help_data_type = UF_CURVE_help_data_value;  
        limit[1].help_data.help_data_type = UF_CURVE_help_data_value;  
        UF_VEC3_copy( firstHelpPt, limit[0].help_data.value);  
        UF_VEC3_copy( secondHelpPt, limit[1].help_data.value);  
        limit[0].limit_type = UF_CURVE_limit_to_entity;  
        limit[1].limit_type = UF_CURVE_limit_to_entity;  
        limit[0].limiting_obj = curve_1;  
        limit[1].limiting_obj = curve_2;  
        limit_p[0] = &limit[0];  
        limit_p[1] = &limit[1];  
        UF_CALL(UF_CURVE_create_arc_tangent_tangent_radius(curve_1, curve_2,   
                arcRadius, help_data_p, limit_p, support_plane, TRUE, &newArc));  
    }  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        if( UF_CALL(UF_initialize()) )   
        {  
            return;  
        }  
        do_ugopen();  
        UF_CALL(UF_terminate());  
    }  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，实现了以下功能：
>
> 1. 包含了创建弧线的方法，包括选择两条曲线、输入弧的半径、选择一个基准平面，然后创建与这两条曲线相切的弧。
> 2. 使用UF_CURVE_create_arc_tangent_tangent_radius函数来创建弧线。
> 3. 定义了一些帮助函数，例如选择曲线上的点、选择基准平面、提示输入数值等。
> 4. 使用UF_CALL宏来捕获函数调用错误，并打印错误信息。
> 5. 使用了视图坐标系和绝对坐标系之间的转换函数。
> 6. 使用UF_DISP_display_temporary_point函数在选择的曲线上显示临时标记。
> 7. 实现了卸载函数ufusr_ask_unload。
> 8. 主函数为ufusr，实现了初始化NX、调用do_ugopen创建弧线、终止NX的功能。
>
> 总体来说，这段代码实现了弧线的创建功能，并提供了用户交互界面。
>
