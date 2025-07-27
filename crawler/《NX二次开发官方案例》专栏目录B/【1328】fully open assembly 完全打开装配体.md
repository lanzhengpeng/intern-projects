### 【1328】fully open assembly 完全打开装配体

#### 代码

```cpp
    /*HEAD FULLY_OPEN_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static logical add_to_cset(tag_t comp, void *user_data)  
    {  
        tag_t  
            *cset = (tag_t *)user_data;  
        UF_CALL(UF_ASSEM_add_to_cset(*cset, comp, FALSE));  
        return TRUE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                ECHO("  ");  
                ECHO(status->file_names[ii]);  
                ECHO(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO(msg);  
                ECHO("\n");  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void fully_load_assembly_tree(tag_t component)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components,  
            part;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        n_children = UF_ASSEM_ask_part_occ_children(component, &child_components);  
        if (n_children > 0)  
        {  
            for (ii = 0; ii < n_children; ii++)  
            {  
                UF_ASSEM_ask_part_name_of_child(  
                    UF_ASSEM_ask_inst_of_part_occ(child_components[ii]), partname);  
                if ((UF_PART_is_loaded(partname)) != 1)  
                {  
                    UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
                    report_load_status(&error_status);  
                }  
                fully_load_assembly_tree(child_components[ii]);  
            }  
            UF_free(child_components);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            all_cset,  
            tmp_cset,  
            part = UF_PART_ask_display_part();  
        UF_PART_load_status_t  
            status;  
        UF_ASSEM_options_t  
            assem_options_orig,  
            assem_options_temp;  
        UF_CALL(UF_ASSEM_ask_assem_options(&assem_options_orig));  
        if (assem_options_orig.load_fully != UF_ASSEM_load_components_fully)  
        {  
            UF_CALL(UF_ASSEM_ask_assem_options(&assem_options_temp));  
            assem_options_temp.load_fully = UF_ASSEM_load_components_fully;  
            UF_CALL(UF_ASSEM_set_assem_options(&assem_options_temp));  
        }  
    /*  
        If you pass UF_PART_open_cset the "all comp" cset, the components will not  
        unblank properly, so create and use a new cset instead - see PRs 4529272  
        and 5372470.  
    */  
        UF_CALL(UF_ASSEM_create_cset(part, "TempCSet", &tmp_cset));  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &all_cset));  
        UF_CALL(UF_ASSEM_apply_to_cset(all_cset, add_to_cset, &tmp_cset));  
        UF_CALL(UF_PART_open_cset(tmp_cset, &status));  
        report_load_status(&status);  
        UF_CALL(UF_OBJ_delete_object(tmp_cset));  
        if (assem_options_orig.load_fully != UF_ASSEM_load_components_fully)  
            UF_CALL(UF_ASSEM_set_assem_options(&assem_options_orig));  
    /*  Any components which were partially loaded before running this  
        application still need to be fully loaded, so do it now.  
    */  
        fully_load_assembly_tree(UF_ASSEM_ask_root_part_occ(part));  
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

> 这段NX二次开发代码的主要功能是确保NX装配体中的所有组件都被完全加载。
>
> 主要步骤包括：
>
> 1. 设置NX装配体加载选项：将NX装配体的加载选项设置为"完全加载组件"，以确保所有组件都会被加载。
> 2. 创建临时组件集合：避免直接使用NX自带的"所有组件"集合，因为直接使用该集合可能导致组件无法正确显示。
> 3. 将所有组件添加到临时集合：通过递归遍历组件树，将所有组件添加到临时集合中。
> 4. 打开临时集合中的组件：通过打开临时集合中的所有组件，实现所有组件的完全加载。
> 5. 恢复NX装配体加载选项：将NX装配体的加载选项恢复到原始设置。
> 6. 完全加载已部分加载的组件：递归遍历装配体根组件，确保所有已部分加载的组件都被完全加载。
> 7. 清理临时组件集合：删除临时组件集合。
>
> 通过上述步骤，该代码可以确保NX装配体中的所有组件都被完全加载，从而避免由于组件未加载而导致的显示错误等问题。
>
