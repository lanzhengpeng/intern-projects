### 【3041】use alternate component 使用备用组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
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
    static void do_it(void)  
    {  
        tag_t  
            comp,  
            inst;  
        double  
            instance_origin[3],  
            instance_matrix[9],  
            instance_trans[4][4];  
        char  
            filename[MAX_FSPEC_SIZE+1] = { "" },  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_PART_load_status_t  
            load_status;  
        while (((comp = select_a_component("Substitute out")) != NULL_TAG) &&  
            prompt_for_existing_part_name("Substitute in", filename))  
        {  
            inst = UF_ASSEM_ask_inst_of_part_occ(comp);  
            UF_CALL(UF_ASSEM_ask_component_data(inst, part_name, refset_name,  
                instance_name, instance_origin, instance_matrix, instance_trans));  
            UF_CALL(UF_ASSEM_use_alternate(&inst, filename, instance_name,  
                refset_name, &load_status));  
            report_load_status(&load_status);  
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

> 这段NX Open API代码用于在装配体中替换组件。主要功能包括：
>
> 1. 定义了ECHO函数，用于在列表窗口和系统日志中输出信息。
> 2. 定义了UF_CALL宏，用于执行NX Open API函数并报告错误。
> 3. 定义了mask_for_components函数，用于设置选择组件的类型掩码。
> 4. 定义了select_a_component函数，用于让用户选择一个组件。
> 5. 定义了report_load_status函数，用于报告组件加载状态。
> 6. 定义了prompt_for_existing_part_name函数，用于提示用户输入要替换的组件的文件名。
> 7. 定义了do_it函数，用于执行替换组件的操作：选择要替换的组件提示用户输入替换组件的文件名获取被替换组件的实例信息使用UF_ASSEM_use_alternate函数替换组件，并报告加载状态
> 8. 选择要替换的组件
> 9. 提示用户输入替换组件的文件名
> 10. 获取被替换组件的实例信息
> 11. 使用UF_ASSEM_use_alternate函数替换组件，并报告加载状态
> 12. 定义了ufusr函数，用于初始化和清理NX Open API，并调用do_it函数。
> 13. 定义了ufusr_ask_unload函数，用于在卸载时立即卸载。
>
> 总体来说，这段代码实现了一个简单的替换组件功能，通过选择组件、输入文件名，调用API完成替换操作。
>
