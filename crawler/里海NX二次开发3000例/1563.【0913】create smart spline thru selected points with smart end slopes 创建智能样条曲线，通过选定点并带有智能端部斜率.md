### 【0913】create smart spline thru selected points with smart end slopes 创建智能样条曲线，通过选定点并带有智能端部斜率

#### 代码

```cpp
    /*HEAD CREATE_SMART_SPLINE_THRU_SELECTED_POINTS_WITH_SMART_END_SLOPES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_vec.h>  
    #include <uf_curve.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_points(char *prompt, tag_t **points)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select points", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_points, NULL, &resp, &cnt, points));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*points)[ii], FALSE));  
        return cnt;  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_pts,  
            resp;  
        tag_t  
            bcurve,  
            end_slope,  
            e_spt,  
            *pts,  
            s_spt,  
            two_pts[2],  
            start_slope;  
        UF_SO_spline_data_t  
            spline_data = { 0, 3, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, {0,0}};  
        if (((n_pts = select_points("smart spline thru points", &pts)) > 0) &&  
            ((s_spt = select_a_point("Start slope toward")) != NULL_TAG) &&  
            ((e_spt = select_a_point("End slope toward")) != NULL_TAG))  
        {  
            set_undo_mark("Create smart Spline");  
            two_pts[0] = pts[0];  
            two_pts[1] = s_spt;  
            UF_CALL(UF_SO_create_dirr_two_points(s_spt,  
                UF_SO_update_within_modeling, two_pts, &start_slope));  
            two_pts[0] = pts[n_pts-1];  
            two_pts[1] = e_spt;  
            UF_CALL(UF_SO_create_dirr_two_points(s_spt,  
                UF_SO_update_within_modeling, two_pts, &end_slope));  
            spline_data.nump = n_pts;  
            spline_data.itype = UF_allocate_memory(n_pts*sizeof(int), &resp);  
            spline_data.position = pts;  
            spline_data.direction = UF_allocate_memory(n_pts*sizeof(tag_t), &resp);  
            spline_data.itype[0] = 1;  
            spline_data.direction[0] = start_slope;  
            for (ii = 1; ii < n_pts-1; ii++)  
            {  
                spline_data.itype[ii] = 0;  
                spline_data.direction[ii] = NULL_TAG;  
            }  
            spline_data.itype[n_pts-1] = 1;  
            spline_data.direction[n_pts-1] = end_slope;  
            UF_CALL(UF_SO_create_spline(UF_SO_update_within_modeling,  
                &spline_data, &bcurve));  
            UF_CALL(UF_SO_set_visibility_option(bcurve, UF_SO_visible));  
            UF_free(pts);  
            UF_free(spline_data.itype);  
            UF_free(spline_data.direction);  
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

> 这段代码是用于在NX中创建智能样条曲线的二次开发代码。其主要功能包括：
>
> 1. 选择点：使用UI接口让用户选择要经过的点，并存储这些点的tag。
> 2. 选择起始和终止斜率点：使用UI接口让用户选择起始和终止斜率指向的点。
> 3. 设置撤销标记：在创建样条曲线前设置撤销标记，以便用户可以撤销操作。
> 4. 创建方向矢量：根据选择的起始和终止斜率点，使用SO模块创建方向矢量。
> 5. 创建样条曲线：使用SO模块的create_spline函数，根据用户选择的点以及起始和终止斜率方向矢量，创建智能样条曲线。
> 6. 设置样条曲线可见：将创建的样条曲线设置为可见。
> 7. 清理内存：释放分配的内存。
> 8. 撤销操作：如果创建样条曲线失败，用户可以通过撤销标记来撤销操作。
>
> 这段代码实现了用户交互式创建智能样条曲线的功能，通过UI接口获取用户输入，并利用SO模块创建样条曲线。
>
