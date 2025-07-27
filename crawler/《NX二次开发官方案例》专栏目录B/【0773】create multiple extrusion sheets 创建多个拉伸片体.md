### 【0773】create multiple extrusion sheets 创建多个拉伸片体

#### 代码

```cpp
    /*HEAD CREATE_MULTIPLE_EXTRUSION_SHEETS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，翻译为：

注意：UF_print_syslog是V18版本新增的。 */  
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
            *curves,  
            *feats;  
        double  
            dir[3],  
            point[3];  
        while (((n_curves = select_curves("Extrude", &curves)) > 0) &&  
            specify_vector("Specify extrusion vector", dir, point))  
        {  
            UF_CALL(UF_MODL_create_extrusion2(curves, n_curves, NULL, "0.0",  
                limits, offsets, point, FALSE, FALSE, dir, UF_NULLSIGN,  
                &feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
                set_feature_face_color(feats[ii], ii+1);  
            if (n_feats > 0) UF_free(feats);  
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

> 这段代码是一个NX Open C++二次开发示例，其主要功能是创建多个拉伸特征。
>
> 主要流程包括：
>
> 1. 定义错误处理函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义曲线选择掩码函数mask_for_curves，用于在曲线选择对话框中仅允许选择线、圆、圆锥曲线和样条曲线。
> 3. 定义曲线选择函数select_curves，用于选择一组曲线，并取消高亮。
> 4. 定义方向向量指定函数specify_vector，用于让用户指定一个方向向量。
> 5. 定义特征面颜色设置函数set_feature_face_color，用于设置一个特征的所有面的颜色。
> 6. 定义主函数do_it，在循环中执行以下操作：调用select_curves选择曲线调用specify_vector指定拉伸方向调用UF_MODL_create_extrusion2创建拉伸特征调用set_feature_face_color设置特征颜色
> 7. 调用select_curves选择曲线
> 8. 调用specify_vector指定拉伸方向
> 9. 调用UF_MODL_create_extrusion2创建拉伸特征
> 10. 调用set_feature_face_color设置特征颜色
> 11. 定义ufusr函数，在NX启动时调用do_it。
> 12. 定义ufusr_ask_unload函数，在NX关闭时卸载二次开发代码。
>
> 通过这段代码，用户可以便捷地选择曲线并指定方向，创建多个拉伸特征，并设置特征颜色，简化了拉伸操作的流程。
>
