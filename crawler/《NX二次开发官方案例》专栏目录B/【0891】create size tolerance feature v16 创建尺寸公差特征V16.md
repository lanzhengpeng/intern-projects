### 【0891】create size tolerance feature v16 创建尺寸公差特征V16

#### 代码

```cpp
    /*HEAD CREATE_SIZE_TOLERANCE_FEATURE_V16 CCC UFUN */  
    /*  This example is designed to work in V16.0 - the UF_GDT structures  
        are different in each UG version, so do not attempt to use this in an  
        earlier or later version.  
        This version does not work - see PR 4248385!  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 版本新增的功能。

功能说明：UF_print_syslog 是一个函数，用于在 V18 版本的 FortiGate 中打印系统日志。 */  
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
            *size_data,  
            size_tol;  
        UF_GDT_description_t  
            my_descr = { "Size tolerance", "my description" };  
        UF_GDT_modifier_data_t  
            none = { 0, NULL, NULL, NULL, NULL, NULL, NULL, 0, NULL };  
        UF_DRF_dim_info_p_t  
            dim_info;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        faces[0] = select_cylindrical_face("select hole face");  
        feat_parms.num_faces = 1;  
        feat_parms.face_list = faces;  
        feat_parms.descript = &my_descr;  
        feat_parms.feature = UF_GDT_HOLE_FEATURE;  
        feat_parms.pattern = UF_GDT_NO_PATTERN;  
        feat_parms.modifier_data = &none;  
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
        size_tol.modifier_data = &none;  
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

> 这是段NX二次开发代码，主要实现了创建尺寸公差特征的功能。以下是代码的主要内容和步骤：
>
> 1. 引入头文件：引入了NX API的相关头文件，如uf.h、uf_modl.h等。
> 2. 定义宏：定义了ECHO和UF_CALL两个宏，用于打印变量值和检查NX API函数调用是否成功。
> 3. 定义错误处理函数：定义了report_error函数，用于打印NX API函数调用失败时的错误信息。
> 4. 定义选择面和尺寸的函数：定义了mask_for_cylindrical_faces和select_cylindrical_face，用于选择圆柱面；定义了mask_for_dimensions和select_a_dimension，用于选择尺寸。
> 5. 实现创建尺寸公差特征功能：在do_it函数中，首先选择圆柱面，然后选择与公差关联的尺寸。接着，设置尺寸公差参数，包括名义值、公差上下限等，并调用UF_GDT_create_size_tolerance函数创建尺寸公差特征。
> 6. UFusr主函数：在UFusr函数中初始化NX，调用do_it函数，然后终止NX。
> 7. 卸载函数：定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码实现了在NX中通过二次开发接口选择面和尺寸，然后创建尺寸公差特征的功能。
>
