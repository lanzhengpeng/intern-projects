### 【2932】substitute component 替换组件

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
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static int prompt_for_an_integer(char *prompt, char *item, int number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            return da[0];  
        }  
        else return 0;  
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
        int  
            layer=1;  
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
            layer = prompt_for_an_integer( "Enter Layer", "Layer", layer );  
            UF_CALL(UF_ASSEM_ask_component_data(inst, part_name, refset_name,  
                instance_name, instance_origin, instance_matrix, instance_trans));  
            UF_CALL(UF_ASSEM_substitute_component(&inst, filename, instance_name,  
                refset_name, layer, &load_status));  
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

> 这段代码是一个NX Open C++应用程序，用于替换装配中的组件。其主要功能包括：
>
> 1. 组件选择：使用UF_UI_select_with_single_dialog函数，允许用户选择装配中的组件，并设置组件选择掩码只允许选择组件类型的对象。
> 2. 替换组件：对于用户选择的每个组件，程序会获取组件实例的相关信息，包括名称、参考集名称、位置、方向等，然后使用UF_ASSEM_substitute_component函数用用户指定的文件替换该组件。
> 3. 错误处理：使用宏UF_CALL和函数report_error来处理函数调用失败的情况，记录错误代码和位置，并显示错误消息。
> 4. 用户交互：使用函数prompt_for_an_integer和prompt_for_existing_part_name获取用户输入的层数和替换组件文件名。
> 5. 资源清理：在main函数 ufusr 中初始化和终止NX Open，在 ufusr_ask_unload 函数中处理卸载请求。
> 6. 日志输出：使用ECHO函数输出日志到列表窗口和系统日志。
>
> 总体来说，这段代码实现了在NX装配环境中替换组件的功能，通过用户交互获取必要信息，并进行错误处理。
>
