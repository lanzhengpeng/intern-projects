### 【2354】report selected objects associativity data 报告选定对象的关联数据

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_OBJECTS_ASSOCIATIVITY_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
    #include <uf_view.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
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
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    /* qq3123197280 */  
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 },  
                        { UF_tabular_note_type, UF_all_subtype, 0 },  
                        { UF_tol_feature_instance_type, 0, 0} };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select dimension or drafting aid",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
            case UF_circle_type: /* non-std format in the header file 里海译:非标准格式头文件 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，就回答找不到。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    if (strcmp(define, "gfem_load")) break;  
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
        sprintf(msg, "%s = %d", what, object);  
        ECHO(msg);  
        if (object == NULL_TAG)  
        {  
            ECHO(" (NULL_TAG)\n");  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        sprintf(msg, ", status: %d", status);  
        ECHO(msg);  
        if (status == UF_OBJ_DELETED)  
        {  
            ECHO("\n");  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            ECHO(", named: ");  
            ECHO(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        ECHO(", type: ");  
        ECHO(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            ECHO(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            ECHO(", occurrence");  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                ECHO(" (from ");  
                ECHO(msg);  
                ECHO(")");  
            }  
        }  
        ECHO("\n");  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
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
            display_temporary_point(assoc_data[ii].assoc_point);  
        }  
        if (n_assoc > 0) UF_free(assoc_data);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dim;  
        while ((dim = select_a_dim_or_draft("Report associativies")) != NULL_TAG)  
        {  
            RX(dim);  
            report_objects_associativity_data(dim);  
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

> 这段代码是一个NX的二次开发程序，主要功能是选择一个尺寸或绘图辅助对象，并报告该对象的关联性数据。下面是代码的主要功能介绍：
>
> 1. 定义了错误报告和输出日志的宏。
> 2. 定义了选择尺寸或绘图辅助对象的函数select_a_dim_or_draft，它会显示一个选择对话框，用户可以选择一个对象，并返回该对象的标签。
> 3. 定义了报告对象简单信息的函数report_simple_object_info，用于输出对象的状态、名称、类型等信息。
> 4. 定义了显示临时点的函数display_temporary_point，用于在图形窗口中显示一个点。
> 5. 定义了报告对象关联性数据的函数report_objects_associativity_data，它会调用UF_DRF_ask_associativity_data函数获取对象的关联性数据，并输出到日志窗口。
> 6. 定义了主函数do_it，它会循环调用select_a_dim_or_draft选择对象，然后调用report_objects_associativity_data报告关联性数据。
> 7. 定义了ufusr函数作为程序的入口，它会初始化NX环境，调用do_it执行主逻辑，然后终止NX环境。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序可以立即卸载。
>
> 综上所述，这段代码的主要功能是选择一个尺寸或绘图辅助对象，并报告该对象的关联性数据，包括关联对象、视图、点类型、线类型、基点等信息，并通过日志窗口输出这些信息。
>
