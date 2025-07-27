### 【1166】edit selected udf parameters 编辑选定的UDF参数

#### 代码

```cpp
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
    #include <uf_cfi.h>  
    #include <uf_sc.h>  
    #include <uf_undo.h>  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
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
    static int allocate_string_array(int n_elements, int n_chars, void ***where)  
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
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            resp;  
        tag_t  
            exp,  
            feature = NULL_TAG;  
        char  
            **exp_rhs,  
            *lhs,  
            *rhs,  
            *string;  
        UF_MODL_udf_exp_data_t  
            exp_data;  
        UF_MODL_udf_ref_data_t  
            ref_data;  
        if (((feature = select_a_feature("Select UDF to edit")) == NULL_TAG) ||  
            UF_CALL(UF_MODL_ask_instantiated_udf(feature, &exp_data, &ref_data)))  
            return;  
        UF_CALL(allocate_string_array(exp_data.num_exps, UF_MAX_EXP_LENGTH,  
            (void ***)&exp_rhs));  
        for (ii = 0; ii < exp_data.num_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(exp_data.old_exps[ii], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp));  
            strcpy(exp_rhs[ii], rhs);  
            prompt_for_text(lhs, exp_rhs[ii]);  
            UF_free(rhs);  
            UF_free(string);  
            UF_free(lhs);  
        }  
        exp_data.new_exp_values = exp_rhs;  
        // allocate a new array and copy original values  
        ref_data.new_refs = UF_allocate_memory(ref_data.num_refs * sizeof(tag_t), &resp);  
        for (ii = 0; ii < ref_data.num_refs; ii++)  
            ref_data.new_refs[ii] = ref_data.old_refs[ii];  
        set_undo_mark("Edit UDF");  
        UF_CALL(UF_MODL_edit_instantiated_udf(feature, FALSE, &exp_data,  
            &ref_data));  
        UF_MODL_udf_free_exp_data(&exp_data);  
        UF_MODL_udf_free_ref_data(&ref_data);  
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

> 这段代码是用于NX Open API开发的二次开发代码，主要实现了编辑已实例化的用户定义特征(UDF)的功能。以下是代码的主要功能介绍：
>
> 1. 错误处理机制：通过UF_CALL宏和report_error函数实现了错误处理，能捕捉函数调用错误并输出错误信息。
> 2. 选择UDF：通过select_a_feature函数让用户选择一个UDF。
> 3. 获取UDF信息：使用UF_MODL_ask_instantiated_udf函数获取UDF的实例信息，包括表达式和引用信息。
> 4. 编辑表达式：通过UF_MODL_ask_exp_tag_string和UF_MODL_dissect_exp_string函数获取表达式的左右侧字符串，然后通过prompt_for_text函数让用户修改表达式右侧字符串。
> 5. 编辑引用：通过复制引用信息来创建新的引用数组。
> 6. 设置撤销标记：使用UF_UNDO_set_mark函数设置撤销标记。
> 7. 编辑UDF：调用UF_MODL_edit_instantiated_udf函数来编辑已实例化的UDF。
> 8. 清理内存：使用UF_MODL_udf_free_exp_data和UF_MODL_udf_free_ref_data函数来清理表达式和引用信息占用的内存。
> 9. 初始化和终止：通过UF_initialize和UF_terminate函数来初始化和终止NX Open API。
> 10. 卸载处理：通过ufusr_ask_unload函数来处理卸载操作。
>
> 总体来说，这段代码实现了在NX环境下编辑已实例化UDF的功能，并提供了错误处理和撤销功能，是一个比较完善的NX二次开发示例。
>
