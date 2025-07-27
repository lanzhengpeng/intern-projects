### 【0862】create relative daxis thru pt perpendicular to face 创建垂直于面的通过点的相对方向轴

#### 代码

```cpp
    /*HEAD CREATE_RELATIVE_DAXIS_THRU_PT_PERPENDICULAR_TO_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_modl_datum_features.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是一个新功能，在 V18 版本中引入。

只回答译文：

UF_print_syslog 是一个新功能，在 V18 版本中引入。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static void create_dirr_daxis( tag_t face, tag_t point )  
    {  
        tag_t  
            u = NULL_TAG,  
            v = NULL_TAG,  
            direction = NULL_TAG,  
            smrt_pnt = NULL_TAG,  
            smrt_pnt_1 = NULL_TAG,  
            dirr_daxis_obj = NULL_TAG,  
            dirr_daxis_feat = NULL_TAG;  
        UF_SO_update_option_t   
            update_option =  UF_SO_update_within_modeling;   
        UF_CALL( UF_SO_create_point_extract( face, update_option, point,   
                                                 NULL_TAG, &smrt_pnt_1 ));  
        UF_CALL( UF_SO_set_visibility_option( smrt_pnt_1, UF_SO_invisible ));  
        UF_CALL(UF_SO_create_scalar_double( face, update_option, 0.5, &u ));  
        UF_CALL(UF_SO_create_scalar_double( face, update_option, 0.5, &v ));  
        UF_CALL( UF_SO_create_point_on_surface(face, update_option,   
                                               face, u, v, &smrt_pnt ));  
        UF_CALL( UF_SO_set_visibility_option( smrt_pnt, UF_SO_invisible ));  
        UF_CALL( UF_SO_create_dirr_normal_to_surface_point ( face, update_option,   
                                              face, smrt_pnt, FALSE, &direction ));  
        UF_CALL( UF_MODL_create_point_dirr_daxis( smrt_pnt_1, direction,   
                                                       &dirr_daxis_obj ));  
        UF_CALL( UF_SO_set_visibility_option( dirr_daxis_obj, UF_SO_visible ));  
        UF_CALL( UF_MODL_ask_object_feat( dirr_daxis_obj, &dirr_daxis_feat ));  
    }  
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static void do_it(void)  
    {  
        tag_t  
            face = NULL_TAG,  
            point = NULL_TAG;  
        while(( face = select_a_face( "Select face" )) != NULL_TAG &&  
              ( point = select_a_point("Select Existing Point:")) != NULL_TAG )  
        {  
            create_dirr_daxis( face, point );  
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

> 这段代码是基于NX Open的二次开发代码，其主要功能是创建一个通过面上的点的相对坐标轴。
>
> 关键要点如下：
>
> 1. 初始化：代码首先调用UF_initialize()进行NX Open的初始化。
> 2. 选择面和点：通过自定义的select_a_face()和select_a_point()函数，代码实现了选择一个面和一个点的功能。
> 3. 创建相对坐标轴：自定义的create_dirr_daxis()函数通过提取面上点的法向量和输入点创建一个相对坐标轴。
> 4. 重复选择：通过一个循环，代码允许用户重复选择面和点来创建多个相对坐标轴。
> 5. 结束：最后调用UF_terminate()来结束NX Open。
> 6. 错误报告：代码中包含一个错误报告函数report_error()，用于在出错时输出错误信息。
> 7. 选择过滤：自定义的mask_for_faces()和mask_for_points()函数用于在用户选择时进行过滤，分别限制选择面和点。
> 8. 隐藏临时对象：代码中创建的临时对象会被设置为不可见，避免影响视图。
> 9. 返回值：ufusr()函数返回void，并通过参数retcode返回错误码。
> 10. 卸载函数：ufusr_ask_unload()函数返回UF_UNLOAD_IMMEDIATELY，表示立即卸载。
>
> 综上所述，这段代码实现了在NX中创建通过面上的点的相对坐标轴的功能，并包含了错误处理和用户交互等逻辑。
>
