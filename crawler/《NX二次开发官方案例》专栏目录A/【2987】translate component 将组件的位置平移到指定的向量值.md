### 【2987】translate component 将组件的位置平移到指定的向量值

#### 代码

```cpp
    static void translate_component(tag_t comp, double vec[3])  
    {  
        tag_t  
            instance;  
        double  
            instance_origin[3],  
            instance_matrix[9],  
            instance_transform[4][4];  
        char  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        instance = UF_ASSEM_ask_inst_of_part_occ(comp);  
        UF_CALL( UF_ASSEM_ask_component_data( instance, part_name, refset_name,  
            instance_name, instance_origin, instance_matrix, instance_transform));  
        UF_VEC3_add(instance_origin, vec, instance_origin);  
        UF_CALL( UF_ASSEM_reposition_instance(instance, instance_origin,  
            instance_matrix));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其主要功能是将指定组件（comp）的实例（instance）沿着指定的向量（vec）进行平移。代码的主要步骤包括：
>
> 1. 获取组件的实例（instance），即该组件在装配中的实例。
> 2. 获取实例的原始位置（instance_origin）、变换矩阵（instance_matrix）和4x4变换矩阵（instance_transform）。
> 3. 将向量vec加到实例的原始位置上，得到新的位置。
> 4. 使用新的位置和原始的变换矩阵重新设置实例的位置。
> 5. 实例的平移完成。
>
> 通过这些步骤，实现了对指定组件实例的平移操作。
>
