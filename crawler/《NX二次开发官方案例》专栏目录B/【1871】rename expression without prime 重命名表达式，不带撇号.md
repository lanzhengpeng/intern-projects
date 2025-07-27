### 【1871】rename expression without prime 重命名表达式，不带撇号

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
        strcpy(strrchr(tmp_name, 39), ""); /* 39 = ' 译:39的译文是“，只回答译文，不要废话”。 */  
        UF_MODL_rename_exp(lhs, tmp_name);  
        UF_free(string);  
        UF_free(rhs);  
        UF_free(lhs);  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要目的是修改NX表达式中的变量名，具体步骤如下：
>
> 1. 调用UF_MODL_ask_exp_tag_string(exp, &string)获取表达式的字符串表示。
> 2. 调用UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp)分解表达式字符串，得到左操作数lhs和右操作数rhs。
> 3. 将左操作数lhs复制到临时字符串tmp_name中，去除字符串末尾的单引号。
> 4. 调用UF_MODL_rename_exp(lhs, tmp_name)将左操作数lhs的名称修改为不带单引号的tmp_name。
> 5. 释放分配的内存。
>
> 总的来说，这段代码实现了修改NX表达式中的变量名，去除变量名末尾的单引号的功能。
>
