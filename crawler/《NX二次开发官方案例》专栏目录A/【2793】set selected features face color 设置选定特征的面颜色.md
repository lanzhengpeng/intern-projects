### 【2793】set selected features face color 设置选定特征的面颜色

#### 代码

```cpp
    /*HEAD SET_SELECTED_FEATURES_FACE_COLOR CCC UFUN */  
    /*  To create your own dialog for use with this program:  
        Application->UIStyler  
        change CHANGE to GTAC  
        set Button Style Options to Close  
        add a color tool  
        add a push button  
        set its activate callback: color_feature_faces_cb  
        File->Save As->color_features  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    #define GTAC_CTOL_0                    ("CTOL_0")  
    #define GTAC_ACTION_1                  ("ACTION_1")  
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, UF_UI_FEAT_SEL_TYPE_BROWSEABLE, &cnt,  
            feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
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
    /* qq3123197280 */  
    static int GTAC_color_feature_faces_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats;  
        double  
            clr_values[3];  
        char  
            *color_name,  
            prompt[133];  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_id = GTAC_CTOL_0;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        UF_CALL(UF_DISP_ask_color(ask_value.value.integer, UF_DISP_rgb_model,  
            &color_name, clr_values));  
        sprintf(prompt, "Change faces to color %s", color_name);  
        UF_free(color_name);  
        n_feats = select_features(prompt, &feats);  
        for (ii = 0; ii < n_feats; ii++)  
            set_feature_face_color(feats[ii], ask_value.value.integer);  
        if (n_feats > 0) UF_free(feats);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] = {  
                {GTAC_ACTION_1, UF_STYLER_ACTIVATE_CB, 1,  
                    GTAC_color_feature_faces_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("color_features.dlg", GTAC_cbs, 2, NULL,  
            &resp));  
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

> 这段NX二次开发代码的功能如下：
>
> 1. 定义了错误报告函数report_error，用于打印出调用UF函数失败的错误信息。
> 2. 定义了选择特征函数select_features，用于让用户选择要改变颜色的特征。
> 3. 定义了设置特征面颜色函数set_feature_face_color，用于设置传入的特征的所有面的颜色。
> 4. 定义了回调函数GTAC_color_feature_faces_cb，当用户选择颜色并点击确定时，会执行这个回调函数。回调函数会先获取用户选择的颜色，然后调用select_features让用户选择特征，再调用set_feature_face_color设置颜色。
> 5. 定义了创建对话框函数do_it，用于创建一个包含颜色选择和确定按钮的对话框，并设置回调函数。
> 6. 定义了ufusr函数，用于初始化NX环境，调用do_it创建对话框，然后终止NX环境。
> 7. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，这段代码实现了一个简单的NX二次开发程序，用于让用户选择颜色，然后选择特征，并将特征的所有面设置为用户选择的颜色。
>
