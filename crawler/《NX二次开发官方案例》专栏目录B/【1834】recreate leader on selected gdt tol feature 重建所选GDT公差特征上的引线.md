### 【1834】recreate leader on selected gdt tol feature 重建所选GDT公差特征上的引线

#### 代码

```cpp
    /*HEAD RECREATE_LEADER_ON_SELECTED_GDT_TOL_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_gdt.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18版本新增的功能。 */  
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
    static int mask_gdt(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            triples = { UF_tol_feature_instance_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &triples)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_gdt(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a GDT", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_gdt, NULL, &resp, &object, cp, &view));  
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
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
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
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中，使用非标准格式进行声明。 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话， */  
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
            owner;  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            owning_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[133];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window(" (NULL_TAG)\n\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        if (status == UF_OBJ_DELETED)  
        {  
            sprintf(msg, ", status: %d\n", status);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_UI_write_listing_window(", occurrence");  
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
        {  
            ask_part_shortname(owner, owning_part);  
            UF_UI_write_listing_window(", in: ");  
            UF_UI_write_listing_window(owning_part);  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void report_gdt_leader_spec(UF_GDT_leader_spec_p_t spec)  
    {  
        RX(spec->view);  
        RX(spec->terminator);  
        WRITE3F(spec->attach_point.coords);  
        display_temporary_point(spec->attach_point.coords);  
        WRITE3F(spec->stub_points[0].coords);  
        display_temporary_point(spec->stub_points[0].coords);  
        WRITE3F(spec->stub_points[1].coords);  
        display_temporary_point(spec->stub_points[1].coords);  
        WRITE_D(spec->num_intermediates);  
        WRITE_D(spec->type);  
        WRITE_L(spec->is_profile_leader);  
        WRITE_L(spec->filled);  
        WRITE_L(spec->all_around);  
        WRITE_F(spec->all_around_diameter);  
        WRITE_F(spec->dot_diameter);  
        WRITE_F(spec->arrowhead_length);  
        WRITE_F(spec->arrowhead_angle);  
        WRITE_F(spec->stub_length);  
        WRITE_D(spec->direction);  
        if (spec->is_profile_leader)  
        {  
            RX(spec->profile_term2);  
            WRITE3F(spec->profile_attach.coords);  
            display_temporary_point(spec->profile_attach.coords);  
            WRITE_F(spec->profile_ldr_length);  
        }  
        WRITE_D(spec->leader_color);  
        WRITE_D(spec->leader_font);  
        WRITE_D(spec->leader_width);  
        WRITE_D(spec->arrowhead_color);  
        WRITE_D(spec->arrowhead_font);  
        WRITE_D(spec->arrowhead_width);  
        WRITE_D(spec->arrow_info.sequence_number);  
        WRITE_D(spec->arrow_info.arrow_type);  
        WRITE_D(spec->arrow_info.arrow_fill);  
        WRITE3F(spec->arrow_info.arrow_origin);  
        display_temporary_point(spec->arrow_info.arrow_origin);  
        WRITE_F(spec->arrow_info.arrow_angle);  
        WRITE_F(spec->arrow_info.arrow_include_angle);  
        WRITE_F(spec->arrow_info.arrow_height);  
        WRITE_F(spec->arrow_info.arrow_width);  
    }  
    static void do_it(void)  
    {  
        int  
            n_leaders;  
        tag_t  
            gdt,  
            gdt_feat;  
        UF_GDT_leader_spec_t  
            spec;  
        if (((gdt = select_a_gdt("Remove/Add Leader")) != NULL_TAG) &&  
            !UF_CALL(UF_GDT_ask_num_leaders( gdt, &n_leaders )) && (n_leaders > 0))  
        {  
            WRITE_D(n_leaders);  
            UF_CALL(UF_GDT_ask_leader(gdt, 0, &spec));  
            report_gdt_leader_spec(&spec);  
            UF_CALL(UF_GDT_remove_leader(gdt, 0));  
            UF_CALL(UF_GDT_ask_tol_feat_of_instance(gdt, &gdt_feat));  
            UF_CALL(UF_GDT_update_features(1, &gdt_feat));  
        /*  This fails - see PR 2077116 里海译:翻译为：这个方法失败了 - 请看PR 2077116。 */  
            UF_CALL(UF_GDT_add_leader(gdt, &spec));  
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

> 这是用于在NX中添加或删除公差标注(General Dimensioning and Tolerancing, GDT)标注线的二次开发代码。
>
> 主要功能包括：
>
> 1. 选择GDT特征：使用对话框让用户选择一个GDT特征。
> 2. 获取标注线信息：获取选中的GDT特征的所有标注线信息，并显示在列表窗口中。
> 3. 删除标注线：调用UF_GDT_remove_leader()删除GDT特征的第一条标注线。
> 4. 添加标注线：调用UF_GDT_add_leader()使用原来的标注线信息，重新添加标注线到GDT特征上。
> 5. 错误处理：使用宏定义UF_CALL()，可以捕捉函数调用错误，并显示错误信息。
> 6. 日志信息：使用WRITE_XXX()系列宏，将信息输出到列表窗口。
> 7. 对象信息：使用RX()宏，可以获取并显示NX对象的基本信息。
> 8. 临时点显示：使用display_temporary_point()函数，可以临时显示一个点。
> 9. 二次开发标准流程：包括UF_initialize()和UF_terminate()来初始化和关闭NX。
>
> 总体而言，这段代码实现了在NX中添加或删除GDT标注线的功能，并采用了常见的错误处理、日志输出和对象信息获取等二次开发技术。
>
