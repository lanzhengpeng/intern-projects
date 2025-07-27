### 【0736】create iso curves at selected bsurfaces knots 在选定B曲面节上创建等参曲线

#### 代码

```cpp
    /*HEAD CREATE_ISO_CURVES_AT_SELECTED_BSURFACES_KNOTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog is new in V18，只回答译文，不要废话。 */  
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
    static int mask_for_bsurfs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PARAMETRIC_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_bsurf(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bsurfs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_isos;  
        tag_t  
            bsurf,  
            *isos;  
        double  
            distol,  
            uvs[4];  
        UF_MODL_bsurface_t  
            bsurf_data;  
        UF_MODL_ask_distance_tolerance(&distol);  
        while ((bsurf = select_a_bsurf("Select bsurf face")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_face_uv_minmax(bsurf, uvs));  
            UF_CALL(UF_MODL_ask_bsurf(bsurf, &bsurf_data));  
            for (ii = 0; ii < bsurf_data.num_poles_u + bsurf_data.order_u; ii++)  
                if ((bsurf_data.knots_u[ii] > uvs[0]) &&  
                    (bsurf_data.knots_u[ii] < uvs[1]) &&  
                    !UF_CALL(UF_MODL_create_isocurve(bsurf, 1,  
                        bsurf_data.knots_u[ii], distol, &isos, &n_isos)) &&  
                    (n_isos > 0)) UF_free(isos);  
            for (ii = 0; ii < bsurf_data.num_poles_v + bsurf_data.order_v; ii++)  
                if ((bsurf_data.knots_v[ii] > uvs[2]) &&  
                    (bsurf_data.knots_v[ii] < uvs[3]) &&  
                    !UF_CALL(UF_MODL_create_isocurve(bsurf, 2,  
                        bsurf_data.knots_v[ii], distol, &isos, &n_isos)) &&  
                    (n_isos > 0)) UF_free(isos);  
            UF_CALL(UF_MODL_free_bsurf_data(&bsurf_data));  
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

> 这段代码的主要功能是在NX中创建B样条曲面上指定结点处的等参数线。以下是代码的主要组成部分和功能：
>
> 1. 函数report_error：用于打印错误信息。
> 2. 函数mask_for_bsurfs：用于设置选择掩码，只允许选择参数化面。
> 3. 函数select_a_bsurf：用于让用户选择一个B样条面。
> 4. 函数do_it：获取B样条面的最小最大UV值。获取B样条面的数据。遍历U方向和V方向的结点，在合法范围内创建等参数线。释放B样条面数据。
> 5. 获取B样条面的最小最大UV值。
> 6. 获取B样条面的数据。
> 7. 遍历U方向和V方向的结点，在合法范围内创建等参数线。
> 8. 释放B样条面数据。
> 9. 主函数ufusr：初始化NX。调用do_it函数执行创建等参数线。终止NX。
> 10. 初始化NX。
> 11. 调用do_it函数执行创建等参数线。
> 12. 终止NX。
>
> 通过以上步骤，该代码允许用户选择B样条面，并在指定结点处创建等参数线，实现了在NX中创建B样条曲面上指定结点处的等参数线的功能。
>
