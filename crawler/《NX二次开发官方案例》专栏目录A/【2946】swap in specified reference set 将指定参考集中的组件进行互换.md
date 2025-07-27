### 【2946】swap in specified reference set 将指定参考集中的组件进行互换

#### 代码

```cpp
    /*HEAD SWAP_IN_SPECIFIED_REFERENCE_SET CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。 */  
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
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &cnt, components));  
        objs = *components;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *targets;  
        char  
            new_name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (((n = select_components("Select components", &targets)) > 0) &&  
            prompt_for_text("Enter new reference set name", new_name))  
        {  
            UF_CALL(UF_ASSEM_replace_refset(n, targets, new_name));  
            UF_free(targets);  
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

> 这段代码是一个NX Open C++ API的示例，用于替换指定参考集中的组件。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个用于组件选择的掩码函数mask_for_components。
> 3. 定义了一个用于选择组件的函数select_components。
> 4. 定义了一个用于提示输入文本的函数prompt_for_text。
> 5. 定义了一个主函数do_it，用于选择组件、输入新参考集名称，并调用UF_ASSEM_replace_refset函数替换参考集中的组件。
> 6. 定义了ufusr函数，用于初始化NX Open C++ API、执行主函数do_it，以及终止NX Open C++ API。
> 7. 定义了一个卸载函数ufusr_ask_unload，用于立即卸载用户函数。
>
> 该代码通过选择组件、输入新参考集名称，然后替换参考集中的组件，实现了在NX中替换指定参考集的功能。
>
