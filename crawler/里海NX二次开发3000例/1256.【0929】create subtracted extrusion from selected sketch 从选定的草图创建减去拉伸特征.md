### 【0929】create subtracted extrusion from selected sketch 从选定的草图创建减去拉伸特征

#### 代码

```cpp
    /*HEAD CREATE_SUBTRACTED_EXTRUSION_FROM_SELECTED_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_sket.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            n_feats;  
        char  
            *limits[2] = { "0", "50" },  
            *offsets[2] = { "0", "0" };  
        tag_t  
            body = NULL_TAG,  
            sketch = NULL_TAG,  
            *curves,  
            *feats;  
        double  
            dir[3],  
            point[3];  
        UF_OBJ_disp_props_t  
            disp_props;  
        while (((sketch = select_a_sketch("Sketch?")) != NULL_TAG) &&  
            specify_vector("Specify extrusion vector", dir, point))  
        {  
            UF_CALL(UF_SKET_ask_geoms_of_sketch(sketch, &n_curves, &curves));  
            UF_CALL(UF_MODL_create_extrusion2(curves, n_curves, NULL, "0.0",  
                limits, offsets, point, FALSE, TRUE, dir, UF_NEGATIVE,  
                &feats, &n_feats));  
        /*  In V18, the new faces are default - match them to body 里海译:在V18版本中，新面孔默认匹配身体。 */  
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

> 这段NX二次开发代码的主要功能是从选择的草图创建减去型腔的拉伸特征，具体功能如下：
>
> 1. 选择草图：使用UF_UI_select_sketch函数让用户选择一个草图。
> 2. 指定拉伸方向：使用自定义的specify_vector函数让用户指定拉伸的方向和原点。
> 3. 创建拉伸特征：使用UF_MODL_create_extrusion2函数从草图曲线创建拉伸特征，并设置拉伸方向为负值，即减去型腔。
> 4. 设置拉伸特征颜色：使用自定义的set_feature_face_color函数将新创建的拉伸特征颜色设置为所选草图的父体颜色。
> 5. 重复操作：在循环中重复上述操作，直到用户不再选择草图或不再指定拉伸方向。
> 6. 错误处理：使用UF_CALL宏和report_error函数进行错误处理。
> 7. 初始化和终止：在ufusr函数中初始化和终止NX Open API。
> 8. 卸载请求：通过ufusr_ask_unload函数请求立即卸载该二次开发代码。
>
> 总的来说，这段代码实现了从选择的草图创建减去型腔的拉伸特征，并且能够重复进行操作，同时包含错误处理和颜色设置功能。
>
