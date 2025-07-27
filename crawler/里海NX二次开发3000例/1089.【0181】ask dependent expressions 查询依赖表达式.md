### 【0181】ask dependent expressions 查询依赖表达式

#### 代码

```cpp
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

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要实现了以下功能：
>
> 1. strip_it函数用于去除字符串末尾的’0’字符。
> 2. allocate_memory函数用于分配指定大小的内存空间。
> 3. make_an_array函数用于从链表中提取元素，生成一个tag数组，并释放链表。
> 4. ask_dependent_expressions函数用于查找一个表达式所依赖的其他表达式。它首先获取当前工作部件的所有表达式，然后逐个检查每个表达式在修改给定表达式后是否发生变化。如果发生变化，则认为该表达式依赖于给定表达式，并将其添加到依赖列表中。最后，该函数返回依赖表达式的tag数组。
>
> 整个代码实现了从NX模型中查询表达式依赖关系的功能，为后续的二次开发提供了基础。
>
