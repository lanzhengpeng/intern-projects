### 【2126】report feature type 报告特征类型

#### 代码

```cpp
    static void report_feature_type(tag_t feature)  
    {  
        char  
            *type;  
        WRITE_D(feature);  
        UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
        WRITE_S(type);  
        UF_free(type);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中的函数，用于报告NX特征类型的示例代码。
>
> 代码的主要功能如下：
>
> 1. 函数名为report_feature_type，接受一个NX特征标签作为参数。
> 2. 使用WRITE_D宏输出特征标签的值。
> 3. 调用UF_MODL_ask_feat_type函数，查询特征标签对应的特征类型字符串，并存储在type指针所指向的内存中。
> 4. 使用WRITE_S宏输出特征类型字符串。
> 5. 调用UF_free释放type指针指向的内存。
> 6. 该函数通过查询特征类型并输出，实现了报告特征类型的功能。
> 7. 使用WRITE_D和WRITE_S宏是为了方便在不同平台上输出数据。
> 8. 注意释放type指针指向的内存，避免内存泄漏。
> 9. 该函数是一个静态函数，可以直接调用，不需要创建对象。
> 10. 通过此代码，我们可以了解如何使用NX Open C++ API来查询和输出特征类型信息。
>
