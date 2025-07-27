### 【2858】show all ref sets 显示所有参考集

#### 代码

```cpp
    /*HEAD SHOW_ALL_REF_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的内容，UF_print_syslog是在V18版本中新增的。因此，我的回答是：

UF_print_syslog是V18版本中新增的。 */  
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
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
    #ifdef UF_reference_design_model_subtype  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
    #else  
            if (subtype == UF_reference_design_subtype) return refset;  
    #endif  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp[1],  
            part,  
            refset = NULL_TAG;  
        char  
            fspec[MAX_FSPEC_SIZE+1],  
            name[UF_CFI_MAX_FILE_NAME_SIZE],  
            orig_name[MAX_ENTITY_NAME_SIZE+1];  
        double  
            origin[3],  
            matrix[9],  
            transform[4][4];  
        while ((comp[0] = select_a_component("Select a component")) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(comp[0], fspec, orig_name, name,  
                origin, matrix, transform));  
            part = UF_ASSEM_ask_prototype_of_occ(comp[0]);  
            while ((refset = ask_next_ref_set(part, refset)) != NULL_TAG)  
            {  
                UF_CALL(UF_OBJ_ask_name(refset, name));  
                UF_CALL(UF_ASSEM_replace_refset(1, comp, name));  
                uc1601(name, TRUE);  
            }  
            UF_CALL(UF_ASSEM_replace_refset(1, comp, NULL));  
            uc1601("Entire Part", TRUE);  
            if (strcmp(orig_name, "None"))  
                UF_CALL(UF_ASSEM_replace_refset(1, comp, orig_name));  
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

> 这是段NX Open C++的二次开发代码，其主要功能是遍历选择的所有组件，并为每个组件替换其引用集。代码的关键步骤如下：
>
> 1. 引入头文件：引入了必要的NX Open C++头文件。
> 2. 错误报告函数：定义了report_error函数，用于在函数调用失败时打印错误信息。
> 3. 组件选择函数：定义了mask_for_components和select_a_component函数，用于在组件选择对话框中仅显示组件。
> 4. 遍历引用集：定义了ask_next_ref_set函数，用于遍历组件的父部件中的所有引用集，并返回下一个设计引用集。
> 5. 主要逻辑：do_it函数是主要逻辑，用于遍历选择的所有组件，并获取组件数据。然后遍历组件的父部件中的所有设计引用集，并替换组件的引用集。最后，如果组件原始引用集不为空，则恢复组件的原始引用集。
> 6. 主函数：ufusr函数是程序的入口，初始化NX Open C++环境，调用do_it函数执行主要逻辑，并在结束时终止NX Open C++环境。
> 7. 卸载函数：ufusr_ask_unload函数用于在用户退出时卸载应用程序。
>
> 总的来说，这段代码实现了为组件替换引用集的功能，适用于NX Open C++二次开发环境。
>
