### 【2142】report gdt size tolerance parms 报告GDT尺寸公差参数

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
    static void report_gdt_modifier_data(UF_GDT_modifier_data_p_t modifier_data)  
    {  
        int  
            ii;  
        if (modifier_data == NULL) return;  
        WRITE_D(modifier_data->num_modifiers);  
        WRITEnD(modifier_data->num_modifiers,  
            modifier_data->modifier_types);  
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
    static void report_gdt_size_tolerance_parms(tag_t size_tol_feat)  
    {  
        UF_GDT_size_tolerance_p_t  
            size_data;  
        if (UF_CALL(UF_GDT_ask_size_tolerance_parms(size_tol_feat, &size_data)))  
            return;  
        WRITE_D(size_data->value_type);  
        WRITE_D(size_data->dimension);  
        WRITE_F(size_data->nominal_value);  
        WRITE_D(size_data->upper_tol.expression);  
        WRITE_F(size_data->upper_tol.value);  
        WRITE_D(size_data->upper_tol.decimal_format);  
        WRITE_D(size_data->lower_tol.expression);  
        WRITE_F(size_data->lower_tol.value);  
        WRITE_D(size_data->lower_tol.decimal_format);  
        WRITE_D(size_data->tol_format);  
        WRITE_P(size_data->modifier_data);  
        report_gdt_modifier_data(size_data->modifier_data);  
        WRITE_D(size_data->decimal_format);  
        UF_CALL(UF_GDT_free(UF_GDT_SIZE_TOL_TYPE, size_data));  
    }

```

#### 代码解析

> 这段代码是NX（Siemens NX）的二次开发代码，主要实现了以下功能：
>
> 1. 定义了几个宏，用于在NX的Listing窗口中输出不同类型的数据，包括指针、整数、浮点数、字符串等。
> 2. 定义了函数report_gdt_modifier_data，用于输出NX中的GDT Modifier数据结构的内容。
> 3. 定义了函数report_gdt_feature_parms，用于输出NX中的GDT Feature Parameters数据结构的内容。
> 4. 定义了函数report_gdt_size_tolerance_parms，用于输出NX中的GDT Size Tolerance Parameters数据结构的内容。
> 5. 这三个函数分别用于输出NX中公差分析相关的不同数据结构的内容，包括Modifier信息、Feature参数和尺寸公差参数。
> 6. 使用UF_CALL宏来调用NX的API函数，并处理函数返回值。
> 7. 通过定义宏和函数，实现了公差分析数据的输出，可用于调试和查看NX中的公差分析数据。
> 8. 整体上，这段代码展示了如何在NX二次开发中输出公差分析相关的数据结构，具有一定的参考价值。
>
