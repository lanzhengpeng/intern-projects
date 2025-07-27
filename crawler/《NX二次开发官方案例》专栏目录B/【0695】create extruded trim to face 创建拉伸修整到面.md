### 【0695】create extruded trim to face 创建拉伸修整到面

#### 代码

```cpp
    /*HEAD CREATE_EXTRUDED_TRIM_TO_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是在V18版本中新增的。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
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
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_zc_dir(double zc_dir[3])  
    {  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        double  
            origin[3],  
            wcs[9];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, wcs));  
        UF_VEC3_copy(&wcs[6], zc_dir);  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            n_objects;  
        tag_t  
            *curves,  
            face,  
            *objects;  
        double  
            dir[3],  
            junk[3] = { 0,0,0 };  
        char  
            *offsets[2] = { "0", "0" };  
        UF_MODL_SWEEP_TRIM_object_t  
            trim = { NULL, 1, UF_MODL_SWEEP_TRIM_TO_FACE, NULL, 0 };  
        trim.trim_objects = &face;  
        if (((n_curves = select_curves("Select section curves", &curves)) > 0) &&  
            ((face = select_a_face("Select trimming face")) != NULL_TAG))  
        {  
            ask_zc_dir(dir);  
            UF_CALL(UF_MODL_create_extrude_trim_opts(curves, n_curves, &trim,  
                DO_NOT_EXTEND_TRIM_FACE, "0", NULL, offsets, junk, FALSE, TRUE,  
                dir, UF_NEGATIVE, &objects, &n_objects));  
            if (n_objects > 0) UF_free(objects);  
            UF_free(curves);  
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

> 这段代码是一个NX的二次开发程序，其主要功能是创建一个拉伸特征，并使用一个面进行修剪。以下是代码的主要步骤和功能：
>
> 1. 函数声明：代码首先声明了几个辅助函数，用于错误报告、选择曲线和面、设置选择掩码等。
> 2. 错误报告函数：report_error函数用于输出错误代码和调用信息，用于调试。
> 3. 选择曲线：select_curves函数用于让用户选择曲线，并设置选择掩码仅允许选择线、圆、圆锥曲线和样条线。
> 4. 选择面：select_a_face函数用于让用户选择一个面，并设置选择掩码仅允许选择实体面。
> 5. 获取ZC方向：ask_zc_dir函数用于获取当前工作坐标系的ZC轴方向。
> 6. 主体逻辑：do_it函数是程序的入口函数，用于执行创建拉伸特征并修剪的逻辑。它首先选择曲线和面，然后调用UF_MODL_create_extrude_trim_opts函数创建拉伸特征，并使用选中的面进行修剪。
> 7. UF初始化：ufusr函数是NX调用的入口函数，用于初始化和终止UF。
> 8. 卸载提示：ufusr_ask_unload函数用于提示立即卸载。
>
> 总体来说，这段代码实现了创建拉伸特征并使用一个面进行修剪的功能，是典型的NX二次开发代码。
>
