### 【0933】create sweep 创建扫掠

#### 代码

```cpp
    /*HEAD CREATE_SWEEP CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_eval.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的功能。 */  
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
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
        UF_CALL(UF_DISP_set_display(TRUE));  
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
    static tag_t select_a_curve_end(char *prompt, int *which_end)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            pos[3];  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            UF_CALL(UF_MODL_get_curve_edge_direction(pos, object, which_end));  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_strings(char *which, UF_STRING_t *string)  
    {  
        int  
            dirs[50],  
            ii = 0,  
            jj = 0,  
            kk = -1,  
            n,  
            n_segs[10];  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        UF_MODL_init_string_list(string);  
        do  
        {  
            if ((kk > -1) && (curves[kk] == NULL_TAG))  
            {  
                n_segs[ii] = jj-1;  
                ii++;  
                jj = 0;  
                kk--;  
            }  
            if (jj > 0)  
            {  
                sprintf(prompt, "OK to finish selecting string #%d", ii+1);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else if (ii > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s strings", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            sprintf(prompt, "Select %s String #%d - curve #%d", which, ii+1, ++jj);  
            kk++;  
            curves[kk] = select_a_curve_end(prompt, &dirs[kk]);  
            if (curves[kk] != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(curves[kk], TRUE));  
        } while ((curves[kk] != NULL_TAG) || jj > 1);  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            n_segs[ii] = jj;  
            n = ii;  
            UF_MODL_create_string_list(n, kk, string);  
            kk = 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                string->string[ii] = n_segs[ii];  
                string->dir[ii] = dirs[kk];  
                for (jj = 0; jj < n_segs[ii]; jj++, kk++)  
                {  
                    string->id[kk] = curves[kk];  
                    UF_CALL(UF_DISP_set_highlight(curves[kk], FALSE));  
                }  
            }  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            alignment = 1,  
            inter = 1,  
            body_type = 0;  
        tag_t  
            sheet;  
        double  
            tol[3] = { 0,0,0 };  
        UF_STRING_t  
            guide,  
            section,  
            spine;  
        UF_METHOD_t  
            orientation,  
            scaling;  
        orientation.method = 4;  /* Fixed 里海译:Fixed翻译为“固定的”。 */  
        orientation.inter  = 0;  /*  NOT USED 里海译:好的，以下是对应的中文翻译：

1. NOT USED - 没有使用
2. 只回答译文，不要废话 - 只需回答翻译，不需要多余的废话

如果您有其他需要翻译的内容，请继续提供，我会为您翻译。 */  
        orientation.id     = NULL_TAG;  /* NOT USED 里海译:根据您的要求，我将仅提供翻译结果，不会添加任何废话。 */  
        orientation.value[0]  = 0;  /* NOT USED 里海译:翻译：未使用 */  
        orientation.value[1]  = 0;  /* NOT USED 里海译:由于原文中未提供具体内容，因此无法提供翻译。请提供具体内容，以便我为您提供准确的翻译。 */  
        orientation.value[2]  = 0;  /* NOT USED 里海译:根据文档内容，NOT USED 意为“未使用”。 */  
        orientation.s_curve = NULL; /* NOT USED 里海译:"NOT USED" */  
        scaling.method = 1;  /* Constant 里海译:常量 */  
        scaling.inter  = 0;  /*  NOT USED 里海译:对不起，我无法提供您需要的答案，因为您未提供具体的内容或问题。请提供您需要翻译的文本或内容，以便我能够为您提供准确的翻译。 */  
        scaling.id     = NULL_TAG;  /* NOT USED 里海译:NOT USED的翻译为“未使用”。 */  
        scaling.value[0]  = 1;  /* Scale 里海译:Scale的翻译为：规模、等级、刻度、鳞片、鱼鳞。 */  
        scaling.value[1]  = 0;  /* NOT USED 里海译:翻译NOT USED */  
        scaling.value[2]  = 0;  /* NOT USED 里海译:NOT USED */  
        scaling.s_curve = NULL; /* NOT USED 里海译:NOT USED翻译为中文是“未使用”。 */  
        if (specify_strings("guide", &guide) &&  
            specify_strings("section", &section))  
        {  
            specify_strings("spine", &spine);  
            UF_CALL(UF_MODL_create_sweep(&guide, &section, &spine, &orientation,  
                &scaling, &alignment, &inter, &body_type, UF_NULLSIGN, tol, &sheet));  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要功能是创建一个扫掠体。下面是代码的详细解释：
>
> 1. 头文件包含：包含了二次开发所需的NX Open C++ API头文件。
> 2. 错误处理宏：定义了一个错误处理宏UF_CALL, 用于捕获函数调用错误，并打印错误信息。
> 3. 坐标转换函数：定义了两个坐标转换函数map_abs2view和map_view2abs，用于将绝对坐标系坐标转换到视图坐标系，反之亦然。
> 4. 获取对象上点的位置：定义了一个函数ask_pos_on_obj，用于获取在指定对象上的点的位置。
> 5. 选择曲线端点的函数：定义了一个函数select_a_curve_end，用于选择曲线的一个端点，并返回该曲线的tag。
> 6. 指定引导线、截面线和脊柱线的函数：定义了一个函数specify_strings，用于指定引导线、截面线和脊柱线。
> 7. 创建扫掠体的函数：定义了一个函数do_it，用于创建扫掠体。
> 8. 主函数：定义了主函数ufusr，用于初始化和结束NX，并调用do_it函数。
> 9. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于立即卸载用户函数。
>
> 总体来说，这段代码实现了扫掠体的创建，通过选择引导线、截面线和脊柱线来生成扫掠体。代码结构清晰，注释详细，是一个很好的二次开发示例。
>
