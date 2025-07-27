### 【2032】report component selected label or id symbol is attached to 报告选定标签或ID符号所附着的组件

#### 代码

```cpp
    /*HEAD REPORT_COMPONENT_SELECTED_LABEL_OR_ID_SYMBOL_IS_ATTACHED_TO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，翻译如下：

注意：UF_print_syslog 在 V18 中新增。 */  
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
    static int mask_for_labels_and_id_symbols(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { {UF_drafting_entity_type, UF_draft_label_subtype, 0},  
                        {UF_drafting_entity_type, UF_draft_id_symbol_subtype, 0} };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_label_or_id_symbol(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a label/ID symbol", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_labels_and_id_symbols, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_component_of_drafting_curve(tag_t curve)  
    {  
        int  
            gtype,  
            n_parents,  
            subtype;  
        tag_t  
            comp = NULL_TAG,  
            group,  
            *parents;  
        if (UF_ASSEM_is_occurrence(curve))  
            comp = UF_ASSEM_ask_part_occurrence(curve);  
        else  
        {  
            UF_CALL(UF_DRAW_ask_group_of_curve(curve, &group, &gtype, &subtype));  
            if ((group != NULL_TAG) && ((gtype == UF_solid_section_type) ||  
                    gtype == UF_solid_silhouette_type))  
            {  
                UF_CALL(UF_SO_ask_parents(curve, UF_SO_ASK_ALL_PARENTS, &n_parents,  
                    &parents));  
                if (n_parents > 0)  
                {  
                    if (UF_ASSEM_is_occurrence(parents[0]))  
                        comp = UF_ASSEM_ask_part_occurrence(parents[0]);  
                    UF_free(parents);  
                }  
            }  
        }  
        return comp;  
    }  
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_assoc;  
        tag_t  
            comp,  
            annot;  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        char  
            component[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        while ((annot = select_a_label_or_id_symbol("Report associated component"))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_associativity_data(annot, &n_assoc, &assoc_data));  
            for (ii = 0; ii < n_assoc; ii++)  
            {  
                UF_CALL(UF_DISP_set_highlight(assoc_data[ii].assoc_object_1,  
                    TRUE));  
                if ((comp = ask_component_of_drafting_curve(  
                    assoc_data[ii].assoc_object_1)) != NULL_TAG)  
                {  
                    ask_component_shortname(comp, component);  
                    sprintf(msg, "attached to: %s", component);  
                }  
                else  
                    strcpy(msg, "not attached to a component");  
                uc1601(msg, TRUE);  
                UF_CALL(UF_DISP_set_highlight(assoc_data[ii].assoc_object_1,  
                    FALSE));  
            }  
            if (n_assoc > 0) UF_free(assoc_data);  
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

> 根据代码内容，这是用于NX的二次开发代码，主要功能是查询选中标签或ID符号所关联的组件，并显示组件的简称。具体介绍如下：
>
> 1. 定义了UF_CALL宏，用于调用NX API并捕获错误。
> 2. 定义了mask_for_labels_and_id_symbols函数，用于设置选择过滤，仅允许选择标签和ID符号。
> 3. 定义了select_a_label_or_id_symbol函数，用于弹出一个选择对话框，让用户选择一个标签或ID符号。
> 4. 定义了ask_component_of_drafting_curve函数，用于查询曲线所关联的组件。
> 5. 定义了ask_component_shortname函数，用于查询组件的简称。
> 6. 定义了do_it函数，主逻辑如下：
>
> 1. 定义了ufusr函数，作为NX的入口函数，调用do_it。
> 2. 定义了ufusr_ask_unload函数，用于卸载该二次开发。
>
> 总的来说，这是一个典型的NX二次开发示例，实现了标签/ID符号与组件的关联查询功能。
>
