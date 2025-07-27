### 【0930】create subtracted extrusion 创建减法拉伸特征

#### 代码

```cpp
    /*HEAD CREATE_SUBTRACTED_EXTRUSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void set_feature_face_color(tag_t feat, int color)  
    {  
        uf_list_p_t  
            face_list,  
            temp;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        for (temp = face_list; temp != NULL; temp = temp->next)  
            UF_CALL(UF_OBJ_set_color(temp->eid, color));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            n_feats;  
        char  
            *limits[2] = { "0", "1" },  
            *offsets[2] = { "0", "0" };  
        tag_t  
            body,  
            *curves,  
            *feats;  
        double  
            dir[3],  
            point[3];  
        UF_OBJ_disp_props_t  
            disp_props;  
        while (((n_curves = select_curves("Extrude", &curves)) > 0) &&  
            specify_vector("Specify extrusion vector", dir, point))  
        {  
            UF_CALL(UF_MODL_create_extrusion2(curves, n_curves, NULL, "0.0",  
                limits, offsets, point, FALSE, TRUE, dir, UF_NEGATIVE,  
                &feats, &n_feats));  
        /*  In V18, the new faces are default - match them to body 里海译:在V18中，新面孔是默认的 - 将它们与身体匹配 */  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                UF_CALL(UF_MODL_ask_feat_body(feats[ii], &body));;  
                UF_CALL(UF_OBJ_ask_display_properties(body, &disp_props));  
                set_feature_face_color(feats[ii], disp_props.color);  
            }  
            UF_free(curves);  
            if (n_feats > 0) UF_free(feats);  
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

> 这段代码是一个NX Open C++的二次开发示例，用于创建减法拉伸特征。以下是代码的主要功能：
>
> 1. 包含了NX Open C++的常用头文件，用于调用NX的API函数。
> 2. 定义了错误报告函数report_error，用于在API调用失败时打印错误信息。
> 3. 定义了选择曲线的函数select_curves，用于提示用户选择要拉伸的曲线。
> 4. 定义了指定向量的函数specify_vector，用于提示用户指定拉伸方向和原点。
> 5. 定义了设置特征面颜色的函数set_feature_face_color，用于将新创建的特征面的颜色设置成父体的颜色。
> 6. 定义了主函数do_it，用于不断循环提示用户选择曲线，并创建减法拉伸特征。
> 7. 定义了ufusr函数，用于初始化和终止NX环境，并调用主函数。
> 8. 定义了卸载函数ufusr_ask_unload，用于在卸载时立即清理环境。
>
> 总体来说，这段代码通过NX Open C++ API实现了选择曲线、指定方向、创建减法拉伸特征等功能，并处理了错误和颜色设置等细节，是一个典型的NX二次开发示例。
>
