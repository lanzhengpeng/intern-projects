### 【2293】report reference sets used for component members of reference sets 报告用于参考集组件成员的参考集

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_ugmgr.h>  
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
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
        }  
        return NULL_TAG;  
    }  
    static void build_unique_temp_part_name(char *fspec)  
    {  
        logical  
            tceng;  
        char  
            *tmp_dir,  
            unique[MAX_FSPEC_SIZE+1];  
        UF_CALL(uc4577(unique));  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
            UF_CALL(UF_UGMGR_encode_part_filename(unique, "A", "", "", fspec));  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
        }  
    }  
    static tag_t create_new_unique_part_quietly(void)  
    {  
        int  
            units;  
        tag_t  
            new_part,  
            old_part = UF_PART_ask_display_part();  
        char  
            unique[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
        UF_CALL(UF_PART_ask_units(old_part, &units));  
        build_unique_temp_part_name(unique);  
        UF_CALL(UF_PART_new(unique, units, &new_part));  
        UF_CALL(UF_PART_set_display_part(old_part));  
        UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
        return new_part;  
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
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void do_it(void)  
    {  
        logical  
            once = FALSE;  
        int  
            ii,  
            n_members,  
            subtype,  
            type;  
        tag_t  
            new_part,  
            *members,  
            new_inst,  
            new_part_occ,  
            occ,  
            part = UF_PART_ask_display_part(),  
            ref_set = NULL_TAG;  
        double  
            abs[6] = { 1,0,0, 0,1,0 },  
            origin[3],  
            csys[9],  
            trans[4][4];  
        char  
            component_name[MAX_ENTITY_NAME_SIZE+1],  
            comp_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            comp_inst[UF_CFI_MAX_FILE_NAME_SIZE],  
            reference_set[MAX_ENTITY_NAME_SIZE+1],  
            this_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            uses_ref_set[MAX_ENTITY_NAME_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        UF_CALL(UF_PART_ask_part_name(part, this_part));  
        while ((ref_set = ask_next_ref_set(part, ref_set)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(ref_set, reference_set));  
            WRITE_S(reference_set);  
            if (!once)  
            {  
                new_part = create_new_unique_part_quietly();  
                UF_CALL(UF_ASSEM_add_part_to_assembly(new_part, this_part,  
                    reference_set, NULL, abs, abs, -1, &new_inst, &error_status));  
                report_load_status(&error_status);  
                new_part_occ = UF_ASSEM_ask_part_occ_of_inst(  
                    UF_ASSEM_ask_root_part_occ(new_part), new_inst);  
                once = TRUE;  
            }  
            else  
            {  
                UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
                UF_CALL(UF_PART_set_display_part(new_part));  
                UF_CALL(UF_ASSEM_replace_refset(1, &new_part_occ, reference_set));  
                UF_CALL(UF_PART_set_display_part(part));  
                UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            }  
            UF_CALL(UF_ASSEM_ask_ref_set_members(ref_set, &n_members, &members));  
            for (ii = 0; ii < n_members; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(members[ii], &type, &subtype));  
                if (type == UF_occ_instance_type)  
                {  
                    UF_CALL(UF_OBJ_ask_name(members[ii], component_name));  
                    occ = UF_ASSEM_ask_part_occ_of_inst(new_part_occ, members[ii]);  
                    if (occ == NULL_TAG)  
                    {  
                        UF_CALL(UF_ASSEM_ask_component_data(members[ii], comp_part,  
                            uses_ref_set, comp_inst, origin, csys, trans));  
                        ECHO("   component %s is suppressed in the default arrangement\n",  
                        component_name, uses_ref_set);  
                    }  
                    else  
                    {  
                        UF_CALL(UF_ASSEM_ask_component_data(occ, comp_part,  
                            uses_ref_set, comp_inst, origin, csys, trans));  
                        ECHO("   component %s uses reference set:- %s.\n",  
                            component_name, uses_ref_set);  
                    }  
                }  
            }  
            if (n_members > 0) UF_free(members);  
        }  
        if (once) UF_CALL(UF_PART_close(new_part, 0, 1));  
        UF_CALL(UF_DISP_regenerate_display()); // needed with NX12 MDP after UF_DISP_set_display  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个ECHO宏，用于输出信息到日志窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用NX的API函数，并在调用失败时报告错误。
> 3. ask_next_ref_set函数用于遍历当前部件的所有参考集，并返回下一个参考集的tag。
> 4. build_unique_temp_part_name函数用于构建一个唯一的临时部件文件名。
> 5. create_new_unique_part_quietly函数用于创建一个新部件，并设置显示部件为当前部件。
> 6. report_load_status函数用于报告加载部件的状态信息。
> 7. WRITE_S宏用于输出字符串到日志窗口。
> 8. do_it函数是主函数，主要逻辑如下：遍历当前部件的所有参考集如果是第一个参考集，则创建一个新部件，并将当前部件作为新部件的根部件如果不是第一个参考集，则替换新部件的参考集为新参考集遍历参考集的成员，如果是实例，则输出实例信息
> 9. 遍历当前部件的所有参考集
> 10. 如果是第一个参考集，则创建一个新部件，并将当前部件作为新部件的根部件
> 11. 如果不是第一个参考集，则替换新部件的参考集为新参考集
> 12. 遍历参考集的成员，如果是实例，则输出实例信息
> 13. ufusr函数是NX调用的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 14. ufusr_ask_unload函数用于设置立即卸载用户函数。
>
> 总的来说，这段代码主要实现了遍历当前部件的参考集，并根据参考集创建或更新一个新部件的功能。
>
