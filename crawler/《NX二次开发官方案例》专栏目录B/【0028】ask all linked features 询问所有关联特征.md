### 【0028】ask all linked features 询问所有关联特征

#### 代码

```cpp
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    #include <uf_kf.h>  
    static logical is_linked_feature(tag_t feature)  
    {  
        logical  
            is_init = false,  
            is_linked = false;  
        char  
            rule[133],  
            unique[UF_CFI_MAX_FILE_NAME_SIZE],  
            *value;  
        UF_CALL(UF_KF_is_initialized(&is_init));  
        if (!is_init)  
            UF_CALL(UF_KF_init_part(UF_ASSEM_ask_work_part()));  
        UF_CALL(uc4577(unique));  
        sprintf(rule, "mqc_isLinkedFeature(%d)", feature);  
        UF_CALL(UF_KF_create_rule_no_update("root:", unique, "Boolean", rule, ""));  
        sprintf(rule, "root:%s:", unique);  
        UF_CALL(UF_KF_evaluate_rule_to_string(rule, &value));  
        UF_CALL(UF_KF_delete_instance_rule("root:", unique));  
        if (strcmp(value, "TRUE") == 0) is_linked = true;  
        UF_free(value);  
        return is_linked;  
    }  
    static int ask_all_linked_features(tag_t part, tag_p_t *feats)  
    {  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
            if (is_linked_feature(feat))  
                UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
        return (make_an_array(&feat_list, feats));  
    }

```

#### 代码解析

> 这段代码是NX Open API的C++实现，主要实现了以下几个功能：
>
> 1. ask_next_feature函数：用于遍历指定部件的所有特征，并返回下一个存活的特征。
> 2. allocate_memory函数：用于分配指定大小的内存，并返回内存指针。
> 3. make_an_array函数：从列表中提取所有对象，将它们放入数组中，然后删除列表。
> 4. is_linked_feature函数：使用知识融合（Knowledge Fusion）功能来检查特征是否为链接特征。
> 5. ask_all_linked_features函数：遍历指定部件的所有特征，并返回一个数组，其中包含所有链接特征。
>
> 总体来说，这段代码提供了遍历部件特征、分配内存、将列表转换为数组以及使用知识融合检查特征类型等基础功能，为NX二次开发提供了便利。
>
