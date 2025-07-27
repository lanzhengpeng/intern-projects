### 【1610】map prototypes and xforms to occurrences 将原型和变换映射到实例

#### 代码

```cpp
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
    static tag_t get_work_occurrence()  
    {  
        tag_t  
            work_occ = UF_ASSEM_ask_work_occurrence();  
        if (work_occ == NULL_TAG)  
            work_occ = UF_ASSEM_ask_root_part_occ(UF_ASSEM_ask_work_part());  
        return work_occ;  
    }  
    static void get_wave_source_and_xform(tag_t object, tag_t *source, tag_t *xform)  
    {  
        tag_t  
            disp_occ,  
            work_occ = get_work_occurrence(),  
            wp = UF_ASSEM_ask_work_part(),  
            wp_obj = UF_OBJ_cycle_all(wp, NULL_TAG);  
        if (UF_ASSEM_is_occurrence(object))  
            *source = UF_ASSEM_ask_prototype_of_occ(object);  
        else  
            *source = object;  
        if (UF_OBJ_is_object_a_promotion(*source))  
            disp_occ = ask_part_occ_of_prom_occ(object);  
        else  
            disp_occ = UF_ASSEM_ask_part_occurrence(object);  
        if ((disp_occ == NULL_TAG) || (disp_occ == work_occ))  
            *xform = NULL_TAG;  
        else  
            UF_CALL(UF_SO_create_xform_assy_ctxt(wp_obj, disp_occ, work_occ,  
                xform));  
    }  
    static void map_prototypes_and_xforms_to_occurrences(int *n, tag_t *protos,  
        tag_t *xforms, tag_t *occs)  
    {  
        logical  
            found;  
        int  
            ii, jj;  
        tag_t  
            dp = UF_PART_ask_display_part(),  
            disp_occ,  
            owner,  
            work_occ = get_work_occurrence(),  
            wp = UF_ASSEM_ask_work_part();  
        for (ii = 0; ii < *n; ii++)  
        {  
            found = FALSE;  
            UF_CALL(UF_OBJ_ask_owning_part(protos[ii], &owner));  
            if (owner == wp)  
            {  
                occs[ii] = UF_ASSEM_find_occurrence(work_occ, protos[ii]);  
                if (occs[ii] == NULL_TAG) occs[ii] = protos[ii];  
                xforms[ii] = NULL_TAG;  
                found = TRUE;  
            }  
            if (!found && (owner == dp))  
            {  
                occs[ii] = protos[ii];  
                get_wave_source_and_xform(occs[ii], &protos[ii],  
                    &xforms[ii]);  
                found = TRUE;  
            }  
            if (!found)  
            {  
                if (xforms[ii] == NULL_TAG)  
                {  
                    occs[ii] = UF_ASSEM_find_occurrence(work_occ, protos[ii]);  
                    if (occs[ii] != NULL_TAG) found = TRUE;  
                }  
                else  
                {  
                    UF_CALL(UF_SO_ask_assy_ctxt_part_occ(xforms[ii], work_occ,  
                        &disp_occ));  
                    if (disp_occ != NULL_TAG)  
                    {  
                        occs[ii] = UF_ASSEM_find_occurrence(disp_occ, protos[ii]);  
                        if (occs[ii] != NULL_TAG) found = TRUE;  
                    }  
                }  
            }  
            if (!found)  
            {  
                for (jj = ii; jj < *n-1; jj++)  
                {  
                    protos[jj] = protos[jj+1];  
                    xforms[jj] = xforms[jj+1];  
                }  
                *n = *n - 1;  
                ii--;  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统进行二次开发的一段代码，其主要功能是获取组件的显示实例和变换矩阵，以映射原型组件和显示实例之间的关系。具体来说，代码包括以下关键部分：
>
> 1. ask_part_occ_of_prom_occ函数：获取促销组件的拥有组件的显示实例。
> 2. get_work_occurrence函数：获取工作组件的显示实例。
> 3. get_wave_source_and_xform函数：获取组件的工作实例和变换矩阵。
> 4. map_prototypes_and_xforms_to_occurrences函数：映射原型组件和显示实例，并获取变换矩阵。该函数首先判断原型组件是否在工作组件中，如果是则直接使用原型组件作为显示实例；其次判断是否在显示组件中，如果是则获取对应的显示实例和变换矩阵；最后遍历查找原型组件在工作组件或显示组件中的显示实例，如果没有找到则从数组中移除。
>
> 整体来看，这段代码实现了在NX CAD系统中获取组件的显示实例和变换矩阵，为后续的显示和操作提供了支持。
>
