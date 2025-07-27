### 【0356】ask tag of expression name 询问表达式名称的标签

#### 代码

```cpp
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
    /*  In NX 4 and 5 the lookup of the expression name is case sensitive.  
        In NX 6 it is not.  
    */  
        if (!UF_CALL(UF_MODL_ask_exp(exp_name, exp_str)) &&  
            !UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要实现的功能是查询NX表达式名对应的表达式标签。
>
> 具体步骤如下：
>
> 1. 定义了查询函数ask_tag_of_expression_name，输入参数为表达式名exp_name。
> 2. 初始化字符串exp_str，用于存储查询到的表达式字符串。
> 3. 调用UF_MODL_ask_exp函数，根据表达式名exp_name查询表达式字符串，并存储到exp_str中。
> 4. 调用UF_MODL_dissect_exp_string函数，解析表达式字符串，获取表达式标签exp_tag，同时获取表达式字符串的左右部分lhs和rhs。
> 5. 释放lhs和rhs占用的内存。
> 6. 如果查询和解析成功，返回表达式标签exp_tag；否则返回NULL_TAG。
> 7. 在NX 4和5中，表达式名查询区分大小写；而在NX 6中，表达式名查询不区分大小写。
> 8. 该函数封装了NX表达式查询和解析的功能，便于后续使用。
>
> 总的来说，这段代码实现了根据NX表达式名查询表达式标签的功能，为后续的二次开发提供了便利。
>
