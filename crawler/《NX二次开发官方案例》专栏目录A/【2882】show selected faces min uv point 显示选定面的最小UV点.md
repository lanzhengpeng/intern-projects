### 【2882】show selected faces min uv point 显示选定面的最小UV点

#### 代码

```cpp
    /*HEAD SHOW_SELECTED_FACES_MIN_UV_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_evalsf.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的，只回答译文，不要废话。 */  
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
    /* qq3123197280 */  
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
    static void ask_face_min_uv_point(tag_t face, double min_point[3])  
    {  
        double  
            minmaxs[4],  
            minuv[2];  
        UF_EVALSF_p_t  
            eval;  
        UF_MODL_SRF_VALUE_t  
            surf_eval;  
        UF_EVALSF_pos3_t  
            srf_pos;  
        UF_CALL(UF_EVALSF_initialize(face, &eval));  
        UF_CALL(UF_EVALSF_ask_face_uv_minmax(eval, minmaxs));  
        minuv[0] = minmaxs[0];  
        minuv[1] = minmaxs[2];  
        UF_CALL(UF_EVALSF_evaluate(eval, UF_MODL_EVAL, minuv, &surf_eval));  
        UF_CALL(UF_EVALSF_ask_minimum_face_dist(eval, surf_eval.srf_pos,  
            &srf_pos));  
        UF_VEC3_copy(srf_pos.pnt3, min_point);  
        UF_CALL(UF_EVALSF_free(&eval));  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            min_uv[3];  
        while ((face = select_a_face("Show minimum UV point")) != NULL_TAG)  
        {  
            ask_face_min_uv_point(face, min_uv);  
            WRITE3F(min_uv);  
            display_temporary_point(min_uv);  
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

> 这段代码是一个NX二次开发的示例，主要用于选择一个面，并显示该面上UV参数的最小点。
>
> 主要功能包括：
>
> 1. 使用UF_UI库定义选择器的选择掩码，限制选择为实体类型中的任意面。
> 2. 使用UF_UI库中的选择对话框，提示用户选择一个面，并获取选择的面的tag。
> 3. 调用UF_EVALSF库，初始化面评估器，并获取面的UV参数范围。
> 4. 计算面UV参数的最小点，并获取该点的三维坐标。
> 5. 在NX中显示该最小点。
> 6. 循环提示用户选择面，直到用户取消。
> 7. 初始化和终止NX的二次开发环境。
>
> 该代码主要演示了如何使用UF_EVALSF库评估面，以及如何与用户进行交互式选择。
>
