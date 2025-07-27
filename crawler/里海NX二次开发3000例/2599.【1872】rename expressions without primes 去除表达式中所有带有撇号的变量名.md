### 【1872】rename expressions without primes 去除表达式中所有带有撇号的变量名

#### 代码

```cpp
    static void rename_expression_without_prime(tag_t exp)  
    {  
        char  
            *lhs,  
            tmp_name[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            *string;  
        UF_CALL(UF_MODL_ask_exp_tag_string(exp, &string));  
        UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp));  
        strcpy(tmp_name, lhs);  
        strcpy(strrstr(tmp_name, "'"), "");  
        UF_MODL_rename_exp(lhs, tmp_name);  
        UF_free(string);  
        UF_free(rhs);  
        UF_free(lhs);  
    }  
    static void rename_expressions_without_primes(tag_t *exps, int n_exps)  
    {  
        int ii;  
        for (ii = 0; ii < n_exps; ii++) rename_expression_without_prime(exps[ii]);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于修改NX表达式中的变量名，以去除变量名中的单引号。
>
> 代码的主要功能如下：
>
> 1. rename_expression_without_prime函数：用于重命名单个表达式，去除变量名中的单引号。
> 2. rename_expressions_without_primes函数：用于批量重命名多个表达式。
> 3. 使用了UF_MODL_ask_exp_tag_string函数获取表达式的字符串形式。
> 4. 使用了UF_MODL_dissect_exp_string函数解析表达式字符串，分离出左部和右部。
> 5. 使用了strcpy和strrstr函数去除变量名中的单引号。
> 6. 使用了UF_MODL_rename_exp函数重命名表达式。
> 7. 使用了UF_free函数释放内存。
> 8. 使用了循环调用rename_expression_without_prime函数，批量重命名多个表达式。
>
> 总的来说，这段代码实现了去除NX表达式变量名中的单引号的功能，方便进行后续的变量处理。
>
