### 【2153】report if selected line is from a section 报告选择的线是否来自一个剖面

#### 代码

```cpp
    /*HEAD REPORT_IF_SELECTED_LINE_IS_FROM_A_SECTION CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的函数。

翻译：UF_print_syslog 是 V18 版本中新增的函数。 */  
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
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_section_segment_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
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
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_section_segment_curves(tag_t part, tag_t **objects)  
    {  
        int  
            ii,  
            n_segs;  
        tag_t  
            object,  
            *sxsegs;  
        uf_list_p_t  
            object_list;  
        UF_DRF_object_t  
            *drf_object;  
        UF_DRAW_sxseg_info_t  
            sxseg_info;  
        n_segs = ask_all_of_type(part, UF_section_segment_type, &sxsegs);  
        if (n_segs == 0) return 0;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < n_segs; ii++)  
        {  
            UF_CALL(UF_DRAW_ask_sxline_sxseg(sxsegs[ii], &sxseg_info, &object,  
                &drf_object));  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
            UF_free(drf_object);  
        }  
        UF_free(sxsegs);  
        return (make_an_array(&object_list, objects));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_sxsegs,  
            subtype,  
            type;  
        tag_t  
            group,  
            curve,  
            part = UF_PART_ask_display_part(),  
            *sxseg_curves;  
    /*  Begin work around to PR 5622968 里海译:开始围绕PR 5622968展开工作。 */  
        n_sxsegs = ask_all_section_segment_curves(part, &sxseg_curves);  
    /*  End work around to PR 5622968 里海译:翻译：解决PR 5622968的相关工作。 */  
        while ((curve = select_a_line("Select line to query")) != NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, TRUE));  
            UF_CALL(UF_DRAW_ask_group_of_curve(curve, &group, &type, &subtype));  
            WRITE_D(group);  
            WRITE_D(type);  
            WRITE_D(subtype);  
            if ((group != NULL_TAG) && (type == UF_solid_section_type))  
                uc1601("This is a section line", TRUE);  
            else  
                uc1601("This is NOT a section line", TRUE);  
        /*  Begin work around to PR 5622968 里海译:开始围绕PR 5622968展开工作。 */  
            for (ii = 0; ii < n_sxsegs; ii++)  
            {  
                if (curve == sxseg_curves[ii])  
                {  
                    uc1601("Yes it is!", TRUE);  
                    break;  
                }  
            }  
        /*  End work around to PR 5622968 里海译:翻译：绕过PR 5622968的修复工作。 */  
            UF_CALL(UF_DISP_set_highlight(curve, FALSE));  
        }  
        if (n_sxsegs > 0) UF_free(sxseg_curves);  
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

> 这段代码是用于NX CAD软件的二次开发代码，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时打印错误信息。
> 2. 定义了一个掩码函数mask_for_lines，用于在选择对话框中过滤出线类型、实体边线、截面线段和截面边线。
> 3. 定义了一个选择线函数select_a_line，用于弹出一个选择对话框，让用户选择一条线，并返回该线的tag。
> 4. 定义了一个写整数到列表窗口的函数write_integer_to_listing_window，用于将整数信息输出到NX的列表窗口。
> 5. 定义了一个内存分配函数allocate_memory，用于分配指定大小的内存。
> 6. 定义了一个生成数组函数make_an_array，用于将列表转换成数组。
> 7. 定义了一个查询类型函数ask_all_of_type，用于查询指定类型的所有对象。
> 8. 定义了一个查询所有截面线段函数ask_all_section_segment_curves，用于查询当前部件中的所有截面线段。
> 9. 定义了主函数do_it，用于循环让用户选择线，并判断线是否为截面线，同时绕过了一个bug。
> 10. 定义了ufusr入口函数，用于初始化、调用主函数、终止。
> 11. 定义了一个卸载函数ufusr_ask_unload，用于设置立即卸载。
>
> 总体来说，这段代码实现了一个简单的NX二次开发程序，让用户可以交互式地选择线，并判断线是否为截面线。
>
