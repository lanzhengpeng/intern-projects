### 【2760】set expression tag value 设置表达式标签值

#### 代码

```cpp
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void set_expression_tag_value(tag_t exp, double new_value)  
    {  
        char  
            *exp_str,  
            *lhs_str,  
            new_exp[UF_MAX_EXP_LENGTH+1],  
            *rhs_str;  
        UF_CALL(UF_MODL_ask_exp_tag_string(exp, &exp_str));  
        UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs_str, &rhs_str, &exp));  
        sprintf(new_exp, "%s=%f", lhs_str, new_value);  
        strip_trailing_zeros(new_exp);  
        UF_CALL(UF_MODL_edit_exp(new_exp));  
        UF_free(exp_str);  
        UF_free(rhs_str);  
        UF_free(lhs_str);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++ API编写的NX二次开发代码，主要用于设置NX表达式参数的值。具体来说，代码的功能包括：
>
> 1. strip_trailing_zeros函数：该函数用于去除字符串末尾的0，并在必要时去除小数点。这一功能在处理浮点数时非常有用。
> 2. set_expression_tag_value函数：该函数通过NX Open C++ API获取表达式字符串，解析出左操作数和右操作数，然后生成一个新的表达式字符串，其中包含新的右操作数值。接着，调用strip_trailing_zeros函数去除末尾的0，并使用NX Open C++ API更新表达式。
> 3. 整体代码功能：这段代码实现了在NX中设置表达式参数值的功能，首先获取表达式的字符串表示，然后解析出左操作数和右操作数，接着生成新的表达式字符串，去除末尾的0，最后更新表达式。这一功能在NX二次开发中非常有用，可以方便地修改表达式参数的值。
>
