### 【2514】rpo sketch RPO草图

#### 代码

```cpp
    /*HEAD RPO_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
请注意，UF_print_syslog是V18版本中新增的。 */  
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
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static tag_t select_point_on_datum(char *prompt, double cp[3])  
    {  
        int  
            irc;  
        tag_t  
            face,  
            line,  
            view;  
        double  
            dist;  
        UF_CURVE_line_t  
            lp;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_datum_plane_type;  
        mask.object_subtype = 0;  
        mask.solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &face, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
            map_abs2view(cp);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_CALL(UF_MODL_ask_minimum_dist(face,line,0,cp,0,cp,&dist,cp,cp));  
            UF_CALL(UF_OBJ_delete_object(line));  
            return face;  
        }  
        else return NULL_TAG;  
    }  
    static int Place_rpo(tag_t feat)  
    {  
        char  
            *const_value[2] = { "0.0", "0.0"},  
            *const_array[2] = { "PERP_DIST_PARMS", "PERP_DIST_PARMS"};  
        double  
            loc[3];  
        int  
            irc,  
            targ_qual[2] = { UF_MODL_ENDPOINT_1,UF_MODL_ENDPOINT_1 };  
        tag_t  
            target[2],  
            tool[2];  
        target[0] = select_point_on_datum("select horizontal target edge", loc);  
        tool[0] = select_a_line("select horizontal tool curve");  
        target[1] = select_point_on_datum("select vertical target edge", loc);  
        tool[1] = select_a_line("select vertical tool curve");  
        irc = UF_CALL(UF_MODL_create_rpo_constraints(feat, NULL_TAG,  
                     NULL_TAG, target, targ_qual, tool, targ_qual,  
                     const_value, const_array, 2));  
        return (irc);  
    }  
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            sket_tag;  
        while ((sket_tag = select_a_sketch("Select sketch")) != NULL_TAG)  
            Place_rpo(sket_tag);  
    }  
    /* qq3123197280 */  
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

> 这是段NX Open C++二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误处理函数report_error，用于打印错误信息。
> 2. 定义了辅助函数mask_for_lines，用于设置选择线的类型。
> 3. 定义了选择线的函数select_a_line，实现了选择线并高亮显示的功能。
> 4. 定义了视图坐标与绝对坐标转换的函数map_abs2view和map_view2abs。
> 5. 定义了选择基准面的函数select_point_on_datum。
> 6. 定义了创建RPO约束的函数Place_rpo，实现了选择两个目标线和工具线，并创建两个RPO约束的功能。
> 7. 定义了选择草图的函数select_a_sketch。
> 8. 定义了主函数do_it，循环选择草图并创建RPO约束。
> 9. 定义了ufusr函数，实现了初始化、执行主函数、终止的功能。
> 10. 定义了卸载函数ufusr_ask_unload。
>
> 综上所述，这段代码实现了在NX中选择草图，并为草图添加两个RPO约束的功能，主要用于NX二次开发中。
>
