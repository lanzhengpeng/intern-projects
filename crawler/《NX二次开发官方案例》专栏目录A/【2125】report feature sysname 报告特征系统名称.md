### 【2125】report feature sysname 报告特征系统名称

#### 代码

```cpp
    static void report_feature_sysname(tag_t feature)  
    {  
        char  
            *sysname;  
        UF_CALL(UF_MODL_ask_feat_sysname(feature, &sysname));  
        ECHO("%s\n", sysname);  
        UF_free(sysname);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，用于获取NX特征对象在NX系统中的名称。
>
> 主要功能包括：
>
> 1. 定义了一个名为report_feature_sysname的静态函数，用于报告NX特征对象的系统名称。
> 2. 函数接收一个NX特征对象feature作为参数。
> 3. 使用UF_MODL_ask_feat_sysname函数查询特征对象的系统名称，结果保存在sysname指针指向的字符串中。
> 4. 输出sysname字符串，显示特征对象的系统名称。
> 5. 使用UF_free函数释放sysname字符串的内存。
>
> 这段代码实现了查询NX特征对象系统名称并打印输出的功能，通过调用NX Open C++ API提供的函数来实现，展示了NX二次开发的基本流程。
>
