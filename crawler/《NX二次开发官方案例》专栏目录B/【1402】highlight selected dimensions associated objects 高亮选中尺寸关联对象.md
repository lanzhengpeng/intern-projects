### 【1402】highlight selected dimensions associated objects 高亮选中尺寸关联对象

#### 代码

```cpp
    /*HEAD HIGHLIGHT_SELECTED_DIMENSIONS_ASSOCIATED_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_eval.h>  
    #include <uf_disp.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog是V18版本中的新增功能。因此，翻译如下：

UF_print_syslog是V18版本中新增的功能。 */  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_object_type_and_subtype(tag_t object)  
    {  
        int  
            subtype,  
            type;  
        WRITE_D(object);  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            WRITE_D(type);  
            WRITE_D(subtype);  
        }  
    }  
    /*  This does not work because the assoc_modifier is alway 1 - see PR 4701110 里海译:这个方法不起作用，因为assoc_modifier始终为1 - 参见PR 4701110。 */  
    static logical convert_assoc_info_to_model_location(  
        UF_DRF_assoc_info_p_t assoc_obj, double loc[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            eval;  
        switch (assoc_obj->assoc_type)  
        {  
            case UF_DRF_end_point:  
                UF_CALL(UF_EVAL_initialize(assoc_obj->assoc_object_tag, &eval));  
                UF_CALL(UF_EVAL_ask_limits(eval, limits));  
                UF_CALL(UF_EVAL_evaluate(eval, 0,  
                    limits[assoc_obj->assoc_modifier - 1], loc, NULL));  
                UF_CALL(UF_EVAL_free(eval));  
                return TRUE;  
            default:  
                return FALSE;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            subtype;  
        tag_t  
            dim;  
        double  
            loc[3],  
            origin[3];  
        UF_DRF_dim_info_t  
            *info;  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        while ((dim = select_a_dimension("Highlight assoc objects")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_DRF_ask_dim_info(dim, &subtype, origin, &info)))  
                continue;  
            UF_CALL(UF_DISP_set_highlight(dim, TRUE));  
            WRITE_D(info->dim_line_info->num_assoc_objs);  
            for (ii = 0; ii < info->dim_line_info->num_assoc_objs; ii++)  
            {  
                UF_CALL(UF_DISP_set_highlight(info->dim_line_info->assoc_objs[ii].  
                    assoc_object_tag, TRUE));  
                WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_tag);  
                report_object_type_and_subtype(info->dim_line_info->assoc_objs[ii].  
                    assoc_object_tag);  
                WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_view_tag);  
                WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_type);  
                WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_modifier);  
                if (convert_assoc_info_to_model_location(&info->dim_line_info->  
                    assoc_objs[ii], loc))  
                {  
                    WRITE3F(loc);  
                    UF_CALL(UF_DISP_display_temporary_point(info->dim_line_info->  
                        assoc_objs[ii].assoc_object_view_tag, UF_DISP_USE_VIEW_TAG,  
                        loc, &attrib, UF_DISP_POINT));  
                }  
            }  
            if (info->num_arcs > 0)  
            {  
                WRITE_D(info->arc_info[0].num_assoc_objs);  
                for (jj = 0; jj < info->arc_info[0].num_assoc_objs; jj++)  
                {  
                    UF_CALL(UF_DISP_set_highlight(info->arc_info[0].assoc_objs[jj].  
                        assoc_object_tag, TRUE));  
                    WRITE_D(info->arc_info[0].assoc_objs[jj].assoc_object_tag);  
                    report_object_type_and_subtype(info->arc_info[0].assoc_objs[jj].  
                        assoc_object_tag);  
                    WRITE_D(info->arc_info[0].assoc_objs[jj].assoc_object_view_tag);  
                    WRITE_D(info->arc_info[0].assoc_objs[jj].assoc_type);  
                    WRITE_D(info->arc_info[0].assoc_objs[jj].assoc_modifier);  
                    if (convert_assoc_info_to_model_location(&info->arc_info[0].  
                        assoc_objs[jj], loc))  
                    {  
                        WRITE3F(loc);  
                        UF_CALL(UF_DISP_display_temporary_point(info->arc_info[0].  
                            assoc_objs[jj].assoc_object_view_tag,  
                            UF_DISP_USE_VIEW_TAG, loc, &attrib, UF_DISP_POINT));  
                    }  
                }  
            }  
            uc1601("Associated objects highlighted", TRUE);  
            UF_CALL(UF_DISP_set_highlight(dim, FALSE));  
            for (ii = 0; ii < info->dim_line_info->num_assoc_objs; ii++)  
            {  
                UF_CALL(UF_DISP_set_highlight(info->dim_line_info->assoc_objs[ii].  
                    assoc_object_tag, FALSE));  
            }  
            if (info->num_arcs > 0)  
            {  
                for (jj = 0; jj < info->arc_info[0].num_assoc_objs; jj++)  
                {  
                    UF_CALL(UF_DISP_set_highlight(info->arc_info[0].assoc_objs[jj].  
                        assoc_object_tag, FALSE));  
                }  
            }  
            UF_CALL(UF_DRF_free_dimension(&info));  
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

> 这段代码是NX Open C++ API编写的NX二次开发示例代码，主要功能是高亮显示关联对象。
>
> 代码的关键点如下：
>
> 1. 使用UF_UI模块中的函数来选择一个标注(dim)，并获取其关联对象的信息，包括关联对象的对象标签、视图标签、关联类型和关联修饰符等。
> 2. 通过UF_DISP模块中的函数高亮显示选定的标注和关联对象。
> 3. 对于关联对象，尝试将其关联信息转换为模型空间中的位置，并在该位置显示一个点。
> 4. 使用UF_DRF模块中的函数来获取标注的详细信息，包括标注线段和圆弧的关联对象信息。
> 5. 在列表窗口中输出与标注关联的对象信息，包括对象标签、类型、子类型、视图标签、关联类型和关联修饰符等。
> 6. 使用UF_EVAL模块中的函数计算关联对象的位置。
> 7. 使用UF_OBJ模块中的函数获取对象的类型和子类型信息。
> 8. 定义了错误报告函数report_error，用于输出API调用失败的错误信息。
> 9. 定义了整数和浮点数数组的输出函数，用于在列表窗口中输出信息。
> 10. 定义了选择标注的函数select_a_dimension，并实现了自定义选择掩码。
> 11. 主函数do_it实现了主要功能。
> 12. ufusr是NX二次开发的标准入口函数。
> 13. ufusr_ask_unload函数实现了卸载请求。
>
> 该代码通过调用NX Open C++ API中的多个模块，实现了选择标注、获取关联信息、高亮显示和位置计算等功能，是学习NX二次开发的一个很好的示例。
>
