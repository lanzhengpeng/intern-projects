### 【2090】report face features 报告面特征

#### 代码

```cpp
    static void report_face_features(tag_t face)  
    {  
        int  
            ii,  
            count;  
        uf_list_p_t  
            list;  
        char  
            *feat_name,  
            *feat_type;  
        tag_t  
            feat;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_MODL_ask_face_feats(face, &list));  
        UF_CALL(UF_MODL_ask_list_count(list, &count));  
        sprintf(msg, "Face (%d) has %d features\n", face, count);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for (ii = 0; ii < count; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(list, ii, &feat));  
            UF_CALL(UF_MODL_ask_feat_name(feat, &feat_name));  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            sprintf(msg, "  %d.  %s (%d - %s)\n", ii+1, feat_name, feat, feat_type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(feat_type);  
            UF_free(feat_name);  
        }  
        UF_CALL(UF_MODL_delete_list(&list));  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API进行NX二次开发的一个示例，其主要功能是报告NX中一个面(face)的所有特征(features)。
>
> 代码的主要逻辑包括：
>
> 1. 获取面(face)的特征列表(list)。
> 2. 获取特征列表的长度(count)。
> 3. 打印面(face)及其特征数量的信息。
> 4. 遍历特征列表，打印每个特征的信息，包括特征名称(feat_name)、特征类型(feat_type)等。
> 5. 释放特征名称和特征类型字符串的内存。
> 6. 删除特征列表。
>
> 这段代码展示了如何使用NX Open C++ API获取NX模型中的特征信息，并通过UF_UI函数在NX的Listing窗口中打印输出，为NX二次开发提供了一个基础示例。
>
