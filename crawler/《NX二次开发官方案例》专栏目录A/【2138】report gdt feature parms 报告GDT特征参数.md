### 【2138】report gdt feature parms 报告GDT特征参数

#### 代码

```cpp
    #define WRITE_P(X) (write_pointer_to_listing_window(#X, X))  
    static void write_pointer_to_listing_window(char *title, void *pointer)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (pointer == NULL)  
            sprintf(msg, "%s = NULL\n", title);  
        else  
            sprintf(msg, "%s != NULL\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
    static void report_gdt_feature_parms(tag_t tolerance_feature)  
    {  
        int  
            ii;  
        UF_GDT_feature_parms_p_t  
            feat_parms;  
        if (UF_CALL(UF_GDT_ask_feature_parms(tolerance_feature, &feat_parms)))  
            return;  
        WRITE_D(feat_parms->num_faces);  
        WRITEnD(feat_parms->num_faces, (int *)feat_parms->face_list);  
        WRITE_S(feat_parms->descript->name);  
        WRITE_S(feat_parms->descript->description);  
        WRITE_D(feat_parms->feature);  
        WRITE_D(feat_parms->pattern);  
        WRITE_P(feat_parms->modifier_data);  
        report_gdt_modifier_data(feat_parms->modifier_data);  
        WRITE_D(feat_parms->num_sets);  
        for (ii = 0; ii < feat_parms->num_sets; ii++)  
        {  
            WRITE_D(feat_parms->sets[ii]->num_faces);  
            WRITEnD(feat_parms->sets[ii]->num_faces,  
                (int *)feat_parms->sets[ii]->faces);  
        }  
        WRITE_D(feat_parms->def.type);  
        WRITE_D(feat_parms->def.so_tag);  
        WRITE_F(feat_parms->def.radius);  
        WRITE_D(feat_parms->def.trimmed_sheet);  
        UF_CALL(UF_GDT_free(UF_GDT_FEAT_PARMS_TYPE, feat_parms));  
    }

```

#### 代码解析

> 这段代码是用于在NX中报告GD&T公差特征的参数和修饰符数据的二次开发代码。
>
> 主要功能包括：
>
> 1. 使用宏定义了各种类型的打印函数，用于在列表窗口中打印不同类型的数据。
> 2. 定义了一个函数report_gdt_modifier_data，用于报告GD&T公差特征的修饰符数据，包括投影区域、长度单位、面积单位和奖励公差等。
> 3. 定义了一个函数report_gdt_feature_parms，用于报告GD&T公差特征的所有参数，包括面列表、描述、特征、模式、修饰符数据、集合、默认值等。
> 4. 在report_gdt_feature_parms中，使用了UF_GDT_ask_feature_parms函数获取特征参数，并逐项打印，同时也调用了report_gdt_modifier_data来打印修饰符数据。
> 5. 在打印修饰符数据时，会判断是否为空，不为空则打印各项内容。
> 6. 在打印关键词时，会判断是否为用户定义，并打印关键词名称和文本。
> 7. 在打印面列表、集合面列表等数组时，会逐项打印。
> 8. 在打印完成后，会释放特征参数内存。
>
> 总的来说，这段代码实现了在NX中获取和打印GD&T公差特征参数和修饰符数据的功能，通过宏定义简化了打印不同类型数据的代码，通过封装实现了打印修饰符数据的复用。
>
