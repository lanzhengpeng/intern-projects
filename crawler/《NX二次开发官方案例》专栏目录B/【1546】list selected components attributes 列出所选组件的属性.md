### 【1546】list selected components attributes 列出所选组件的属性

#### 代码

```cpp
    /*HEAD LIST_SELECTED_COMPONENTS_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_attr.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_attributes(tag_t object)  
    {  
        int  
            indx = 0;  
        char  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && indx)  
        {  
            switch ( att.type )  
            {  
                case UF_ATTR_integer:  
                    sprintf(mstr, "%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    sprintf(mstr, "%s = %f (FLOATING POINT)\n", title,  
                        att.value.real);  
                break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(att.value.time, 2, dstr, tstr));  
                    sprintf(mstr, "%s = %s %s (DATE & TIME)\n", title, dstr, tstr);  
                break;  
                case UF_ATTR_null:  
                    sprintf(mstr, "%s = (NULL)\n", title);  
                break;  
                case UF_ATTR_string:  
                    sprintf(mstr, "%s = %s (STRING)\n", title, att.value.string);  
                    UF_free(att.value.string);  
                break;  
                case UF_ATTR_reference:  
                    sprintf(mstr, "%s = %s (REFERENCE)\n", title,  
                        att.value.reference);  
                    UF_free(att.value.reference);  
                break;  
                default:  
                break;  
            }  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n;  
        tag_t  
            comp,  
            inst;  
        uf_list_p_t  
            insts,  
            occs;  
        while ((comp = select_a_component("Select a component")) != NULL_TAG)  
        {  
            report_attributes(comp);  
            UF_CALL(UF_MODL_create_list(&insts));  
            UF_CALL(UF_MODL_create_list(&occs));  
            do  
            {  
                UF_CALL(UF_MODL_put_list_item(occs, comp));  
                if ((inst = UF_ASSEM_ask_inst_of_part_occ(comp)) != NULL_TAG)  
                    UF_CALL(UF_MODL_put_list_item(insts, inst));  
            } while ((comp = UF_ASSEM_ask_part_occurrence(comp)) != NULL_TAG);  
            UF_CALL(UF_DISP_set_display(FALSE));  
            UF_CALL(UF_MODL_ask_list_count(insts, &n));  
            for (ii = n-1; ii > 0; ii--)  
            {  
                UF_CALL(UF_MODL_ask_list_item(occs, ii, &comp));  
                UF_PART_set_display_part(UF_ASSEM_ask_prototype_of_occ(comp));  
                comp = NULL_TAG;  
                for (jj = ii-1; jj >= 0; jj--)  
                {  
                    UF_MODL_ask_list_item(insts, jj, &inst);  
                    comp = UF_ASSEM_ask_part_occ_of_inst(comp, inst);  
                }  
                report_attributes(comp);  
            }  
            UF_CALL(UF_DISP_set_display(TRUE));  
            UF_CALL(UF_MODL_ask_list_item(occs, n, &comp));  
            UF_PART_set_display_part(UF_ASSEM_ask_prototype_of_occ(comp));  
            UF_CALL(UF_MODL_delete_list(&insts));  
            UF_CALL(UF_MODL_delete_list(&occs));  
        }  
    }  
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

> 这段NX二次开发代码的主要功能是选择一个组件，并报告该组件及其父实例的属性。具体来说，它包括以下步骤：
>
> 1. 使用对话框让用户选择一个组件，并高亮显示选中的组件。
> 2. 获取选中组件的属性，包括整数、浮点数、时间、字符串、引用等类型，并输出到列表窗口。
> 3. 遍历选中组件及其所有父实例，重复步骤2，输出每个组件的属性。
> 4. 在遍历过程中，暂时关闭组件的显示，以避免混淆。
> 5. 遍历结束后，恢复组件的显示。
> 6. 使用模型列表来管理组件和实例，以便遍历。
> 7. 在函数 ufusr 中初始化和终止 NX，并调用主函数 do_it。
> 8. 提供卸载函数 ufusr_ask_unload。
>
> 总的来说，这段代码实现了选择组件、遍历组件及其父实例、获取并输出属性的功能。通过对话框选择、列表管理、属性输出等操作，实现了对NX组件属性的查询。
>
