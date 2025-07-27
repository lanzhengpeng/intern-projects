### 【3002】trim sheets 在NX中，trim sheets指的是使用曲线修剪片体的功能

#### 代码

```cpp
    /*HEAD TRIM_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本中的新功能。 */  
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
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            ii,  
            irc,  
            n;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask[5];  
        opts.num_mask_triples = 5;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask[0].object_type = UF_line_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = 0;  
        mask[1].object_type = UF_circle_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        mask[2].object_type = UF_conic_type;  
        mask[2].object_subtype = UF_all_subtype;  
        mask[2].solid_type = 0;  
        mask[3].object_type = UF_spline_type;  
        mask[3].object_subtype = 0;  
        mask[3].solid_type = 0;  
        mask[4].object_type = UF_point_type;  
        mask[4].object_subtype = 0;  
        mask[4].solid_type = 0;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &irc, &n, curves));  
        for (ii=0; ii<n; ii++)  
            UF_CALL(UF_DISP_set_highlight(*(*curves + ii), FALSE));  
        return n;  
    }  
    static void ask_body_midpoint(tag_t body, double mid_point[3])  
    {  
        int  
            irc;  
        tag_t  
            point;  
        double  
            dist,  
            junk[3],  
            minmaxs[6];  
        FTN(uf6540)(&body, &irc, minmaxs);  
        mid_point[0] = (minmaxs[0] + minmaxs[3]) / 2;  
        mid_point[1] = (minmaxs[1] + minmaxs[4]) / 2;  
        mid_point[2] = (minmaxs[2] + minmaxs[5]) / 2;  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_CURVE_create_point(mid_point, &point));  
        UF_CALL(UF_MODL_ask_minimum_dist( point, body, FALSE, junk, FALSE,  
            junk, &dist, junk, mid_point));  
        UF_CALL(UF_OBJ_delete_object(point));  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }  
    static logical prompt_for_a_vector(char *prompt, double vec[3])  
    {  
        char  
            menu[3][16] = { "X", "Y", "Z" };  
        int  
            irc,  
            junk;  
        irc = uc1609(prompt, menu, 3, vec, &junk);  
        if (irc == 3 || irc == 4) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_gaps,  
            n;  
        tag_t  
            *curves,  
            sheet;  
        double  
            distol,  
            *gap_pts,  
            keep[3],  
            vector[3] = { 0, 1, 0 };  
        UF_MODL_trim_object_t  
            *trims;  
        UF_MODL_trim_object_p_t  
            *trim_ptrs;  
        while (  
            ((sheet = select_a_sheet("Select sheet to trim")) != NULL_TAG) &&  
            ((n = select_curves("Select bounding curves", &curves)) != 0) &&  
            ((prompt_for_a_vector("Projection along", vector))))  
        {  
            ask_body_midpoint(sheet, keep);  
            UF_MODL_ask_distance_tolerance(&distol);  
            trims = (UF_MODL_trim_object_t *)malloc(n *  
                sizeof(UF_MODL_trim_object_t));  
            trim_ptrs = (UF_MODL_trim_object_p_t *)malloc(n *  
                sizeof(UF_MODL_trim_object_p_t));  
            for (ii = 0; ii < n; ii++)  
            {  
                trim_ptrs[ii] = &trims[ii];  
                trims[ii].object_tag = curves[ii];  
                trims[ii].curve_project_method = 2;  
            }  
            UF_free(curves);  
            UF_CALL(UF_MODL_trim_sheet(sheet, n, trim_ptrs, vector, 1, 1, keep,  
                distol, &n_gaps, &gap_pts));  
            if (n_gaps) UF_free(gap_pts);  
            free(trims);  
            free(trim_ptrs);  
        }  
    }  
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

> 这段代码是用于NX的二次开发，其主要功能是对模型进行修剪。代码的主要流程包括以下步骤：
>
> 1. 初始化：首先调用UF_initialize来初始化NX环境。
> 2. 主体选择：使用自定义的选择对话框，让用户选择要修剪的片体(sheet)。
> 3. 曲线选择：通过另一个自定义的选择对话框，让用户选择作为修剪边界的曲线。
> 4. 投影方向：提示用户输入修剪的投影方向。
> 5. 修剪：根据用户选择的片体、曲线和投影方向，调用UF_MODL_trim_sheet进行修剪操作。
> 6. 清理：释放内存，调用UF_terminate终止NX环境。
> 7. 卸载：通过ufusr_ask_unload函数卸载用户自定义模块。
> 8. 错误处理：通过UF_CALL宏来捕获并处理函数调用错误。
> 9. 选择过滤：使用自定义的选择过滤函数来限定用户只能选择片体。
> 10. 提示对话框：通过自定义的提示对话框获取用户输入。
>
> 该代码实现了对NX模型进行修剪的基本流程，同时包含了错误处理和内存管理等机制。
>
