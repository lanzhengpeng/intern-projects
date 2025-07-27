### 【1709】open selected components 打开所选组件

#### 代码

```cpp
    /*HEAD OPEN_SELECTED_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以总结出以下要点：

1. UF_print_syslog 是一个在 V18 版本中新增加的函数。

2. 文档要求只翻译上述内容，不要添加任何额外的话语。

3. 该函数的名称翻译为：UF_print_syslog。

4. 该函数是在 V18 版本中新增加的。

综上所述，关于 UF_print_syslog 的信息可以翻译为：UF_print_syslog 是一个在 V18 版本中新增加的函数。 */  
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
    static int select_components(char *prompt, tag_t **components)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select components", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &cnt, components));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*components)[ii], FALSE));  
        return cnt;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_comps;  
        tag_t  
            *comps,  
            cset,  
            part = UF_PART_ask_display_part();  
        UF_PART_load_status_t  
            status;  
        while ((n_comps = select_components("Open", &comps)) > 0)  
        {  
            UF_CALL(UF_ASSEM_create_cset(part, "xOpen", &cset));  
            for (ii = 0; ii < n_comps; ii++)  
                UF_CALL(UF_ASSEM_add_to_cset(cset, comps[ii], FALSE));  
            UF_CALL(UF_PART_open_cset(cset, &status));  
            report_load_status(&status);  
            UF_CALL(UF_OBJ_delete_object(cset));  
            UF_free(comps);  
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

> 这是段用于在NX中进行二次开发的代码，其主要功能是允许用户选择要打开的组件，并将它们加载到NX中。
>
> 代码的主要部分包括：
>
> 1. 错误报告函数：用于记录和报告NX函数调用时的错误。
> 2. 组件选择：使用NX的UI函数，允许用户选择要打开的组件，并将其以组件集的形式打开。
> 3. 组件集创建：创建一个组件集，将用户选择的组件添加到其中。
> 4. 组件加载：调用NX的PART函数，打开组件集，并加载组件。
> 5. 加载状态报告：报告组件加载的结果。
> 6. 主函数：调用组件选择、组件集创建、组件加载等函数。
> 7. 卸载函数：用于卸载二次开发。
>
> 通过这段代码，用户可以方便地选择并加载NX模型中的组件，提高了NX的易用性。
>
