### 【1472】is feature rolled back feature被回滚了吗

#### 代码

```cpp
    /* This function compares the timestamps of the two features feat1 and feat2.  
       It returns an integer less than, equal to, or greater than zero if the timestamp  
       of feat1 is found, respectively, to be earlier than, to match, or be later than  
       the timestamp of feat2.  (It works like strcmp.) */  
    static int timestamp_compare(tag_t feat1, tag_t feat2)  
    {  
        int  
            ts1, ts2;  
        UF_CALL(UF_MODL_ask_time_stamp_of_feature(feat1, &ts1));  
        UF_CALL(UF_MODL_ask_time_stamp_of_feature(feat2, &ts2));  
        return (ts1 - ts2);  
    }  
    static is_feature_rolled_back(tag_t feature)  
    {  
        tag_t  
            current;  
        UF_CALL(UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &current));  
        return (timestamp_compare(feature, current) > 0);  
    }

```

#### 代码解析

> 这段代码实现了两个功能：
>
> 1. timestamp_compare函数：用于比较两个特征(feature)的时间戳。它通过UF_MODL_ask_time_stamp_of_feature函数获取两个特征的时间戳，然后比较大小，返回-1、0或1，表示第一个特征的时间戳早于、等于或晚于第二个特征。
> 2. is_feature_rolled_back函数：用于判断一个特征是否被回滚。它首先通过UF_ASSEM_ask_work_part获取当前工作部件，然后通过UF_MODL_ask_current_feature获取当前工作部件的最新特征。接着调用timestamp_compare函数，判断指定特征的时间戳是否晚于当前工作部件的最新特征的时间戳。如果晚于，则表示该特征被回滚，函数返回1；否则返回0。
>
> 这两个函数主要用于NX的二次开发，通过比较特征的时间戳来控制特征的创建和回滚等操作。
>
