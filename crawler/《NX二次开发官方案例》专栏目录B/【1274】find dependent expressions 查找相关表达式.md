### 【1274】find dependent expressions 查找相关表达式

#### 代码

```cpp
    /*HEAD FIND_DEPENDENT_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

注意：UF_print_syslog是在V18版本中新增的，只回答翻译内容，不要添加其他无关信息。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
        UF_CALL(UF_MODL_ask_exp(exp_name, exp_str));  
        if (!UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
    }  
    static void strip_it(double n, char *s)  
    {  
        int ii;  
        sprintf(s, "%f", n);  
        for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_dependent_expressions(tag_t exp, tag_t **dep_exps)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            part = UF_ASSEM_ask_work_part();  
        double  
            exp_val,  
            *old_vals;  
        UF_UNDO_mark_id_t  
            mark;  
        char  
            *exp_str,  
            *lhs,  
            new_str[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            val_str[UF_MAX_EXP_LENGTH+1];  
        uf_list_p_t  
            dep_list;  
        if (exp == NULL_TAG) return 0;  
        UF_CALL(UF_MODL_ask_exp_tag_string(exp, &exp_str));  
        UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp));  
        UF_CALL(UF_MODL_ask_exp_tag_value(exp, &exp_val));  
        strip_it(exp_val, val_str);  
        strcat(val_str, "1");  
        sprintf(new_str, "%s=%s", lhs, val_str);  
        UF_free(lhs);  
        UF_free(rhs);  
        UF_free(exp_str);  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps));  
        UF_CALL(allocate_memory(n_exps * sizeof(double), (void **) &old_vals));  
        for (ii = 0; ii < n_exps; ii++)  
            UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &old_vals[ii]));  
        UF_CALL(UF_MODL_create_list(&dep_list));  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_MODL_edit_exp(new_str));  
        UF_CALL(UF_MODL_update());  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &exp_val));  
            if (exp_val != old_vals[ii])  
                UF_CALL(UF_MODL_put_list_item(dep_list, exps[ii]));  
        }  
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
        UF_CALL(UF_DISP_set_display(TRUE));  
        UF_free(exps);  
        UF_CALL(UF_MODL_delete_list_item(&dep_list, exp));  
        return make_an_array(&dep_list, dep_exps);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_deps;  
        tag_t  
            *dep_exps,  
            exp_tag;  
        char  
            exp_name[UF_MAX_EXP_LENGTH+1] = { "" },  
            *exp_str;  
        while (prompt_for_text("Find dependent expressions", exp_name))  
        {  
            exp_tag = ask_tag_of_expression_name(exp_name);  
            if (exp_tag == NULL_TAG) continue;  
            UF_CALL(UF_MODL_ask_exp_tag_string(exp_tag, &exp_str));  
            WRITE(exp_str);  
            WRITE("\n");  
            UF_free(exp_str);  
            n_deps = ask_dependent_expressions(exp_tag, &dep_exps);  
            WRITE_D(n_deps);  
            for (ii = 0; ii < n_deps; ii++)  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(dep_exps[ii], &exp_str));  
                WRITE(exp_str);  
                WRITE("\n");  
                UF_free(exp_str);  
            }  
            if (n_deps > 0) UF_free(dep_exps);  
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

> 这段代码是NX Open C++ API编写的NX二次开发程序，其主要功能是查询一个表达式在NX部件中的依赖表达式。以下是代码的主要内容和功能：
>
> 1. 包含NX Open C++ API的头文件，如uf.h、uf_ui.h、uf_modl.h等。
> 2. 定义了错误处理函数report_error，用于打印错误信息。
> 3. 定义了提示输入文本的函数prompt_for_text。
> 4. 定义了根据表达式名称查询表达式tag的函数ask_tag_of_expression_name。
> 5. 定义了去除字符串末尾0的函数strip_it。
> 6. 定义了分配内存的函数allocate_memory。
> 7. 定义了将链表转换为数组的函数make_an_array。
> 8. 定义了查询依赖表达式的函数ask_dependent_expressions，其主要逻辑是先保存所有表达式的原始值，然后修改目标表达式的值，再对比表达式的值变化，变化了的即为依赖表达式。
> 9. 定义了写入信息的宏WRITE和写入整数的函数write_integer_to_listing_window。
> 10. 定义了主函数do_it，循环提示用户输入表达式名称，查询依赖表达式并打印。
> 11. 定义了ufusr函数，作为程序的入口，初始化后调用do_it函数，然后终止。
> 12. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码通过NX Open C++ API实现了查询表达式依赖表达式的功能，具有较好的注释和结构。
>
