### 【1458】insert udf in batch mode 在批处理模式下插入UDF

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
                ECHO("  %s - %s", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static logical do_it(void)  
    {  
        int  
            ii,  
            num_parents,  
            num_expression;  
        tag_t  
            *expression = NULL,  
            exp,  
            feature = NULL_TAG,  
            new_udf_feature,  
            *parents = NULL,  
            part,  
            prev_work_part;  
        char  
            **exp_rhs,  
            **expression_prompt = NULL,  
            *lhs,  
            **parents_prompt = NULL,  
            *rhs,  
            *string,  
            *udf_part = "Z:\\udf\\cube.prt";  
        UF_PART_load_status_t  
            error_status;  
        UF_CALL(UF_PART_open_quiet(udf_part, &part, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(part, &prev_work_part));  
        feature = ask_next_feature_of_type(part, "UDF_DEF", feature);  
        UF_CALL(UF_MODL_ask_udf_definition(feature, &parents, &parents_prompt,  
            &num_parents, &expression, &expression_prompt, &num_expression));  
        if (num_parents > 0)  
        {  
            ECHO("num_parents = %d\n", num_parents);  
            ECHO("This program is not designed for UDFs requiring parents!\n");  
            UF_free(parents);  
            UF_free_string_array(num_parents, parents_prompt) ;  
            UF_CALL(UF_PART_close(part, 0, 1));  
            return FALSE;  
        }  
        UF_CALL(allocate_string_array(num_expression, UF_MAX_EXP_LENGTH, &exp_rhs));  
        for (ii = 0; ii < num_expression; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(expression[ii], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp));  
            strcpy(exp_rhs[ii], rhs);  
            UF_free(lhs);  
            UF_free(rhs);  
            UF_free(string);  
        }  
        UF_CALL(UF_ASSEM_set_work_part_quietly(prev_work_part, &part));  
        UF_CALL(UF_MODL_create_instantiated_udf(feature, NULL, NULL, NULL, 0,  
            expression, exp_rhs, num_expression, &new_udf_feature));  
        UF_free(expression);  
        UF_free_string_array(num_expression, expression_prompt);  
        UF_free_string_array(num_expression, exp_rhs);  
        UF_CALL(UF_PART_close(part, 0, 1));  
        return TRUE;  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        char  
            dspec[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_ENTITY_NAME_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            *tmp_dir;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (!tceng)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            strcat(part_name, "-new");  
            UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
        }  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            UF_CALL(uc4575(tmp_dir, 2, part_number, copy_fspec));  
        }  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            new_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                ECHO("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    if (do_it())  
                    {  
                        build_copy_partname(new_fspec);  
                        if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                            ECHO("\tsaved as %s\n", new_fspec);  
                    }  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            ECHO("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发代码，主要实现以下功能：
>
> 1. 加载和查询UDF特征：代码首先加载一个部件文件，并查询该部件中的所有UDF_DEF特征，然后获取每个UDF_DEF特征的参数表达式。
> 2. 创建实例化UDF特征：根据UDF_DEF特征定义，创建实例化的UDF特征，并插入到当前部件中。
> 3. 保存修改后的部件：对每个加载的部件文件进行处理，如果处理成功，则保存修改后的部件文件到新文件。
> 4. 参数输入：代码提供了两种方式输入待处理的部件文件路径：命令行参数和NX部件管理器。
> 5. 错误处理和日志记录：代码中包含错误处理和日志记录功能。
> 6. 支持NX和UG两种模式：代码兼容NX和UG两种模式，通过检测UG管理器是否激活来选择不同的文件路径处理方式。
>
> 总体来说，这段代码实现了UDF特征的加载、查询、实例化创建和保存，并提供了友好的用户交互和日志记录功能。
>
