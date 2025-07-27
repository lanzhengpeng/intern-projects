### 【2003】report associativity data 报告关联性数据

#### 代码

```cpp
    static void report_associativity_data(tag_t dim)  
    {  
        int  
            ii,  
            n_assoc;  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        RX(dim);  
        UF_CALL(UF_DRF_ask_associativity_data(dim, &n_assoc, &assoc_data));  
        WRITE_D(n_assoc);  
        for (ii = 0; ii < n_assoc; ii++)  
        {  
            WRITE_D(ii);  
            RX(assoc_data[ii].assoc_object_1);  
            RX(assoc_data[ii].assoc_object_2);  
            RX(assoc_data[ii].object_view);  
            WRITE_D(assoc_data[ii].point_type);  
            WRITE_D(assoc_data[ii].line_type);  
            WRITE3F(assoc_data[ii].base_pt_1);  
            WRITE3F(assoc_data[ii].base_pt_2);  
            WRITE_F(assoc_data[ii].entered_angle);  
            WRITE3F(assoc_data[ii].assoc_point);  
        }  
        if (n_assoc > 0) UF_free(assoc_data);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是报告NX中某维度(dim)的关联数据。
>
> 具体来说，它执行以下步骤：
>
> 1. 定义一个结构体来存储关联数据，包括关联的两个对象、视图、点线类型、基点、输入角度和关联点。
> 2. 调用UF_DRF_ask_associativity_data函数，获取指定维度(dim)的关联数据，包括关联对象的数量和关联数据数组。
> 3. 输出关联对象的数量。
> 4. 遍历关联数据数组，输出每个关联对象的详细信息，包括关联的两个对象、视图、点线类型、基点、输入角度和关联点。
> 5. 如果关联对象数量大于0，释放关联数据数组。
> 6. 使用宏RX来接收关联对象，宏WRITE_D/WRITE_F/WRITE3F来输出关联数据的详细内容。
>
> 总体来说，这段代码的功能是查询并报告NX中某维度(dim)的关联数据，包括关联对象及其详细信息。
>
