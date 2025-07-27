### 【2400】report udfs def data 报告UDF定义数据

#### 代码

```cpp
    static void report_udfs_def_data(UF_MODL_udfs_def_data_t udfs_data)  
    {  
        int  
            ii,  
            jj;  
        printf("\tudfs_data.num_frecs = %d\n", udfs_data.num_frecs);  
        for (ii = 0; ii < udfs_data.num_frecs; ii++)  
            printf("\tudfs_data.frecs[%d] = %d\n", ii, udfs_data.frecs[ii]);  
        printf("\tudfs_data.num_exp = %d\n", ii, udfs_data.num_exp);  
        for (ii = 0; ii < udfs_data.num_exp; ii++)  
        {  
            printf("\tudfs_data.exp_defs[%d]->type = %d\n", ii,  
                udfs_data.exp_defs[ii]->type);  
            printf("\tudfs_data.exp_defs[%d]->exp = %s\n", ii,  
                udfs_data.exp_defs[ii]->exp);  
            printf("\tudfs_data.exp_defs[%d]->exp_name = %s\n", ii,  
                udfs_data.exp_defs[ii]->exp_name);  
            switch (udfs_data.exp_defs[ii]->type)  
            {  
                case UF_UDFS_EXP_OPTION:  
                    printf("\tudfs_data.exp_defs[%d]->num_def = %d\n", ii,  
                        udfs_data.exp_defs[ii]->num_def);  
                    for (jj = 0; jj < udfs_data.exp_defs[ii]->num_def; jj++)  
                        printf("\tudfs_data.exp_defs[%d]->define_value[%d]"  
                            "= %s\n", ii, jj,  
                            udfs_data.exp_defs[ii]->define_value[jj]);  
                    break;  
                case UF_UDFS_EXP_RANGE:  
                    printf("\tudfs_data.exp_defs[%d]->low_end = %s\n", ii,  
                        udfs_data.exp_defs[ii]->low_end);  
                    printf("\tudfs_data.exp_defs[%d]->high_end = %s\n", ii,  
                        udfs_data.exp_defs[ii]->high_end);  
                    printf("\tudfs_data.exp_defs[%d]->scale_type = %d\n",  
                        ii, udfs_data.exp_defs[ii]->scale_type);  
                    break;  
                default:  
                    break;  
            }  
        }  
        printf("\tudfs_data.num_ref = %d\n", ii, udfs_data.num_ref);  
        for (ii = 0; ii < udfs_data.num_ref; ii++)  
        {  
            printf("\tudfs_data.refs[%d] = %d\n", ii, udfs_data.refs[ii]);  
            printf("\tudfs_data.ref_prompts[%d] = %s\n", ii,  
                udfs_data.ref_prompts[ii]);  
        }  
        printf("\tudfs_data.name_str_data = %s\n", udfs_data.name_str_data);  
    }

```

#### 代码解析

> 根据代码，这是一段用于打印NX UDFs(用户定义函数)定义数据的C语言函数。
>
> 函数名为report_udfs_def_data，它接受一个UF_MODL_udfs_def_data_t结构体参数，该结构体包含UDF的定义数据。
>
> 函数首先打印出UDF中的频率数和每个频率的值。然后，它遍历UDF中的表达式定义，打印每个表达式的类型、表达式字符串、名称以及相关定义值。根据表达式的类型，它还会打印出范围表达式中的下限、上限和比例类型。接着，函数打印出UDF中的引用数和每个引用的值以及提示信息。最后，函数打印出UDF的名称字符串。
>
> 总体来说，这个函数的目的是打印出UDF的定义信息，包括频率、表达式、引用等，以便于调试和查看UDF的定义情况。
>
