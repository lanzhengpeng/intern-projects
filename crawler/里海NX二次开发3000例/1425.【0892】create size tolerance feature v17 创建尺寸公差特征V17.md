### 【0892】create size tolerance feature v17 创建尺寸公差特征V17

#### 代码

```cpp
    /*HEAD CREATE_SIZE_TOLERANCE_FEATURE_V17 CCC UFUN */  
    /*  This example has been updated to work in V17.0.1 - the UF_GDT structures  
        are different in each UG version, so do not attempt to use this in an  
        earlier version.  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_drf.h>  
    #include <uf_drf_types.h>  
    #include <uf_gdt.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，V18中新增了UF_print_syslog函数。因此，我的回答是：UF_print_syslog是在V18版本中新增的函数。 */  
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
    /* 里海 */  
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
    static tag_t select_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            sub_type;  
        double  
            dim_orig[3];  
        tag_t  
            sz_tol_tag,  
            faces[1];  
        UF_GDT_feature_parms_t  
            feat_parms;  
        UF_GDT_size_tolerance_t  
            size_tol;  
        UF_GDT_size_tolerance_p_t  
            size_data;  
        UF_DRF_dim_info_p_t  
            dim_info;  
        UF_GDT_description_t  
            my_descr = { "Size tolerance", "my description" };  
        UF_CALL(UF_UI_set_cursor_view(0));  
        faces[0] = select_cylindrical_face("select hole face");  
        feat_parms.num_faces = 1;  
        feat_parms.face_list = faces;  
        feat_parms.descript = &my_descr;  
        feat_parms.feature = UF_GDT_HOLE_FEATURE;  
        feat_parms.pattern = UF_GDT_NO_PATTERN;  
        feat_parms.modifier_data = NULL;  
        feat_parms.num_sets = 0;  
        feat_parms.sets = NULL;  
        feat_parms.def.type = UF_GDT_ALL_GEOMETRY;  
        feat_parms.def.so_tag = NULL_TAG;  
        feat_parms.def.radius = 0.0;  
        feat_parms.def.trimmed_sheet = NULL_TAG;  
        size_tol.value_type = UF_GDT_DIAMETRAL_SIZE;  
        size_tol.dimension = select_a_dimension("select dimension to associate");  
        UF_CALL(UF_DRF_ask_dim_info(size_tol.dimension, &sub_type, dim_orig,  
                                      &dim_info));  
        size_tol.nominal_value= atof(dim_info->text_info[0].text[0].string);  
        UF_DRF_free_dimension(&dim_info);  
        UF_CALL(UF_MODL_create_exp_tag("0.5", &size_tol.upper_tol.expression));  
        UF_CALL(UF_MODL_create_exp_tag("-0.5", &size_tol.lower_tol.expression));  
        UF_CALL(UF_MODL_ask_exp_tag_value(size_tol.upper_tol.expression,  
                                          &size_tol.upper_tol.value));  
        UF_CALL(UF_MODL_ask_exp_tag_value(size_tol.lower_tol.expression,  
                                          &size_tol.lower_tol.value));  
        size_tol.upper_tol.decimal_format = 4;  
        size_tol.lower_tol.decimal_format = 4;  
        size_tol.tol_format = UF_GDT_LIM_PLUS_BEFORE_MINUS;  
        size_tol.modifier_data = NULL;  
        size_tol.decimal_format = 4;  
        if (!UF_CALL(UF_GDT_create_size_tolerance(&feat_parms, &size_tol,  
            &sz_tol_tag)))  
        {  
            ECHO(sz_tol_tag);  
            UF_CALL(UF_GDT_ask_size_tolerance_parms(sz_tol_tag, &size_data));  
            printf("nominal value for tolerance feature is : %f\n",  
                size_data->nominal_value);  
            UF_CALL(UF_GDT_free(UF_GDT_SIZE_TOL_TYPE, size_data));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，用于创建一个尺寸公差特征。以下是代码的主要功能和流程：
>
> 1. 引入所需的头文件，包括NX Open API的UF模块。
> 2. 定义一个错误报告函数report_error，用于输出UF函数调用的错误信息。
> 3. 定义两个选择函数mask_for_cylindrical_faces和mask_for_dimensions，分别用于选择圆柱面和尺寸。
> 4. 主函数do_it中：
>
> 1. ufusr是NX调用的主函数，其中调用do_it执行创建公差特征的操作。
> 2. ufusr_ask_unload函数返回立即卸载，表示二次开发应用程序不需要保留在内存中。
>
> 总的来说，这段代码实现了在NX中通过选择面和尺寸来创建一个尺寸公差特征的功能。
>
