### 【1703】open component as 将组件作为另一组件打开

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_ugmgr.h>  
    #include <uf_cfi.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
    }  
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        tag_t  
            instance;  
        if ((instance = UF_ASSEM_ask_inst_of_part_occ(component)) != NULL_TAG)  
            UF_CALL(UF_ASSEM_ask_part_name_of_child(instance, partname));  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp,  
            part;  
        char  
            close[MAX_FSPEC_SIZE+1],  
            open[MAX_FSPEC_SIZE+1] = { "" };  
        UF_PART_load_status_t  
            status;  
        while (((comp = select_a_component("Reopen Component As")) != NULL_TAG)  
            && prompt_for_existing_part_name("Reopen As", open))  
        {  
            ask_component_partname(comp, close);  
            part = UF_ASSEM_ask_prototype_of_occ(comp);  
            if (part != NULL_TAG) UF_CALL(UF_PART_close(part, 1, 1));  
            WRITENZ(UF_PART_open_component_as(comp, close, open, &part, &status));  
            report_load_status(&status);  
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

> 这段代码是用于NX的二次开发代码，主要功能是在NX中重新打开一个组件作为另一个部件。
>
> 主要步骤包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个用于组件选择的面具函数mask_for_components。
> 3. 定义了一个选择组件的函数select_a_component。
> 4. 定义了一个提示输入已存在部件名称的函数prompt_for_existing_part_name。
> 5. 定义了一个询问组件的部件名称的函数ask_component_partname。
> 6. 定义了一个报告加载状态的函数report_load_status。
> 7. 定义了一个将整数写入清单窗口的函数write_integer_to_listing_window。
> 8. 定义了一个执行重新打开组件的函数do_it，包括选择组件、提示输入新部件名称、关闭组件原型、重新打开组件、报告加载状态等步骤。
> 9. 定义了UFusr函数，用于初始化NX环境、执行重新打开组件、终止NX环境。
> 10. 定义了一个卸载询问函数ufusr_ask_unload，返回立即卸载。
>
> 综上，这段代码实现了在NX中重新打开一个组件作为另一个部件的功能，包含错误处理、组件选择、部件名称处理、加载状态报告等完整的流程。
>
