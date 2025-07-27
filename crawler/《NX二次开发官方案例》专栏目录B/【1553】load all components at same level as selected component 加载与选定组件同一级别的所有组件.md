### 【1553】load all components at same level as selected component 加载与选定组件同一级别的所有组件

#### 代码

```cpp
    /*  
    NOTE:  UF_ASSEM_ensure_child_loaded will obey the current load options.    
    If partial loading is turned on, the component and its siblings will only be partially loaded.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_sibs;  
        tag_t  
            comp,  
            parent,  
            instance,  
            *siblings;  
        UF_PART_load_status_t  
            load_status;  
        if ((comp = select_a_component("Load all siblings")) == NULL_TAG) return;  
        parent = UF_ASSEM_ask_part_occurrence(comp);  
        instance = UF_ASSEM_ask_inst_of_part_occ(parent);  
        UF_CALL(UF_ASSEM_ensure_child_loaded(instance, &load_status));  
        report_load_status(&load_status);  
        n_sibs = UF_ASSEM_ask_part_occ_children(parent, &siblings);  
        for (ii = 0; ii < n_sibs; ii++)  
        {  
            instance = UF_ASSEM_ask_inst_of_part_occ(siblings[ii]);  
            UF_CALL(UF_ASSEM_ensure_child_loaded(instance, &load_status));  
            report_load_status(&load_status);  
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

> 这段代码是一个NX Open C++程序，其主要功能是加载一个组件及其所有兄弟组件到内存中。
>
> 代码的主要逻辑如下：
>
> 1. 通过UF_CALL宏调用NX API函数，并检查函数返回码。如果返回码非零，则记录错误信息。
> 2. 定义了一个echo函数，用于在Listing窗口和系统日志中输出信息。
> 3. 定义了一个report_error函数，用于在出错时输出错误信息。
> 4. 定义了一个mask_for_components函数，用于设置选择组件时只选择组件类型。
> 5. 定义了一个select_a_component函数，用于让用户选择一个组件，并返回其tag。
> 6. 定义了一个report_load_status函数，用于输出组件的加载状态信息。
> 7. 定义了一个do_it函数，用于加载用户选择的组件及其所有兄弟组件。首先获取组件的父组件和实例，然后调用UF_ASSEM_ensure_child_loaded函数确保组件及其兄弟组件被加载，并输出加载状态。
> 8. ufusr函数是程序的入口点，初始化NX系统后调用do_it函数，然后终止NX系统。
> 9. ufusr_ask_unload函数用于设置程序卸载模式为立即卸载。
>
> 总的来说，这个程序通过交互方式选择一个组件，然后递归加载该组件及其所有兄弟组件，并输出加载状态信息。
>
