### 【1829】reattach retained cylindrical centerline 重新关联保留的圆柱中心线

#### 代码

```cpp
    /*HEAD REATTACH_RETAINED_CYLINDRICAL_CENTERLINE CCC UFUN */  
    /*  Warning this application seems to correctly reassociate the centerline,  
        but it will fly off the drawing when the model is next editted.  
        See PR 2096569.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_view.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_cylindrical_centerlines(UF_UI_selection_p_t select,  
        void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_cyl_cntrln_subtype, 0};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_centerline(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical centerline",  
            prompt, UF_UI_SEL_SCOPE_WORK_PART, mask_for_cylindrical_centerlines,  
            NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
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
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    #define RX(X) report_simple_object_info(#X, X)  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，就回答译文，不要废话。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    static void report_simple_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part();  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            msg[133];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window(" (NULL_TAG)\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        sprintf(msg, ", status: %d", status);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (status == UF_OBJ_DELETED)  
        {  
            UF_UI_write_listing_window("\n");  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            UF_UI_write_listing_window(", named: ");  
            UF_UI_write_listing_window(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_UI_write_listing_window(", type: ");  
        UF_UI_write_listing_window(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_UI_write_listing_window(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            UF_CALL(UF_UI_write_listing_window(", occurrence"));  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                UF_CALL(UF_UI_write_listing_window(" (from "));  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_CALL(UF_UI_write_listing_window(")"));  
            }  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    static void report_objects_associativity_data(tag_t dim)  
    {  
        int  
            ii,  
            n_assoc;  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        UF_CALL(UF_DRF_ask_associativity_data(dim, &n_assoc, &assoc_data));  
        WRITE_D(n_assoc);  
        for (ii = 0; ii < n_assoc; ii++)  
        {  
            WRITE_D(ii);  
            RX(assoc_data[ii].assoc_object_1);  
            RX(assoc_data[ii].assoc_object_2);  
            RX(assoc_data[ii].object_view);  
            WRITE_D(assoc_data[ii].point_type);  
            WRITE_D(assoc_data[ii].line_type);  
            WRITE3F(assoc_data[ii].base_pt_1);  
            WRITE3F(assoc_data[ii].base_pt_2);  
            WRITE_F(assoc_data[ii].entered_angle);  
            WRITE3F(assoc_data[ii].assoc_point);  
        }  
        if (n_assoc > 0) UF_free(assoc_data);  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_retained;  
        int  
            n,  
            n_assoc;  
        tag_t  
            cline,  
            face;  
        double  
            centerline_origin[3];  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        UF_DRF_valid_cline_form_t  
            centerline_type;  
        UF_DRF_centerline_info_t  
            *centerline_info;  
        while (((cline = select_a_cylindrical_centerline("Reattach")) != NULL_TAG)  
            && ((face = select_a_cylindrical_face("To face")) != NULL_TAG))  
        {  
            UF_CALL(UF_DRF_is_annotation_retained(cline, &is_retained));  
            if (!is_retained)  
            {  
                WRITE_L(is_retained);  
                continue;  
            }  
            if (UF_CALL(UF_DRF_ask_associativity_data(cline, &n_assoc,  
                    &assoc_data)) ||  
                UF_CALL(UF_DRF_ask_centerline_info(cline, &centerline_type,  
                    centerline_origin, &centerline_info))) return;  
            assoc_data[0].assoc_object_1 = face;  
            assoc_data[0].point_type = UF_DRF_CYLINDRICAL_FACE_TYPE;  
            UF_CALL(UF_VIEW_map_drawing_to_model(assoc_data[0].object_view,  
                &(centerline_info->line_info[0].segment_pnts[0]),  
                assoc_data[0].assoc_point)); /* Start point of first segment 里海译:开始点 of 第一个段 */  
            assoc_data[1].assoc_object_1 = face;  
            assoc_data[1].point_type = UF_DRF_CYLINDRICAL_FACE_TYPE;  
            n = (centerline_info->line_info[0].num_segments * 6) - 3;  
            UF_CALL(UF_VIEW_map_drawing_to_model(assoc_data[1].object_view,  
                &(centerline_info->line_info[0].segment_pnts[n]),  
                assoc_data[1].assoc_point)); /* End point of last segment 里海译:终点 */  
            UF_CALL(UF_DRF_free_centerline(&centerline_info));  
            UF_CALL(UF_DRF_set_associativity_data(cline, n_assoc, assoc_data));  
            if (n_assoc > 0) UF_free(assoc_data);  
            report_objects_associativity_data(cline);  
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

> 这是用于重新关联NX中圆柱中心线到圆柱面的二次开发代码。
>
> 代码的主要功能包括：
>
> 1. 选择圆柱中心线，使用UI函数进行选择，并设置选择过滤器。
> 2. 选择圆柱面，同样使用UI函数，并设置选择过滤器。
> 3. 检查中心线是否为保留注释，如果不是则跳过。
> 4. 获取中心线的关联数据，包括关联对象、视图、点类型、线类型等信息。
> 5. 更新中心线的关联数据，将关联对象1更新为选择的圆柱面，并设置点类型为圆柱面。
> 6. 获取中心线信息，包括中心线类型、原点等信息。
> 7. 映射中心线端点到模型空间，得到关联点。
> 8. 更新中心线的关联数据，将关联点设置为端点。
> 9. 设置中心线的关联数据，完成重新关联。
> 10. 打印中心线的关联数据，用于调试。
> 11. 循环执行以上步骤，直到用户取消选择。
> 12. 提供了错误报告函数和日志打印函数，用于调试。
> 13. 定义了加载和卸载函数。
>
> 综合来看，这段代码实现了在NX中重新关联圆柱中心线到圆柱面的功能，通过UI选择对象，获取和设置关联数据，实现了二次开发功能。
>
