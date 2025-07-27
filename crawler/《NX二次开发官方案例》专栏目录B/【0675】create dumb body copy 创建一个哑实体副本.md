### 【0675】create dumb body copy 创建一个哑实体副本

#### 代码

```cpp
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static tag_t ask_part_occ_of_prom_occ(tag_t prom_object)  
    {  
        tag_t  
            owning_part,  
            part_occ = prom_object,  
            proto;  
        proto = UF_ASSEM_ask_prototype_of_occ(prom_object);  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        while (!UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ))  
            && (UF_ASSEM_ask_prototype_of_occ(part_occ) != owning_part));  
        return part_occ;  
    }  
    static tag_t create_dumb_body_copy(tag_t body)  
    {  
        tag_t  
            feat,  
            part_occ,  
            proto,  
            xform,  
            dumb_body;  
        uf_list_p_t  
            list;  
        if (UF_ASSEM_is_occurrence(body))  
            proto = UF_ASSEM_ask_prototype_of_occ(body);  
        else  
            proto = body;  
        if (UF_OBJ_is_object_a_promotion(proto))  
            part_occ = ask_part_occ_of_prom_occ(body);  
        else  
            part_occ = UF_ASSEM_ask_part_occurrence(body);  
        if (part_occ == NULL_TAG)  
            UF_CALL(UF_WAVE_create_linked_body(proto,NULL_TAG,body,FALSE,&feat));  
        else  
        {  
            UF_CALL(UF_SO_create_xform_assy_ctxt(body, part_occ, NULL_TAG, &xform));  
            UF_CALL(UF_WAVE_create_linked_body(proto, xform, body, FALSE, &feat));  
        }  
        UF_CALL(UF_MODL_ask_feat_body(feat, &dumb_body));  
        if (dumb_body != NULL_TAG)  
        {  
            list = make_a_list(1, &dumb_body);  
            UF_CALL(UF_MODL_delete_object_parms(list));  
            UF_CALL(UF_MODL_delete_list(&list));  
        }  
        return dumb_body;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要用于创建一个“哑”的body副本。其关键功能包括：
>
> 1. make_a_list函数：创建一个列表并添加指定数量的item。
> 2. ask_part_occ_of_prom_occ函数：获取与给定promotion相关的part_occurrence。
> 3. create_dumb_body_copy函数：创建一个body的“哑”副本，即复制body但不复制其特征。如果body是promotion，会获取其相关的part_occurrence。然后创建一个与原始body平行的linked_body，并从linked_body中提取出body作为“哑”副本。最后，删除linked_body及其特征，只保留“哑”副本。
>
> 总体来说，这段代码的目的是为了创建一个不包含特征的body副本，以便进行进一步的操作或修改。
>
