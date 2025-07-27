### 【0768】create mirror body in another component 在另一组件中创建镜像体

#### 代码

```cpp
    /*HEAD CREATE_MIRROR_BODY_IN_ANOTHER_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_wave.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是一个新功能，从V18版本开始提供。它的作用是打印系统日志信息。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            body_xform = NULL_TAG,  
            comp,  
            datum_xform = NULL_TAG,  
            dplane,  
            linked_feature,  
            mirror_part;  
        if (((body = select_a_body("Mirror body")) != NULL_TAG) &&  
            ((dplane = select_a_datum_plane("Mirror thru")) != NULL_TAG) &&  
            ((comp = select_a_component("Mirror into component")) != NULL_TAG))  
        {  
            mirror_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            if (UF_ASSEM_is_occurrence(body))  
            {  
                UF_CALL(UF_SO_create_xform_assy_ctxt(mirror_part,  
                    UF_ASSEM_ask_part_occurrence(body), comp, &body_xform));  
                body = UF_ASSEM_ask_prototype_of_occ(body);  
            }  
            else  
            {  
                UF_CALL(UF_SO_create_xform_assy_ctxt(mirror_part,  
                    NULL_TAG, comp, &body_xform));  
            }  
            if (UF_ASSEM_is_occurrence(dplane))  
            {  
                UF_CALL(UF_SO_create_xform_assy_ctxt(mirror_part,  
                    UF_ASSEM_ask_part_occurrence(dplane), comp, &datum_xform));  
                dplane = UF_ASSEM_ask_prototype_of_occ(dplane);  
            }  
            else  
            {  
                UF_CALL(UF_SO_create_xform_assy_ctxt(mirror_part,  
                    NULL_TAG, comp, &datum_xform));  
            }  
            UF_CALL(UF_WAVE_create_linked_mirror(body, body_xform, dplane,  
                datum_xform, mirror_part, TRUE, &linked_feature));  
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

> 这段NX二次开发代码的主要功能是创建一个体的镜像并放置在另一个组件中。
>
> 具体步骤包括：
>
> 1. 使用UI函数选择一个体、一个基准面和一个组件。
> 2. 获取组件的原始零件。
> 3. 如果选择的是组件实例，则创建一个变换矩阵。
> 4. 使用波形函数UF_WAVE_create_linked_mirror创建镜像特征，并放置在另一个组件中。
> 5. 定义了一个错误报告函数report_error，用于打印错误信息。
> 6. 定义了几个选择函数，用于设置选择掩码并选择对象。
> 7. ufusr函数用于初始化和终止NX，并调用do_it函数执行主体逻辑。
> 8. ufusr_ask_unload函数用于卸载用户函数。
>
> 这段代码实现了选择一个体、一个基准面和一个组件，然后创建该体的镜像并放置在另一个组件中的功能，是一个典型的NX二次开发示例。
>
