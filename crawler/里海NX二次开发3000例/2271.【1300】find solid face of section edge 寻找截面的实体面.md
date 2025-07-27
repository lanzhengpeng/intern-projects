### 【1300】find solid face of section edge 寻找截面的实体面

#### 代码

```cpp
    /*HEAD FIND_SOLID_FACE_OF_SECTION_EDGE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是在V18版本中新增的，请只回答翻译内容，不要添加其他无关内容。 */  
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
    static int filter_section_edges(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            subtype,  
            gtype;  
        tag_t  
            group;  
        UF_CALL(UF_DRAW_ask_group_of_curve(object, &group, &gtype, &subtype));  
        if ((group != NULL_TAG) && (gtype == UF_solid_section_type))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* qq3123197280 */  
    static int mask_for_section_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[6] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 6, mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_section_edges, NULL,NULL)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_section_edge(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_section_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%f", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }  
    static void report_component_data(tag_t comp)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        printf("\npart_name = %s\n", part_name);  
        printf("refset_name = %s\n", refset_name);  
        printf("instance_name = %s\n", instance_name);  
        report_double_array("origin", origin, 1, 3);  
        report_double_array("csys_matrix", csys_matrix, 3, 3);  
        report_double_array("transform", (double *)transform, 4, 4);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_parents;  
        tag_t  
            curve,  
            *parents;  
        while ((curve = select_a_section_edge("Select curve to query"))!=NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, TRUE));  
            UF_CALL(UF_SO_ask_parents(curve, UF_SO_ASK_ALL_PARENTS, &n_parents,  
                &parents));  
            if (n_parents > 0)  
            {  
                for (ii = 0; ii < n_parents; ii++)  
                    UF_CALL(UF_DISP_set_highlight(parents[ii], TRUE));  
                uc1601("Sectioned face highlighted", TRUE);  
                for (ii = 0; ii < n_parents; ii++)  
                {  
                    UF_CALL(UF_DISP_set_highlight(parents[ii], FALSE));  
                    if (UF_ASSEM_is_occurrence(parents[ii]))  
                        report_component_data(UF_ASSEM_ask_part_occurrence(  
                            parents[ii]));  
                }  
                UF_free(parents);  
            }  
            UF_CALL(UF_DISP_set_highlight(curve, FALSE));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告函数：用于在函数调用失败时输出错误信息，便于调试。
> 2. 过滤函数：用于过滤出截面线，只接受属于实体截面类型的曲线。
> 3. 选择掩码：用于设置选择时只显示曲线类型。
> 4. 选择函数：用于提示用户选择一个曲线，并返回所选曲线的tag。
> 5. 报告函数：用于打印出数组的值。
> 6. 报告组件数据：用于打印出组件的详细信息，包括名称、坐标系等。
> 7. 主要逻辑：循环提示用户选择曲线，对每条曲线，获取其父实体，并高亮显示。如果父实体是组件，则打印组件信息。这是代码的核心功能。
> 8. ufusr函数：NX二次开发的入口函数，用于初始化、调用主要逻辑、终止。
> 9. 卸载函数：用于在二次开发模块卸载时调用，返回立即卸载。
>
> 总体来说，这段代码主要用于在NX中选择截面线，并获取其父实体，如果父实体是组件，则打印组件信息，实现了一个简单的NX二次开发示例。
>
