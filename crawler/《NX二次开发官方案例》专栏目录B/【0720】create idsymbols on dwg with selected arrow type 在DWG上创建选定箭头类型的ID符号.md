### 【0720】create idsymbols on dwg with selected arrow type 在DWG上创建选定箭头类型的ID符号

#### 代码

```cpp
    /*HEAD CREATE_ID_SYMBOLS_ON_DWG_WITH_SELECTED_ARROW_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
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
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_curve_pos(char *prompt, UF_DRF_object_p_t curve,  
        double cp[3])  
    {  
        int  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_curves, NULL, &resp,  
            &curve->object_tag, cp, &curve->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(curve->object_tag, cp);  
            UF_CALL(UF_DISP_set_highlight(curve->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_for_arrow_type(char *prompt, int *selection)  
    {  
        int  
            resp = 0;  
        char  
            options[8][38] = { "UF_DRF_FILLED_DOT",  
                               "UF_DRF_FILLED_ARROW",  
                               "UF_DRF_CLOSED_ARROW",  
                               "UF_DRF_OPEN_ARROW",  
                               "UF_DRF_CROSS_ARROW",  
                               "UF_DRF_DOT_ARROW",  
                               "UF_DRF_ORIGIN_SYMBOL_ARROW",  
                               "UF_DRF_NO_ARROW" };  
        resp = *selection;  
        resp = uc1603(prompt, resp, options, 8);  
        if ((resp > 4) && (resp < 19))  
        {  
            // subtract 6 not 4 because UF_DRF_FILLED_DOT begins with -1  
            *selection = resp - 6;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        char  
            id_text[MAX_ID_SYM_TEXT_LENGTH + 1];  
        double  
            cp[3],  
            pos[3];  
        int  
            arrowtype,  
            i = 0;  
        tag_t  
            id;  
        UF_DRF_object_t  
            curve;  
        UF_DRF_line_arrow_preferences_t   
            line_arrow_preferences;  
        UF_CALL(UF_DRF_init_object_structure(&curve));  
        while (select_drf_curve_pos("Select item to label", &curve, cp))  
        {  
            if (select_pos("Indicate origin for label", pos))  
            {  
                sprintf(id_text, "ID %d", ++i);  
                prompt_for_arrow_type( "Select an arrow type", &arrowtype);  
                UF_CALL( UF_DRF_ask_line_arrow_preferences ( &line_arrow_preferences ) );  
                line_arrow_preferences.first_arrow_type = arrowtype;  
                UF_CALL( UF_DRF_set_line_arrow_preferences ( &line_arrow_preferences ) );  
                UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, id_text, "",  
                    pos, UF_DRF_with_leader, UF_DRF_leader_attach_object, &curve,  
                    cp, &id));  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是创建带有箭头的标识符号。
>
> 主要步骤如下：
>
> 1. 包含必要的头文件，定义宏和全局变量。
> 2. 定义错误报告函数 report_error，用于在出错时打印错误信息。
> 3. 定义两个坐标转换函数 map_abs2view 和 map_view2abs，用于将绝对坐标转换为视图坐标，反之亦然。
> 4. 定义 ask_pos_on_obj 函数，用于确定对象上的一个点。
> 5. 定义 mask_for_drf_curves 函数，用于设置选择掩码，限制只能选择线、圆、圆锥、样条线等对象。
> 6. 定义 select_drf_curve_pos 函数，用于让用户选择一个对象，并获取其上的一个点。
> 7. 定义 select_pos 函数，用于让用户指定一个位置点。
> 8. 定义 prompt_for_arrow_type 函数，用于提示用户选择箭头类型。
> 9. 定义 do_it 函数，主逻辑函数，用于选择对象，获取位置点，选择箭头类型，并创建标识符号。
> 10. 定义 ufusr 函数，是NX调用的主入口函数。
> 11. 定义 ufusr_ask_unload 函数，用于卸载时立即返回。
>
> 总体来说，这段代码实现了让用户选择对象和位置点，然后在该位置创建带有指定箭头的标识符号的功能。
>
