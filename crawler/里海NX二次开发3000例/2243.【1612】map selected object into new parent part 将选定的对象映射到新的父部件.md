### 【1612】map selected object into new parent part 将选定的对象映射到新的父部件

#### 代码

```cpp
    /*HEAD MAP_SELECTED_OBJECT_INTO_NEW_PARENT_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的函数。该函数用于打印系统日志。 */  
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
    static void build_copy_partname(char *addon, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, addon);  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
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
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            tceng;  
        int  
            units;  
        tag_t  
            original,  
            new_occ,  
            master_part = UF_PART_ask_display_part(),  
            spec_part,  
            instance;  
        double  
            origin[3] = {0, 0, 0},  
            matrix[6] = {1, 0, 0, 0, 1, 0};  
        char  
            master_encoded_name[MAX_FSPEC_SIZE+1],  
            spec_encoded_name[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        original = select_anything("Pick an object");  
        UF_CALL(UF_OBJ_set_name(original, "original"));  
        UF_PART_ask_part_name(master_part, master_encoded_name);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(master_encoded_name, part_number,  
                part_revision, part_file_type, part_file_name));  
            strcpy(part_file_type, "specification");  
            strcpy(part_file_name, "165-001-01");  
            UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
                part_file_type, part_file_name, spec_encoded_name));  
        }  
        else  
            build_copy_partname("_dwg", spec_encoded_name);  
        UF_CALL(UF_PART_ask_units(master_part, &units));  
        UF_CALL(UF_PART_new(spec_encoded_name, units, &spec_part));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(spec_part, master_encoded_name, NULL,  
            NULL, origin, matrix, 0, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        UF_CALL(UF_ASSEM_set_work_occurrence(UF_ASSEM_ask_part_occ_of_inst(  
            UF_ASSEM_ask_root_part_occ(spec_part), instance)));  
        UF_CALL(UF_ASSEM_get_occ_in_work_occ(original, &new_occ));  
        UF_CALL(UF_OBJ_set_name(new_occ, "new_occ"));  
        UF_CALL(UF_ASSEM_set_work_part(spec_part));  
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

> 这段代码是一个NX Open C++二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个构建复制部件名称的函数build_copy_partname，用于在当前打开部件名称的基础上添加后缀，生成新的部件名称。
> 3. 定义了一个报告加载状态的函数report_load_status，用于输出加载部件时的错误信息。
> 4. 定义了一个添加选择面和边的函数mask_add_faces_and_edges，用于在选择对话框中仅允许选择实体面和边。
> 5. 定义了一个选择任意对象的函数select_anything，用于打开选择对话框，让用户选择一个对象并返回其tag。
> 6. 定义了一个主函数do_it，主要流程如下：选择一个对象，并设置其名称为"original"获取当前打开部件的名称，并解析出部件编号、版本、文件类型等信息生成一个新的部件名称，文件类型为"specification"，文件名为"165-001-01"根据当前打开部件的长度单位，创建一个新部件将新部件添加到当前装配中，设置其在装配中的位置和方向如果加载部件时有错误，报告错误信息设置工作部件为新创建的部件获取原始选择对象在新部件中的实例，并设置其名称为"new_occ"
> 7. 选择一个对象，并设置其名称为"original"
> 8. 获取当前打开部件的名称，并解析出部件编号、版本、文件类型等信息
> 9. 生成一个新的部件名称，文件类型为"specification"，文件名为"165-001-01"
> 10. 根据当前打开部件的长度单位，创建一个新部件
> 11. 将新部件添加到当前装配中，设置其在装配中的位置和方向
> 12. 如果加载部件时有错误，报告错误信息
> 13. 设置工作部件为新创建的部件
> 14. 获取原始选择对象在新部件中的实例，并设置其名称为"new_occ"
> 15. 定义了ufusr函数，作为程序的入口，调用do_it函数执行主要流程。
> 16. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序可以被立即卸载。
>
> 综上所述，这段代码实现了一个简单的NX二次开发示例，其主要功能是在当前装配中添加一个新部件，并将原始选择对象复制到新部件中。
>
