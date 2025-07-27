### 【1352】get wave source and xform 获取波源和变换

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

```

#### 代码解析

> 这段代码是用于NX CAD系统的二次开发代码，主要功能包括：
>
> 1. ask_part_occ_of_prom_occ函数：获取给定推广对象的部件实例。首先获取推广对象的原始对象，然后不断获取上级部件实例，直到达到拥有该推广对象的部件实例为止。
> 2. get_work_occurrence函数：获取当前工作部件实例。首先尝试获取工作部件实例，如果不存在，则获取工作部件的根部件实例。
> 3. get_wave_source_and_xform函数：获取给定对象在工作部件中的源对象和变换。首先判断对象是否是推广，如果是则获取推广的源对象，否则直接获取对象本身。然后获取源对象在工作部件中的实例。如果实例不存在或就是工作部件实例，则变换为空。否则，创建源对象在工作部件实例和工作部件实例之间的变换。
> 4. 这些函数提供了获取对象在工作部件中的源对象和变换的通用方法，为后续的装配波操作提供了基础。
>
