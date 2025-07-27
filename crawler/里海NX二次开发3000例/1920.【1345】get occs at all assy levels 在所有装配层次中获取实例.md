### 【1345】get occs at all assy levels 在所有装配层次中获取实例

#### 代码

```cpp
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
    static int get_object_instance_map(tag_t object_occ, tag_t **instances)  
    {  
        tag_t  
            instance,  
            part_occ;  
        uf_list_p_t  
            instance_list;  
        UF_CALL(UF_MODL_create_list(&instance_list));  
    /*  Walk up the assy tree and put each parent instance in list for map 译:翻译为：沿着装配树向上走，并将每个父实例放入列表以映射。 */  
        UF_CALL(UF_ASSEM_ask_parent_component(object_occ, &part_occ));  
        while (part_occ != NULL_TAG)  
        {  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
            if (instance != NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(instance_list, instance));  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
        }  
        return make_an_array(&instance_list, instances);  
    }  
    static int find_object_at_all_assy_levels(tag_t object_occ, tag_t **occs)  
    {  
        int  
            ii, jj,  
            n_instances;  
        tag_t  
            *instances,  
            object_proto,  
            part_occ;  
        uf_list_p_t  
            occ_list;  
        *occs = NULL;  
        if (!UF_ASSEM_is_occurrence(object_occ)  
            || UF_ASSEM_is_part_occurrence(object_occ)) return 0;  
        UF_CALL(UF_MODL_create_list(&occ_list));  
        UF_CALL(UF_MODL_put_list_item(occ_list, object_occ));  
        n_instances = get_object_instance_map(object_occ, &instances);  
        object_proto = UF_ASSEM_ask_prototype_of_occ(object_occ);  
    /*  Walk to the bottom of the same instance branch at each assy level 译:在各个装配级别，沿着相同的实例分支走到最底层。 */  
        for (ii = n_instances - 1; ii > 0; ii--)  
        {  
            part_occ = UF_ASSEM_ask_root_part_occ(  
                UF_ASSEM_ask_child_of_instance(instances[ii]));  
            for (jj = ii - 1; jj >= 0; jj--)  
                part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, instances[jj]);  
            object_occ = UF_ASSEM_find_occurrence(part_occ, object_proto);  
            UF_CALL(UF_MODL_put_list_item(occ_list, object_occ));  
        }  
        UF_CALL(UF_MODL_put_list_item(occ_list, object_proto));  
        if (n_instances > 0) UF_free(instances);  
        return make_an_array(&occ_list, occs);  
    }  
    static int get_component_instance_map(tag_t part_occ, tag_t **instances)  
    {  
        tag_t  
            instance;  
        uf_list_p_t  
            instance_list;  
        UF_CALL(UF_MODL_create_list(&instance_list));  
    /*  Walk up the assy tree and put each parent instance in list for map 译:翻译为：沿着装配树向上爬，并将每个父实例放入列表中用于映射。 */  
        while (part_occ != NULL_TAG)  
        {  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
            if (instance != NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(instance_list, instance));  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
        }  
        return make_an_array(&instance_list, instances);  
    }  
    static int find_component_at_all_assy_levels(tag_t part_occ, tag_t **occs)  
    {  
        int  
            ii, jj,  
            n_instances;  
        tag_t  
            *instances;  
        uf_list_p_t  
            occ_list;  
        *occs = NULL;  
        if (!UF_ASSEM_is_part_occurrence(part_occ)) return 0;  
        UF_CALL(UF_MODL_create_list(&occ_list));  
        UF_CALL(UF_MODL_put_list_item(occ_list, part_occ));  
        n_instances = get_component_instance_map(part_occ, &instances);  
    /*  Walk to the bottom of the same instance branch at each assy level 译:在每个装配级别，沿着同一实例分支向下走。 */  
        for (ii = n_instances - 1; ii > 0; ii--)  
        {  
            part_occ = UF_ASSEM_ask_root_part_occ(  
                UF_ASSEM_ask_child_of_instance(instances[ii]));  
            for (jj = ii - 1; jj >= 0; jj--)  
                part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, instances[jj]);  
            UF_CALL(UF_MODL_put_list_item(occ_list, part_occ));  
        }  
        if (n_instances > 0) UF_free(instances);  
        return make_an_array(&occ_list, occs);  
    }  
    static int get_occs_at_all_assy_levels(tag_t object, tag_t **occs)  
    {  
        *occs = NULL;  
        if (UF_ASSEM_is_part_occurrence(object))  
            return find_component_at_all_assy_levels(object, occs);  
        if (UF_ASSEM_is_occurrence(object))  
            return find_object_at_all_assy_levels(object, occs);  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于实现获取NX模型中零件在所有装配层次下的实例的功能。
>
> 主要功能包括：
>
> 1. 获取对象实例映射：通过递归遍历父组件，获取指定对象在各个装配层次下的实例，并将它们存储在一个列表中。
> 2. 获取组件实例映射：类似地，此函数用于获取指定零件在各个装配层次下的实例，并将它们存储在一个列表中。
> 3. 在所有装配层次下查找对象：通过实例映射列表，递归查找同一实例分支下的对象实例，并存储在一个列表中。
> 4. 在所有装配层次下查找组件：与查找对象类似，递归查找同一实例分支下的零件实例，并存储在一个列表中。
> 5. 获取所有装配层次下的实例：根据输入对象类型，调用不同的函数来获取零件或对象在所有装配层次下的实例。
>
> 总体来说，这段代码通过递归遍历装配结构，实现了在NX模型中获取零件或对象在各个装配层次下的实例的功能。这对于需要在不同装配层次下操作或查询同一零件的不同实例的应用场景非常有用。
>
