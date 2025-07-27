### 【2108】report feature array 报告特征数组

#### 代码

```cpp
    #define WRITEnFn(n,X)(report_feature_array(#X, X, n))  
    static void report_feature_array(char *array_name, tag_t *feats, int n_feats)  
    {  
        int  
            ii;  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        UF_UI_open_listing_window();  
        for (ii = 0; ii < n_feats; ii++)  
        {  
            if (!UF_CALL(UF_MODL_ask_feat_name(feats[ii], &name)) &&  
                !UF_CALL(UF_MODL_ask_feat_type(feats[ii], &type)))  
            {  
                sprintf(msg, "%s[%d] (%d) = %s (%s)\n", array_name, ii, feats[ii],  
                    name, type);  
                UF_UI_write_listing_window(msg);  
                UF_free(name);  
                UF_free(type);  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，其主要功能是遍历并打印出NX特征数组中的每个特征名称和类型。
>
> 代码首先定义了一个宏WRITEnFn，用于调用report_feature_array函数，并传入特征数组和特征数量作为参数。
>
> report_feature_array函数用于遍历特征数组，并调用NX API获取每个特征的特征名称和类型，然后格式化成字符串，并输出到NX的日志窗口中。
>
> 在遍历过程中，使用NX API UF_MODL_ask_feat_name和UF_MODL_ask_feat_type分别获取特征的名称和类型，并使用sprintf函数格式化成字符串，然后调用NX API UF_UI_write_listing_window输出到NX的日志窗口。
>
> 需要注意的是，获取的特征名称和类型是动态内存，需要调用UF_free进行释放。
>
> 总体而言，这段代码实现了遍历特征数组，并打印出每个特征名称和类型的功能，用于调试NX的特征数据。
>
