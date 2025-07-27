### 【1659】move selected features geometry to specified layer 将选定的特征几何移动到指定图层

#### 代码

```cpp
    /*HEAD MOVE_SELECTED_FEATURES_GEOMETRY_TO_SPECIFIED_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_layer.h>  
    #include <uf_wave.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_UI_feat_sel_type_t  
            all = UF_UI_FEAT_SEL_TYPE_BROWSEABLE;  
        UF_CALL(UF_UI_select_feature(prompt, &all, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    #ifndef UF_LAYER_MAX_LAYER  
        #define UF_LAYER_MAX_LAYER 256  
    #endif  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void set_layer_object_array(int n_objs, tag_t *objs, int layer)  
    {  
        logical  
            is_display;  
        int  
            ii;  
        for (ii = 0; ii < n_objs; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objs[ii], &is_display));  
            if (is_display)  
                UF_CALL(UF_OBJ_set_layer(objs[ii], layer));  
        }  
    }  
    static void set_feature_layer(tag_t feat, int layer)  
    {  
        int  
            n_objs;  
        tag_t  
            geom,  
            *objs;  
        if (!UF_WAVE_ask_linked_feature_geom(feat, &geom) && geom)  
            set_layer_object_array(1, &geom, layer);  
        else  
        {  
            if (!UF_CURVE_ask_feature_curves(feat, &n_objs, &objs) && n_objs)  
            {  
                set_layer_object_array(n_objs, objs, layer);  
                UF_free(objs);  
            }  
            else  
            {  
                if (!UF_MODL_ask_feat_object(feat, &n_objs, &objs) && n_objs)  
                {  
                    set_layer_object_array(n_objs, objs, layer);  
                    UF_free(objs);  
                }  
            }  
        }  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats,  
            layer = 1;  
        tag_t  
            *feats;  
        set_undo_mark("Set Features Layer");  
        while (((n_feats = select_features("Move to Layer", &feats)) > 0) &&  
            prompt_for_a_layer("Move to", "Layer", &layer))  
        {  
            for (ii = 0; ii < n_feats; ii++)  
                set_feature_layer(feats[ii], layer);  
            UF_free(feats);  
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

> 这段代码是一个NX Open C++应用程序，用于将选择的特征移动到指定的图层。
>
> 代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了一个选择特征函数select_features，用于让用户选择特征并返回特征数组。
> 3. 定义了一个提示输入图层函数prompt_for_a_layer，用于提示用户输入图层数并返回图层数。
> 4. 定义了一个设置图层函数set_layer_object_array，用于将对象设置到指定图层。
> 5. 定义了一个设置特征图层函数set_feature_layer，用于将特征的所有关联几何体设置到指定图层。
> 6. 定义了一个设置撤销标记函数set_undo_mark，用于设置撤销标记点。
> 7. 定义了一个执行函数do_it，用于循环提示用户选择特征和图层，然后移动特征到指定图层。
> 8. 定义了ufusr函数，用于初始化和执行功能，以及终止应用程序。
> 9. 定义了ufusr_ask_unload函数，用于卸载应用程序。
>
> 整体来说，这段代码实现了一个简单的NX Open C++应用程序，用户可以通过选择特征和输入图层号，将特征移动到指定的图层。
>
