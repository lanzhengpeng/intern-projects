### 【2033】report components mated to selected component 报告所选组件的配合组件

#### 代码

```cpp
    /*HEAD REPORT_COMPONENTS_MATED_TO_SELECTED_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能，仅用于打印系统日志信息。该函数可以打印系统日志消息，包括时间戳、日志级别和日志信息。在V18版本中，UF_print_syslog用于打印系统日志信息。 */  
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
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, partname);  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_mated;  
        tag_t  
            component,  
            owning,  
            *mated;  
        char  
            compname[MAX_FSPEC_SIZE+1],  
            partname[MAX_FSPEC_SIZE+1];  
        while ((component = select_a_component("Select component")) != NULL_TAG)  
        {  
            ask_component_shortname(component, compname);  
            UF_CALL(UF_OBJ_ask_owning_part(component, &owning));  
            ask_part_shortname(owning, partname);  
            printf("\nSelected %d (%s) in %d (%s) \n", component, compname,  
                owning, partname);  
            UF_CALL(UF_ASSEM_ask_comps_mated_to_me(component, &mated, &n_mated));  
            ECHO(n_mated);  
            for (ii = 0; ii < n_mated; ii++)  
            {  
                ask_component_shortname(mated[ii], compname);  
                UF_CALL(UF_OBJ_ask_owning_part(mated[ii], &owning));  
                ask_part_shortname(owning, partname);  
                printf("  %d.  %d (%s) in %d (%s)\n", ii+1, mated[ii], compname,  
                    owning, partname);  
            }  
            if (n_mated > 0) UF_free(mated);  
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

> 这是NX二次开发中的用户函数示例代码，其主要功能是选择一个组件，然后列出与其配合的组件。
>
> 具体介绍如下：
>
> 1. 包含了必要的NX API头文件，如uf.h、uf_ui.h、uf_assem.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数执行失败时打印错误信息。
> 3. 定义了一个选择组件的函数select_a_component，用于通过对话框选择一个组件。
> 4. 定义了获取组件简称的函数ask_component_shortname，用于获取组件的简称。
> 5. 定义了获取部件简称的函数ask_part_shortname，用于获取组件所属部件的简称。
> 6. 定义了主函数do_it，用于选择一个组件，打印组件信息，获取与组件配合的组件列表，并打印配合组件信息。
> 7. 定义了ufusr函数，用于初始化NX API，调用主函数do_it，然后终止NX API。
> 8. 定义了ufusr_ask_unload函数，用于卸载用户函数时立即返回。
>
> 通过该代码，可以学习到NX二次开发的基本流程，以及如何使用NX的组件配合关系API。
>
