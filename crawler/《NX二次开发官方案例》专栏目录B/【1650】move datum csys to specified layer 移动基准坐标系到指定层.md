### 【1650】move datum csys to specified layer 移动基准坐标系到指定层

#### 代码

```cpp
    /*HEAD MOVE_DATUM_CSYS_TO_SPECIFIED_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能，用于打印系统日志。因此，针对问题“UF_print_syslog is new in V18”，翻译为“UF_print_syslog是V18版本新增的”。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
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
    static logical is_feat_of_type(tag_t feat, char *type)  
    {  
        logical  
            is_it = TRUE;  
        char  
            *this_type;  
        UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
        if (strcmp(type, this_type)) is_it = FALSE;  
        UF_free(this_type);  
        return is_it;  
    }  
    static int ask_object_layer(tag_t object)  
    {  
        logical  
            is_displayable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable &&  
            !UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props)))  
                return disp_props.layer;  
        else  
            return -1;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            new_layer;  
        tag_t  
            csys,  
            datum_csys,  
            daxes[3],  
            dplanes[3],  
            origin;  
        while ((datum_csys = select_a_feature("Move to layer")) != NULL_TAG)  
        {  
            if (!is_feat_of_type(datum_csys, "DATUM_CSYS"))  
            {  
                uc1601("Please select a Datum Coordinate System", TRUE);  
                continue;  
            }  
            UF_MODL_ask_datum_csys_components( datum_csys, &csys,  
                &origin, daxes, dplanes);  
            new_layer = ask_object_layer( csys );  
            if (prompt_for_a_layer("Destination", "Layer", &new_layer))  
            {  
                UF_CALL(UF_OBJ_set_layer(csys, new_layer));  
                UF_CALL(UF_OBJ_set_layer(origin, new_layer));  
                for (ii = 0; ii < 3; ii++)  
                    UF_CALL(UF_OBJ_set_layer(daxes[ii], new_layer));  
                for (ii = 0; ii < 3; ii++)  
                    UF_CALL(UF_OBJ_set_layer(dplanes[ii], new_layer));  
            }  
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

> 这是段NX的二次开发代码，其主要功能是将选定的坐标系和相关的特征移动到指定的图层。代码主要包括以下几个部分：
>
> 1. 定义错误报告函数report_error，用于在出现UF函数调用错误时打印错误信息。
> 2. 定义选择特征的函数select_a_feature，用于提示用户选择一个特征。
> 3. 定义提示用户输入图层的函数prompt_for_a_layer。
> 4. 定义检查特征类型的函数is_feat_of_type。
> 5. 定义获取对象图层的函数ask_object_layer。
> 6. 定义主函数do_it，用于循环选择坐标系，并移动到指定图层。
> 7. 定义UF函数ufusr，初始化和调用主函数。
> 8. 定义卸载函数ufusr_ask_unload。
>
> 整体上，该代码通过UF函数实现了坐标系和相关特征移动到指定图层的功能，具有较好的模块化和可读性。
>
