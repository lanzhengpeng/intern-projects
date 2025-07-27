### 【0767】create mirror body in another component 在另一个组件中创建镜像体

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

> 这段代码主要用于在NX中创建镜像体，并放置在另一个组件中。具体介绍如下：
>
> 1. 包含了必要的NX Open头文件，并定义了错误报告函数report_error()。
> 2. 定义了选择体的函数select_a_body()，设置了选择过滤器只选择实体。
> 3. 定义了选择基准平面的函数select_a_datum_plane()，设置了选择过滤器只选择基准平面。
> 4. 定义了选择组件的函数select_a_component()，设置了选择过滤器只选择组件。
> 5. 定义了主体函数do_it()，首先通过select_a_body()、select_a_datum_plane()和select_a_component()选择体、基准平面和组件。然后获取体的原型和基准平面的原型，创建体和基准平面的装配上下文，最后使用UF_WAVE_create_linked_mirror()函数创建镜像特征。
> 6. ufusr()是NX调用的主函数，初始化后调用do_it()，完成后终止。
> 7. ufusr_ask_unload()用于卸载用户函数。
>
> 综上，这段代码实现了在NX中创建镜像体并放置在另一个组件的功能。
>
