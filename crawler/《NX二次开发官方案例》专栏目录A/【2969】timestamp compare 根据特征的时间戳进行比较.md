### 【2969】timestamp compare 根据特征的时间戳进行比较

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

```

#### 代码解析

> 这是段NX Open C++ API开发的代码，其主要功能是定义了一个名为timestamp_compare的静态函数，用于比较两个特征feat1和feat2的时间戳。该函数首先通过调用UF_MODL_ask_time_stamp_of_feature函数来获取两个特征的时间戳ts1和ts2。然后，函数通过返回ts1 - ts2的值，来表示feat1的时间戳与feat2的时间戳的比较结果。如果ts1 < ts2，则返回负值，表示feat1的时间戳早于feat2；如果ts1 == ts2，则返回0，表示两个特征时间戳相等；如果ts1 > ts2，则返回正值，表示feat1的时间戳晚于feat2。因此，该函数实现了类似于strcmp的功能，用于比较两个特征的时间戳大小。
>
