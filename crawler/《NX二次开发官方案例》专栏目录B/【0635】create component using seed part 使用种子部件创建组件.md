### 【0635】create component using seed part 使用种子部件创建组件

#### 代码

```cpp
    /*HEAD CREATE_COMPONENT_USING_SEED_PART CCC UFUN */  
    /*  This program demonstrates using a seed part when creating new  
        components.  To replace the standard UG action taken when the user  
        selects Assemblies->Components->Create New...  with this program, save  
        the menuscript file lines below to a file using a ".men" extension in a  
        "startup" subdirectory of a directory listed in either  
        $UGII_UG_CUSTOM_DIRECTORY_FILE or $UGII_CUSTOM_DIRECTORY_FILE.  E.g.  
        $UGII_USER_DIR/startup  
        Then compile this C code, and place the resulting program file in  
        an "application" subdirectory.  
        For more information on the "startup" and "application" subdirectories  
        or the UGII_CUSTOM_DIRECTORY_FILE environment variable, consult the  
        UG/Open Menuscript documentation.  
    ! Begin menufile  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    MODIFY  
        BUTTON UG_ASSY_INSERT_NEW_COMPONENT  
        ACTIONS create_component_using_seed_part  
    END_OF_MODIFY  
    ! End menufile  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我理解如下：

UF_print_syslog是V18版本新增的函数。这个函数用于打印系统日志。

关于UF_print_syslog函数的详细说明如下：

1. 它是一个新添加的函数，在V18版本中首次引入。

2. 该函数的功能是打印系统日志。

3. 在V18版本之前，并不存在UF_print_syslog这个函数。

4. 对于V18版本及以后版本，可以使用UF_print_syslog函数来打印系统日志。

5. UF_print_syslog函数的具体实现细节和参数等信息需要参考V18版本的官方文档。

综上所述，UF_print_syslog函数是V18版本新增的打印系统日志的函数，具体用法和参数需要参考V18版本的官方文档。 */  
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
    static logical is_file_there(char *file_spec)  
    {  
        int missing = TRUE;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        if (missing)  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_new_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        while (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            if (!is_file_there(fspec)) return TRUE;  
            uc1601("File exists - try again", TRUE);  
        }  
        return FALSE;  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n_objects;  
        tag_t  
            cpart,  
            instance,  
            *objects,  
            wpart = UF_ASSEM_ask_work_part();  
        double  
            abs[6] = { 1,0,0, 0,1,0 },  
            zero[3] = { 0,0,0 };  
        char  
            cspec[UF_CFI_MAX_PATH_NAME_SIZE] = { "" };  
        UF_PART_load_status_t  
            error_status;  
        UF_PART_export_options_t  
            opts = { FALSE, UF_PART_maintain_all_params,  
                        UF_PART_copy_exp_shallowly };  
        n_objects = select_objects("Select objects for new component", &objects);  
        if (!prompt_for_new_part_name("Create component", cspec)) return;  
        UF_CALL(UF_PART_open_quiet("parts/seed.prt", &cpart, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        if (error_status.failed) return;  
        UF_CALL(UF_PART_rename(cpart, cspec));  
        if (n_objects > 0)  
        {  
            UF_CALL(UF_PART_export_with_options(cspec, n_objects, objects, &opts));  
            UF_CALL(UF_OBJ_delete_array_of_objects(n_objects, objects, NULL));  
            UF_free(objects);  
        }  
        UF_CALL(UF_ASSEM_add_part_to_assembly(wpart, cspec, "", "", zero, abs,  
           -1, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，其主要功能是使用种子零件创建新的组件。具体功能包括：
>
> 1. 修改NX菜单，将“插入新组件”按钮的动作改为调用该代码。
> 2. 提示用户选择要用于创建组件的对象。
> 3. 提示用户输入新组件的名称。
> 4. 使用种子零件打开新组件，并重命名新组件。
> 5. 如果用户选择了对象，将这些对象导出到新组件中，并删除原始对象。
> 6. 将新组件添加到当前装配中。
> 7. 提供错误报告功能，在出现错误时显示错误信息。
> 8. 提供组件加载状态报告，在组件加载过程中显示加载信息。
> 9. 包含主函数和卸载函数。
>
> 这段代码展示了如何使用NX提供的API进行二次开发，实现组件的创建和对象操作，以及如何处理错误和状态报告。代码结构清晰，功能完整，是一个很好的NX二次开发示例。
>
