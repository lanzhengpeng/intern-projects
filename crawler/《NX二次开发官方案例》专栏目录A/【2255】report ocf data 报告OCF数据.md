### 【2255】report ocf data 报告OCF数据

#### 代码

```cpp
    static void report_ocf_data(tag_t ocf_feature)  
    {  
        int  
            ii,  
            jj,  
            n_faces,  
            n_rules,  
            *rule_types;  
        tag_t  
            *faces;  
        UF_SC_input_data_t  
            *rules;  
        UF_CURVE_ocf_data_t  
            *offset_data;  
        if (UF_CALL(UF_CURVE_ask_ocf_data(ocf_feature, &offset_data))) return;  
        WRITE_D(offset_data->num_string_data);  
        for (ii = 0; ii < offset_data->num_string_data; ii++)  
        {  
            WRITE_D(ii);  
            RT(offset_data->string_data[ii].string_tag);  
            report_section_data(offset_data->string_data[ii].string_tag);  
            WRITE_D(offset_data->string_data[ii].offset_direction);  
            WRITE_D(offset_data->string_data[ii].num_offsets);  
            for (jj = 0; jj < offset_data->string_data[ii].num_offsets; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_S(offset_data->string_data[ii].offset_distances[jj].string);  
            }  
        }  
        RT(offset_data->face_data->face_tag);  
        if (!UF_CALL(UF_MODL_ask_smart_face_container(  
            offset_data->face_data->face_tag, &n_rules, &rule_types, &rules,  
            &n_faces, &faces)))  
        {  
            report_sc_input_data(n_rules, rule_types, rules);  
            for (ii = 0; ii < n_rules; ii++)  
                UF_CALL(UF_MODL_free_sc_input_data(rule_types[ii], &rules[ii]));  
            if (n_rules > 0)  
            {  
                UF_free(rule_types);  
                UF_free(rules);  
            }  
            WRITE_D(n_faces);  
            for (ii = 0; ii < n_faces; ii++)  
            {  
                WRITE_D(ii);  
                RT(faces[ii]);  
            }  
            if (n_faces > 0) UF_free(faces);  
        }  
        WRITE_D(offset_data->cross_boundary_mode);  
        WRITE_D(offset_data->offset_method);  
        WRITE_D(offset_data->trim_method);  
        WRITE_D(offset_data->span_method);  
        WRITE_F(offset_data->dist_tol);  
        WRITE_F(offset_data->ang_tol);  
        WRITE_F(offset_data->string_tol);  
        UF_CALL(UF_CURVE_free_ocf_data(&offset_data));  
    }

```

#### 代码解析

> 这段代码用于获取NX中的偏置特征(Offset Characteristic Feature, OCF)的数据，并打印出相关信息。
>
> 代码主要步骤包括：
>
> 1. 调用UF_CURVE_ask_ocf_data函数获取OCF特征的数据结构UF_CURVE_ocf_data_t *offset_data。
> 2. 遍历offset_data中的string_data数组，打印每个字符串特征的标签、偏置方向、偏置距离等信息。
> 3. 打印offset_data中的face_data的标签。
> 4. 调用UF_MODL_ask_smart_face_container函数获取face_data中的规则和面的信息，并打印出来。
> 5. 打印offset_data中的边界模式、偏置方法、修剪方法、跨距方法、距离公差、角度公差、字符串公差等参数。
> 6. 调用UF_CURVE_free_ocf_data释放offset_data。
>
> 总体来说，这段代码通过获取OCF特征的数据，打印出OCF特征的详细信息，用于二次开发中的信息获取和调试。
>
