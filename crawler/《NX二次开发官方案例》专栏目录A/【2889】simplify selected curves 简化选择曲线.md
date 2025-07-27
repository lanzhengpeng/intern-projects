### 【2889】simplify selected curves 简化选择曲线

#### 代码

```cpp
    /*HEAD SIMPLIFY_SELECTED_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的，请不要添加任何废话，只回答译文：

UF_print_syslog是V18版本新增的功能。 */  
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
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_curves,  
            n_segments;  
        tag_t  
            *curves,  
            *segments;  
        UF_UNDO_mark_id_t  
            mark_id,  
            temp_mark;  
        double  
            tolerance;  
        char  
            msg[133];  
        uf_list_p_t  
            curve_list,  
            fail_list;  
        UF_MODL_ask_distance_tolerance(&tolerance);  
        while ((n_curves = select_curves_or_edges("Simplify", &curves)) > 0)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Simplify Curves", &mark_id));  
            UF_CALL(UF_MODL_create_list(&fail_list));  
            if (UF_CURVE_create_simplified_curve(n_curves, curves, tolerance,  
                &n_segments, &segments))  
            {  
            /*  Back out curves created even though the simplify process failed 里海译:尽管简化过程失败，仍创建了回退曲线。 */  
                UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
            /*  try one at a time since all at once failed 里海译:尝试一次一个，因为一次性全部失败了。 */  
                UF_CALL(UF_MODL_create_list(&curve_list));  
                for (ii = 0; ii < n_curves; ii++)  
                {  
                    UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &temp_mark));  
                    if (!UF_CURVE_create_simplified_curve(1, &curves[ii],  
                        tolerance, &n_segments, &segments))  
                    {  
                        for (jj = 0; jj < n_segments; jj++)  
                            UF_CALL(UF_MODL_put_list_item(curve_list,  
                                segments[jj]));  
                        if (n_segments > 0) UF_free(segments);  
                    }  
                    else  
                    {  
                        UF_CALL(UF_UNDO_undo_to_mark(temp_mark, NULL));  
                        UF_CALL(UF_MODL_put_list_item(fail_list, curves[ii]));  
                    }  
                    UF_CALL(UF_UNDO_delete_mark(temp_mark, NULL));  
                }  
                n_segments = make_an_array(&curve_list, &segments);  
            }  
            set_highlight_object_array(n_segments, segments, TRUE);  
            sprintf(msg, "%d lines/arcs created", n_segments);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_segments, segments, FALSE);  
            if (n_segments > 0) UF_free(segments);  
            n_segments = make_an_array(&fail_list, &segments);  
            if (n_segments > 0)  
            {  
                set_highlight_object_array(n_segments, segments, TRUE);  
                sprintf(msg, "%d curves failed to simplify", n_segments);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n_segments, segments, FALSE);  
                UF_free(segments);  
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

> 这是段用于NX的二次开发代码，其主要功能是简化曲线和边缘。以下是代码的主要功能和流程：
>
> 1. 错误报告函数：定义了report_error函数，用于在出现UF函数调用错误时输出错误信息。
> 2. 选择曲线和边缘：定义了mask_for_curves_and_edges函数，用于设置选择掩码，只允许选择曲线和边缘；定义了select_curves_or_edges函数，用于弹出对话框让用户选择曲线和边缘。
> 3. 内存分配和数组处理：定义了allocate_memory函数，用于分配内存；定义了make_an_array函数，用于将列表转换为数组。
> 4. 简化曲线和边缘：定义了do_it函数，用于简化用户选择的曲线和边缘。首先尝试一次性简化所有曲线，如果失败，则逐个尝试。简化成功后，高亮显示新生成的线段/圆弧，并统计成功和失败的曲线数量。
> 5. 主函数：定义了ufusr函数，用于在NX启动时初始化并调用do_it函数进行操作。在NX关闭时，定义了ufusr_ask_unload函数，用于立即卸载。
>
> 综上所述，该代码实现了简化曲线和边缘的功能，采用了错误处理、选择、内存管理、数组处理等机制。
>
