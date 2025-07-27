### 【2141】report gdt modifier data 报告GDT修改器数据

#### 代码

```cpp
    static void report_gdt_modifier_data(UF_GDT_modifier_data_p_t modifier_data)  
    {  
        logical  
            is_user_keyword;  
        int  
            ii;  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            *keyword_name,  
            *keyword_text;  
        if (modifier_data == NULL) return;  
        WRITE_D(modifier_data->num_modifiers);  
        WRITEnD(modifier_data->num_modifiers,  
            (int*)modifier_data->modifier_types);  
        WRITE_P(modifier_data->proj_zone);  
        if (modifier_data->proj_zone != NULL)  
        {  
            WRITE_D(modifier_data->proj_zone->expression);  
            WRITE_F(modifier_data->proj_zone->value);  
            WRITE_D(modifier_data->proj_zone->decimal_format);  
            WRITE_D(modifier_data->proj_zone->direction);  
        }  
        WRITE_P(modifier_data->unit_length);  
        if (modifier_data->unit_length != NULL)  
        {  
            WRITE_D(modifier_data->unit_length->tolerance_expression);  
            WRITE_D(modifier_data->unit_length->length_expression);  
            WRITE_D(modifier_data->unit_length->width_expression);  
            WRITE_D(modifier_data->unit_length->decimal_format);  
            WRITE_D(modifier_data->unit_length->ub_decimal_format);  
        }  
        WRITE_P(modifier_data->unit_area);  
        if (modifier_data->unit_area != NULL)  
        {  
            WRITE_D(modifier_data->unit_area->tolerance_expression);  
            WRITE_D(modifier_data->unit_area->length_expression);  
            WRITE_D(modifier_data->unit_area->width_expression);  
            WRITE_D(modifier_data->unit_area->decimal_format);  
            WRITE_D(modifier_data->unit_area->ub_decimal_format);  
        }  
        WRITE_P(modifier_data->bonus_tol);  
        if (modifier_data->bonus_tol != NULL)  
        {  
            WRITE_D(modifier_data->bonus_tol->expression);  
            WRITE_F(modifier_data->bonus_tol->value);  
            WRITE_D(modifier_data->bonus_tol->decimal_format);  
        }  
        WRITE_D(modifier_data->num_keywords);  
        for (ii = 0; ii < modifier_data->num_keywords; ii++)  
        {  
            WRITE_D(modifier_data->keywords[ii].id);  
            UF_CALL(UF_GDT_is_user_defined_keyword(part,  
                modifier_data->keywords[ii].id, &is_user_keyword));  
            WRITE_L(is_user_keyword);  
            UF_CALL(UF_GDT_ask_keyword_name(part, modifier_data->keywords[ii].id,   
                &keyword_name));  
            WRITE_S(keyword_name);  
            UF_CALL(UF_GDT_ask_keyword_text(part, keyword_name, &keyword_text));  
            WRITE_S(keyword_text);  
            UF_free(keyword_text);  
            UF_free(keyword_name);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于报告GDT(几何尺寸与公差)修饰符数据。
>
> 主要功能包括：
>
> 1. 获取当前工作零件
> 2. 报告修饰符数据修饰符数量修饰符类型投影区域单位长度单位面积奖励公差
> 3. 修饰符数量
> 4. 修饰符类型
> 5. 投影区域
> 6. 单位长度
> 7. 单位面积
> 8. 奖励公差
> 9. 报告关键词数据关键词数量每个关键词的ID是否是用户自定义关键词关键词名称关键词文本
> 10. 关键词数量
> 11. 每个关键词的ID
> 12. 是否是用户自定义关键词
> 13. 关键词名称
> 14. 关键词文本
> 15. 释放内存释放关键词名称和文本的内存
> 16. 释放关键词名称和文本的内存
>
> 代码结构清晰，逻辑性强，通过调用UF_GDT系列函数，可以获取零件的GDT修饰符和关键词数据，并输出报告。这对于GDT相关功能的二次开发非常有帮助。
>
