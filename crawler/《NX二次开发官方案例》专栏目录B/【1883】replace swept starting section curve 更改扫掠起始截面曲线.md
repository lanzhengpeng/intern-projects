### 【1883】replace swept starting section curve 更改扫掠起始截面曲线

#### 代码

```cpp
    /*HEAD REPLACE_SWEPT_STARTING_SECTION_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
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
    static int ask_curve_close_end(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            end[3],  
            junk[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk,  
            junk, junk));  
        UF_VEC3_distance(pos, start, &dist[0]);  
        UF_VEC3_distance(pos, end, &dist[1]);  
        if (dist[0] < dist[1]) return UF_MODL_CURVE_START_FROM_BEGIN;  
        else return UF_MODL_CURVE_START_FROM_END;  
    }  
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            *which_end = ask_curve_close_end(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_string(char *which, UF_STRING_t *string)  
    {  
        int  
            dirs[50],  
            jj = 0,  
            kk = -1;  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        do  
        {  
            if (kk > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s string", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            kk++;  
            sprintf(prompt, "Select %s String - curve #%d", which, kk+1);  
            curves[kk] = select_a_curve_end(prompt, &dirs[kk]);  
            if (curves[kk] != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(curves[kk], TRUE));  
        } while ((curves[kk] != NULL_TAG) && (kk < 50));  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            UF_MODL_init_string_list(string);  
            UF_MODL_create_string_list(1, kk, string);  
            string->string[0] = kk;  
            string->dir[0] = dirs[0];  
            for (jj = 0; jj < kk; jj++)  
            {  
                string->id[jj] = curves[jj];  
                UF_CALL(UF_DISP_set_highlight(curves[jj], FALSE));  
            }  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static void set_highlight_string_list(UF_STRING_t *string, int flag)  
    {  
        int ii, jj, kk = 0;  
        for (ii = 0; ii < string->num; ii++)  
            for (jj = 0; jj < string->string[ii]; jj++, kk++)  
                UF_CALL(UF_DISP_set_highlight(string->id[kk], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            alignment,  
            body_type,  
            inter;  
        tag_t  
            skin;  
        double  
            tol[3];  
        UF_STRING_t  
            guide,  
            new_section,  
            section,  
            spine;  
        UF_METHOD_t  
            orientation,  
            scaling;  
        while ((skin = select_a_feature("Select SWEPT feature")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_MODL_ask_sweep(skin, &guide, &section, &spine,  
                &orientation, &scaling, &alignment, &inter, &body_type, tol)))  
                    continue;  
            set_highlight_string_list(&section, TRUE);  
            if (specify_string("New Starting Section", &new_section))  
            {  
                if (!UF_CALL(UF_MODL_replace_feat_strings(skin,  
                    UF_MODL_STRING_REPLACE, &new_section, 2, 1)))  
                        UF_CALL(UF_MODL_update());  
                UF_MODL_free_string_list(&new_section);  
            }  
            set_highlight_string_list(&section, FALSE);  
            UF_MODL_free_string_list(&guide);  
            UF_MODL_free_string_list(&section);  
            UF_MODL_free_string_list(&spine);  
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

> 这段代码是用于替换NX中Swept特征的起始截面曲线的二次开发代码。其主要功能和步骤如下：
>
> 1. 初始化和终止UF库。
> 2. 提示用户选择一个Swept特征，并获取其相关信息，如引导线、截面曲线、脊线等。
> 3. 高亮显示Swept特征的截面曲线。
> 4. 提示用户选择新的起始截面曲线，并获取其信息。
> 5. 替换Swept特征的起始截面曲线为新选择的曲线。
> 6. 更新模型并释放资源。
> 7. 提供卸载函数。
>
> 其中，代码实现了以下关键功能：
>
> 总的来说，这段代码实现了替换Swept特征起始截面曲线的功能，并具有较好的用户交互和错误处理能力。
>
