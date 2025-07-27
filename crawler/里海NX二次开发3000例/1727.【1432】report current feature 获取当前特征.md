### 【1432】report current feature 获取当前特征

#### 代码

```cpp
    static void report_current_feature(void)  
    {  
        tag_t  
            feature;  
        char  
            *sysname;  
        UF_CALL(UF_MODL_ask_current_feature(&feature));  
        UF_CALL(UF_MODL_ask_feat_sysname(feature, &sysname));  
        ECHO("Current feature is %s\n", sysname);  
        UF_free(sysname);  
    }

```

#### 代码解析

> 这段NX Open C++代码用于报告当前选中的特征。代码首先调用UF_MODL_ask_current_feature函数来获取当前选中特征的tag，然后使用UF_MODL_ask_feat_sysname函数获取该特征的对象名称。最后，代码输出当前选中特征的名称，并释放使用UF_free函数分配的内存。整体而言，这段代码实现了获取当前选中特征名称的功能。
>
