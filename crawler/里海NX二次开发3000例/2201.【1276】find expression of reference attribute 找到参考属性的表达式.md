### 【1276】find expression of reference attribute 找到参考属性的表达式

#### 代码

```cpp
    /* This only works when the attribute references a single expression 译:这仅适用于属性引用单个表达式的情况。 */  
    static void find_expression_of_reference_attribute(tag_t object,  
        char *att_title, char *att_value, char *exp_str)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            part = UF_ASSEM_ask_work_part();  
        double  
            att_val = atof(att_value),  
            exp_val,  
            t_val,  
            tweak;  
        UF_UNDO_mark_id_t  
            mark;  
        char  
            *lhs,  
            new_str[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            *t_str;  
        UF_ATTR_value_t  
            new_value;  
        strcpy(exp_str, "");  
    /*  Reading a UF_ATTR_reference so assign the character array to the pointer 译:根据上下文，该句可翻译为：读取 UF_ATTR_reference，因此将字符数组赋值给指针。 */  
        new_value.value.reference = new_str;  
    /*  Do not know the precision of the attribute value, so figure it out 译:不知道属性值的精确度，所以弄清楚一下。 */  
        strcpy(new_str, att_value);  
        for (ii = 0; ii < strlen(new_str) - 2; ii++)  
            if (new_str[ii] != '.') new_str[ii] = '0';  
        new_str[strlen(new_str) - 1] = '1';  
        tweak = atof(new_str);  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps));  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &exp_val));  
            if (abs(exp_val - att_val) < tweak)  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &t_str));  
                UF_CALL(UF_MODL_dissect_exp_string(t_str, &lhs, &rhs, &exps[ii]));  
                t_val = exp_val + tweak;  
                sprintf(new_str, "%s=%f", lhs, t_val);  
                UF_free(lhs);  
                UF_free(rhs);  
                UF_CALL(UF_MODL_edit_exp(new_str));  
                UF_CALL(UF_MODL_update());  
                UF_CALL(UF_ATTR_read_value(object, att_title, UF_ATTR_reference,  
                    &new_value));  
                if (abs(atof(new_value.value.reference) - t_val) < tweak)  
                    strcpy(exp_str, t_str);  
                UF_free(t_str);  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
            }  
        }  
        UF_free(exps);  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }

```

#### 代码解析

> 这段代码是一个NX Open API函数，用于查找引用属性的数学表达式。
>
> 主要功能包括：
>
> 1. 根据给定的对象、属性名称和属性值，查找对应的数学表达式。
> 2. 首先确定属性值的精度，然后遍历所有表达式，找到与属性值接近的表达式。
> 3. 解析表达式，获取左值和右值，并将右值修改为目标属性值。
> 4. 更新模型，读取修改后的属性值，验证是否正确。
> 5. 如果正确，将表达式字符串返回给调用者。
> 6. 使用UNDO标记，可以撤销修改，保证模型的完整性。
> 7. 最后恢复显示设置。
>
> 总体来说，这段代码实现了通过属性值查找对应表达式的功能，为NX二次开发提供了便捷的方式。
>
