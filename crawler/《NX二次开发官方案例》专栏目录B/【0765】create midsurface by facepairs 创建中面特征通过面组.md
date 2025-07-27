### 【0765】create midsurface by facepairs 创建中面特征通过面组

#### 代码

```cpp
    /*HEAD CREATE_MIDSURFACE_BY_FACEPAIRS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 版本新增的功能。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            body,  
            faces[2],  
            facepair,  
            midsrf;  
        uf_list_p_t  
            side_1,  
            side_2;  
        while (((faces[0] = select_a_face("Select face 1")) != NULL_TAG) &&  
               ((faces[1] = select_a_face("Select face 2")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_ask_face_body(faces[0], &body));  
            UF_CALL(UF_MODL_create_midsrf_feature(body, &midsrf));  
            UF_CALL(UF_MODL_ask_adjac_faces(faces[0], &side_1));  
            UF_CALL(UF_MODL_ask_adjac_faces(faces[1], &side_2));  
            UF_CALL(UF_MODL_cre_def_facepair_feat(midsrf, faces[0], faces[1],  
                side_1, side_2, &facepair));  
            UF_CALL(UF_MODL_delete_list(&side_1));  
            UF_CALL(UF_MODL_delete_list(&side_2));  
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

> 这段代码是NX的二次开发代码，主要用于创建中面特征。以下是代码的简要介绍：
>
> 1. 功能介绍：该代码实现了通过选择两个面来创建中面特征的功能。用户首先需要选择两个面，然后代码会自动创建一个中面特征。
> 2. 关键函数：select_a_face：用于选择一个面，并返回其tag。UF_MODL_create_midsrf_feature：创建一个中面特征。UF_MODL_ask_adjac_faces：获取面的相邻面列表。UF_MODL_cre_def_facepair_feat：基于两个面和它们的相邻面列表创建默认的面对面特征。UF_MODL_delete_list：删除面列表。
> 3. select_a_face：用于选择一个面，并返回其tag。
> 4. UF_MODL_create_midsrf_feature：创建一个中面特征。
> 5. UF_MODL_ask_adjac_faces：获取面的相邻面列表。
> 6. UF_MODL_cre_def_facepair_feat：基于两个面和它们的相邻面列表创建默认的面对面特征。
> 7. UF_MODL_delete_list：删除面列表。
> 8. 错误处理：代码使用宏UF_CALL来封装NX API调用，并通过report_error函数来打印错误信息。
> 9. 交互式选择：代码使用NX的交互式选择功能，提示用户选择面，并设置选择掩码以限制选择范围。
> 10. 初始化和终止：在ufusr函数中初始化NX环境，调用do_it函数实现功能，并在结束时终止NX环境。
> 11. 卸载函数：ufusr_ask_unload函数允许立即卸载该代码。
>
> 综上所述，这段代码实现了选择两个面并创建默认中面特征的功能，通过交互式选择和NX API调用实现了二次开发。
>
