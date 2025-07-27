### 【1460】insert user defined feature 插入用户定义特征

#### 代码

```cpp
    /*HEAD INSERT_USER_DEFINED_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。

仅提供译文，不添加其他内容。 */  
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
    static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static tag_t ask_next_udf_definition(tag_t part, tag_t feat)  
    {  
        char  
            *type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
                if (!strcmp(type, "UDF_DEF"))  
                {  
                    UF_free(type);  
                    return feat;  
                }  
                UF_free(type);  
            }  
        }  
        return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_parents,  
            n_exps;  
        tag_t  
            exp_tag,  
            *exps,  
            feature = NULL_TAG,  
            *new_parents,  
            new_udf_feature,  
            *parents,  
            part,  
            prev_work_part;  
        char  
            **exp_prompts = NULL,  
            *lhs,  
            **parents_prompt = NULL,  
            new_exp[UF_MAX_EXP_LENGTH],  
            **new_exp_rhs,  
            *rhs,  
            *string;  
        UF_PART_load_status_t  
            error_status;  
        UF_CALL(UF_PART_open_quiet("boss.prt", &part, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(part, &prev_work_part));  
        feature = ask_next_udf_definition(part, feature);  
        UF_CALL(UF_MODL_ask_udf_definition(feature, &parents, &parents_prompt,  
            &n_parents, &exps, &exp_prompts, &n_exps));  
        UF_CALL(allocate_memory(sizeof(char *)*n_exps, (void *)&new_exp_rhs));  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp_tag));  
            strcpy(new_exp, rhs);  
            prompt_for_text(exp_prompts[ii], new_exp);  
            UF_CALL(allocate_memory((sizeof(char) * strlen(new_exp))+1,   
                (void *)&new_exp_rhs[ii]));  
            strcpy(new_exp_rhs[ii], new_exp);  
            UF_free(lhs);  
            UF_free(rhs);  
            UF_free(string);  
        }  
        UF_CALL(UF_ASSEM_set_work_part_quietly(prev_work_part, &part));  
        UF_CALL(allocate_memory(sizeof(tag_t) * n_parents, (void *)&new_parents));  
        for (ii = 0; ii < n_parents; ii++)  
            new_parents[ii] = select_planar_face(parents_prompt[ii]);  
        UF_CALL(UF_MODL_register_rpo_routine(UF_MODL_default_rpo_menu));  
        UF_CALL(UF_MODL_create_instantiated_udf(feature, "boss.cgm", parents,  
            new_parents, n_parents, exps, new_exp_rhs, n_exps,  
            &new_udf_feature));  
        UF_free(parents);  
        UF_free(new_parents);  
        UF_free_string_array(n_parents, parents_prompt) ;  
        UF_free(exps);  
        UF_free_string_array(n_exps, exp_prompts);  
        UF_free_string_array(n_exps, new_exp_rhs);  
        UF_CALL(UF_PART_close(part, 0, 1));  
        WRITE_D(new_udf_feature);  
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
    /*  Note:  can NOT be unloaded - calls UF_MODL_register_rpo_routine.  
        return (UF_UNLOAD_IMMEDIATELY);  
    */  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 根据代码内容，这是一段NX Open API的二次开发代码，其主要功能是创建一个用户定义特征(UDF)实例。以下是代码的主要步骤：
>
> 1. 包含必要的头文件，定义错误报告函数。
> 2. 定义一个用于选择平面的函数mask_for_planar_face()，并在对话框中调用该函数。
> 3. 定义一个函数用于选择平面，并返回平面特征标签。
> 4. 定义一个函数用于循环遍历部件中的特征，并返回下一个UDF定义特征。
> 5. 定义一些辅助函数，用于向列表窗口写入信息、分配内存、提示输入文本等。
> 6. 在do_it()函数中，打开一个prt文件，设置工作部件，选择一个UDF定义特征。
> 7. 获取该UDF定义的父特征和表达式，提示用户修改表达式值。
> 8. 选择新的父平面特征，注册默认的RPO菜单。
> 9. 使用修改后的表达式和父特征创建一个UDF实例，并关闭prt文件。
> 10. 定义ufusr()函数，在NX启动时调用do_it()函数。
> 11. 定义ufusr_ask_unload()函数，返回不能卸载该代码，因为调用了UF_MODL_register_rpo_routine()。
>
> 总体来说，这段代码实现了通过选择一个UDF定义，并修改其表达式和父特征，来创建一个UDF实例的功能。
>
