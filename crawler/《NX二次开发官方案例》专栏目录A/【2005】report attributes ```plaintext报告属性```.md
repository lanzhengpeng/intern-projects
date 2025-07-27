### 【2005】report attributes ```plaintext报告属性```

#### 代码

```cpp
    /* This only works when the attribute references a single expression 译:这只有在属性引用单个表达式时才有效。 */  
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
        strcpy (exp_str, "");  
    /*  Reading a UF_ATTR_reference so assign the character array to the pointer 译:读取UF_ATTR_reference，所以将字符数组赋值给指针。 */  
        new_value.value.reference = new_str;  
    /*  Do not know the precision of the attribute value, so figure it out 译:不知道属性值的精确性，所以需要确定一下。 */  
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
    static void report_attributes(tag_t object)  
    {  
        int  
            indx = 0,  
            n;  
        char  
            exp_str[UF_MAX_EXP_LENGTH],  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_any, &n));  
        sprintf(mstr, "tag %d has %d attributes\n", object, n);  
        UF_CALL(UF_UI_write_listing_window(mstr));  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && (indx != 0))  
        {  
            switch ( att.type )  
            {  
                case UF_ATTR_integer:  
                    sprintf(mstr, "%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    sprintf(mstr, "%s = %f (FLOATING POINT)\n", title,  
                        att.value.real);  
                break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(att.value.time, 2, dstr, tstr));  
                    sprintf(mstr, "%s = %s %s (DATE & TIME)\n", title, dstr, tstr);  
                break;  
                case UF_ATTR_null:  
                    sprintf(mstr, "%s = (NULL)\n", title);  
                break;  
                case UF_ATTR_string:  
                    sprintf(mstr, "%s = %s (STRING)\n", title, att.value.string);  
                    UF_free(att.value.string);  
                break;  
                case UF_ATTR_reference:  
                    find_expression_of_reference_attribute(object, title,  
                        att.value.reference, exp_str);  
                    sprintf(mstr, "%s = %s = %s (REFERENCE)\n", title,  
                        exp_str, att.value.reference);  
                    UF_free(att.value.reference);  
                break;  
                default:  
                break;  
            }  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
        UF_CALL(UF_UI_write_listing_window("\n"));  
    }

```

#### 代码解析

> 这段NX Open API代码用于查询和报告NX部件的属性信息。
>
> 主要功能包括：
>
> 1. 查询部件属性：通过UF_ATTR_count_attributes函数获取部件的属性数量，然后使用UF_ATTR_cycle函数循环遍历所有属性。
> 2. 读取属性值：根据属性类型，使用不同的UF_ATTR_read_value函数读取属性值，如整数、浮点数、字符串等。
> 3. 处理引用属性：对于引用属性，会调用find_expression_of_reference_attribute函数，该函数会查找与属性值匹配的表达式，并将其字符串表示输出。
> 4. 报告属性信息：将属性信息格式化为字符串，并输出到列表窗口，包括属性名称、值和类型。
> 5. 处理日期时间属性：对于日期时间属性，使用uc4582函数将其转换为字符串。
> 6. 处理字符串和引用属性：在读取字符串和引用属性值后，释放对应的内存。
> 7. 使用UNDO标记：在查找引用属性的表达式时，使用UNDO标记来撤销操作。
> 8. 关闭显示：在查找表达式时关闭显示，以避免图形更新。
> 9. 获取表达式信息：调用UF_MODL相关函数获取表达式的值和字符串表示。
>
> 总体而言，这段代码实现了查询NX部件的属性信息，并将详细信息输出到列表窗口的功能。
>
