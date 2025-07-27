### 【1411】import a udf to a dplane v14 在指定平面上导入UDF版本14

#### 代码

```cpp
    /*HEAD IMPORT_A_UDF_TO_A_DPLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_true_or_false(char *prompt, logical deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "False", "True" };  
        deflt = deflt + 1;  
        resp = uc1603(prompt, deflt, options, 2);  
        if (resp > 4)  
        {  
            resp = resp - 5;  
            return (logical)resp;  
        }  
        else  
            return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            flip = FALSE;  
        int  
            num_values,  
            mark;  
        tag_t  
            body,  
            dplane,  
            feat,  
            meta_id,  
            tool_face,  
            udf;  
        double  
            csys[9],  
            loc[3],  
            z[3];  
        char  
            *angle,  
            *offset,  
            **udf_prompt,  
            **udf_values,  
            *udf_name;  
        while (((body = select_a_solid("Select target body")) != NULL_TAG) &&  
               ((dplane = select_a_datum_plane("Select datum plane")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_ask_object_feat(dplane, &feat));  
            UF_CALL(UF_MODL_ask_datum_plane_parms(feat,loc,z,&offset,&angle));  
            UF_CALL(UF_MTX3_initialize_z(z, csys));  
            UF_free(angle);  
            UF_free(offset);  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, loc, z, 0);  
            flip = ask_true_or_false("Flip", flip);  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
            if (!UF_CALL(UF_MODL_import_udf("/pr/v14/756467-subtract.udf",  
                csys, loc, &udf)))  
            {  
                UF_CALL(UF_MODL_ask_udf_parms(udf, &udf_name, &udf_prompt,  
                    &udf_values, &num_values));  
                UF_CALL(UF_MODL_set_udf_parms(udf, udf_prompt, udf_values,  
                    num_values));  
                UF_free_string_array(num_values,udf_values);  
                UF_free_string_array(num_values,udf_prompt);  
                UF_free(udf_name);  
                tool_face = NULL_TAG;  
                UF_CALL(UF_OBJ_cycle_by_name("TOOL_FACE", &tool_face));  
                if (UF_CALL(UF_MODL_boolean_udf_1(udf, dplane, tool_face, csys,  
                        0, NULL, 0, NULL, FALSE, body, flip, &meta_id)))  
                    UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                else  
                    UF_CALL(UF_MODL_update());  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发用户自定义特征(UDF)创建和布尔操作的示例。主要功能如下：
>
> 1. 错误报告函数：定义了report_error函数，用于在出错时输出错误信息。
> 2. 选择过滤函数：定义了mask_for_datum_planes和mask_body_type，用于在选择时过滤出特定类型的对象。
> 3. 选择对话框：定义了select_a_datum_plane和select_a_solid，用于弹出一个对话框让用户选择一个基准面和一个实体。
> 4. 布尔函数：定义了ask_true_or_false，用于弹出一个对话框让用户选择布尔操作的翻转参数。
> 5. UDF导入和操作：在do_it函数中，首先让用户选择一个基准面和一个实体。然后从外部文件导入一个UDF，并设置其参数。接着选择布尔操作的面，并调用UF_MODL_boolean_udf_1函数进行布尔操作，最后更新模型。
> 6. 主函数和卸载函数：在ufusr函数中，初始化NX环境，调用do_it函数执行主体功能，然后终止NX环境。在ufusr_ask_unload函数中，设置卸载方式为立即卸载。
>
> 总体来看，这段代码实现了导入UDF并进行布尔操作的功能，具有一定的参考价值。
>
