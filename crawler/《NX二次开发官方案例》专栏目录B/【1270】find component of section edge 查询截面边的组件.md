### 【1270】find component of section edge 查询截面边的组件

#### 代码

```cpp
    /*HEAD FIND_COMPONENT_OF_SECTION_EDGE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18。 */  
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
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_parent_loaded(tag_t object)  
    {  
        tag_t  
            instance,  
            part_occurrence;  
        UF_PART_load_status_t  
            load;  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            part_occurrence = UF_ASSEM_ask_part_occurrence(object);  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occurrence);  
            if (instance == NULL_TAG)  
            {  
                ensure_parent_loaded(part_occurrence);  
                instance = UF_ASSEM_ask_inst_of_part_occ(part_occurrence);  
            }  
            UF_ASSEM_ensure_child_loaded(instance, &load);  
            if (load.n_parts > 0) report_load_status(&load);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_parents;  
        tag_t  
            curve,  
            *parents;  
        char  
            component[MAX_FSPEC_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        while ((curve = select_a_section_edge("Select curve to query"))!=NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, TRUE));  
            UF_CALL(UF_SO_ask_parents(curve, UF_SO_ASK_ALL_PARENTS, &n_parents,  
                &parents));  
            if (n_parents > 0)  
            {  
                for (ii = 0; ii < n_parents; ii++)  
                {  
                    if (UF_ASSEM_is_occurrence(parents[ii]))  
                    {  
                        ensure_parent_loaded(parents[ii]);  
                        UF_ASSEM_ask_part_name_of_child(  
                            UF_ASSEM_ask_inst_of_part_occ(  
                            UF_ASSEM_ask_part_occurrence(parents[ii])), component);  
                        sprintf(msg, "section edge is for %s", component);  
                        uc1601(msg, TRUE);  
                    }  
                    else  
                        uc1601("section edge is not for a component", TRUE);  
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

> 这段代码是一个NX Open的UF程序，其主要功能是查询选定的截面边属于哪个组件。
>
> 主要流程如下：
>
> 1. 使用自定义的函数select_a_section_edge选择一个截面边，该函数会弹出选择对话框，并过滤出截面边。
> 2. 查询选定截面边的所有父对象，并判断父对象是否为组件实例。如果是组件实例，则查询组件名称，并显示“截面边属于XX组件”。
> 3. 重复以上步骤，直到用户选择“取消”。
> 4. 使用UF_free释放查询到的父对象数组。
> 5. 在UFusr函数中初始化UF库，调用do_it函数，最后终止UF库。
> 6. 定义unload函数，用于卸载插件。
> 7. 定义错误报告函数report_error，用于在出错时打印错误信息。
> 8. 定义选择过滤器函数filter_section_edges，用于过滤出截面边。
> 9. 定义选择掩码函数mask_for_section_edges，用于设置选择掩码。
> 10. 定义确保组件加载函数ensure_parent_loaded，用于确保组件被加载。
> 11. 使用宏UF_CALL简化UF函数调用。
>
> 总的来说，这段代码通过查询截面边的父对象，实现了查询截面边属于哪个组件的功能。
>
