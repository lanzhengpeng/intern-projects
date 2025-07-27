### 【0696】create extrusion 创建拉伸特征

#### 代码

```cpp
    /*HEAD CREATE_EXTRUSION CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
            *feats,  
            *curves;  
        double  
            dir[3],  
            point[3];  
        while (((n_curves = select_curves("Extrude", &curves)) > 0) &&  
            specify_vector("Specify extrusion vector", dir, point))  
        {  
            UF_CALL(UF_MODL_create_extrusion(curves, n_curves, NULL, "0.0", limits,  
                offsets, point, FALSE, TRUE, dir, UF_NULLSIGN, &feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                UF_CALL(UF_MODL_ask_feat_body(feats[ii], &body));;  
                UF_CALL(UF_OBJ_set_name(body, "Extruded"));  
            }  
            UF_free(curves);  
            if (n_feats > 0) UF_free(feats);  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 包含了必要的NX Open头文件，定义了UF_CALL宏用于错误处理。
> 2. 定义了一个report_error函数，用于输出错误信息。
> 3. 定义了一个mask_for_curves函数，用于设置选择曲线时的过滤条件。
> 4. 定义了一个select_curves函数，用于选择曲线并获取曲线数组。
> 5. 定义了一个specify_vector函数，用于获取用户指定的方向向量和点。
> 6. 定义了一个do_it函数，用于实现主体逻辑：
>
> 1. 定义了ufusr函数，作为NX的回调函数，用于初始化、执行主体逻辑、终止。
> 2. 定义了ufusr_ask_unload函数，用于立即卸载用户函数。
>
> 总体来说，这段代码实现了在NX中通过用户选择曲线并指定方向，创建实体拉伸特征的功能。
>
