### 【0849】create proj curves 创建投影曲线

#### 代码

```cpp
    /*HEAD CREATE_PROJ_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，UF_print_syslog在V18版本中是新增的。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_face_plane_datums(char *prompt, tag_t **items)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_face_plane_datums, NULL, &resp, &cnt, items));  
        objs = *items;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            n_targets;  
        tag_t  
            *curves,  
            *targets,  
            proj_feat;  
        UF_CURVE_proj_t  
            proj_data = { 1, NULL_TAG, {0,0,0}, {0,0,0}, 1, 0, 0, {0,0,0} };  
        UF_UNDO_mark_id_t  
            mark;  
        while (((n_curves = select_curves("Curves to project", &curves)) > 0) &&  
            ((n_targets = select_face_plane_datums( "project onto", &targets)) > 0))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Project Curves", &mark));  
            UF_CALL(UF_CURVE_create_proj_curves(n_curves, curves, n_targets,  
                    targets, 3, &proj_data, &proj_feat));  
            UF_free(curves);  
            UF_free(targets);  
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

> 这段代码是NX的二次开发代码，用于实现曲线投影功能。主要功能包括：
>
> 1. 错误报告函数：使用report_error函数记录并报告函数调用失败时的错误代码和信息。
> 2. 选择曲线：使用select_curves函数通过对话框让用户选择要投影的曲线，并设置选择过滤，仅允许选择线、圆、二次曲线、样条线和点。
> 3. 选择目标面或平面：使用select_face_plane_datums函数通过对话框让用户选择投影的目标面或平面，并设置选择过滤，允许选择实体面、平面和基准面。
> 4. 曲线投影：do_it函数负责循环调用选择曲线和选择目标面的函数，获取用户选择的曲线和目标面，然后调用UF_CURVE_create_proj_curves函数将曲线投影到目标面上，并设置投影方向为法向。
> 5. 撤销标记：在投影前调用UF_UNDO_set_mark函数设置撤销标记，以便用户撤销投影操作。
> 6. 程序入口：ufusr函数是程序的入口，初始化NX环境后调用do_it函数，最后结束NX环境。
> 7. 卸载函数：ufusr_ask_unload函数用于处理程序卸载时的操作。
>
> 总体而言，这段代码通过对话框交互式地实现了曲线投影功能，并提供了撤销功能，以便用户方便地调整投影结果。
>
