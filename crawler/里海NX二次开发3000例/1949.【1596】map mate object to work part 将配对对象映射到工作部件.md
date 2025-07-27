### 【1596】map mate object to work part 将配对对象映射到工作部件

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
    static tag_t map_mate_object_to_work_part(tag_t face, tag_t *top_instance)  
    {  
        int  
            ii,  
            n_instances;  
        tag_t  
            part_occ,  
            instance,  
            *instances,  
            work_part,  
            work_occ;  
        uf_list_p_t  
            instance_list;  
        *top_instance = NULL_TAG;  
        part_occ = UF_ASSEM_ask_part_occurrence(face);  
        if (part_occ != NULL_TAG)  
            *top_instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
        work_occ = UF_ASSEM_ask_work_occurrence();  
        if (part_occ == work_occ) return NULL_TAG;  
    /*  Walk up the displayed part to the work occurrence keeping track of  
        instances along the way */  
        UF_CALL(UF_MODL_create_list(&instance_list));  
        while (part_occ != work_occ)  
        {  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
            if (instance != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_put_list_item(instance_list, instance));  
                *top_instance = instance;  
            }  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
        }  
        n_instances = make_an_array(&instance_list, &instances);  
    /*  Walk down the same instance tree in the root occurrence of the work part 译:在根出现的工作部分中，沿着相同的实例树向下走。 */  
        work_part = UF_ASSEM_ask_work_part();  
        part_occ = UF_ASSEM_ask_root_part_occ(work_part);  
        for (ii = n_instances; ii; ii--)  
            part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, instances[ii-1]);  
        UF_free(instances);  
        return part_occ;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 内存分配：allocate_memory函数用于申请指定大小的内存，并返回内存地址。
> 2. 创建数组：make_an_array函数用于从对象列表创建一个数组，并释放对象列表。
> 3. 映射装配体中的面到工作部件：map_mate_object_to_work_part函数用于映射装配体中的面到工作部件。它首先获取面的部件实例，然后通过实例链找到工作部件，并返回工作部件中的面对应的部件实例。如果面在根部件中，则返回NULL。
> 4. 关键API函数：代码中使用了NX的装配建模相关API函数，如UF_ASSEM_ask_part_occurrence、UF_ASSEM_ask_inst_of_part_occ等，用于获取面所在部件实例、实例的上级实例等。
> 5. 实例链的构建与遍历：通过构建实例链，并沿着实例链向下遍历，最终找到工作部件中对应的面。
> 6. 释放内存：代码最后释放了申请的实例数组内存。
>
> 综上所述，这段代码实现了从装配体中的面对象映射到工作部件中的面对象的功能，展示了NX装配建模相关API的典型使用方法。
>
