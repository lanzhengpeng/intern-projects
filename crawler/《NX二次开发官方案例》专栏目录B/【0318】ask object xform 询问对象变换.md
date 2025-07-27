### 【0318】ask object xform 询问对象变换

#### 代码

```cpp
    static tag_t ask_object_xform(tag_t object)  
    {  
        tag_t  
            base_tag,  
            part_occ,  
            prom_part,  
            part_proto,  
            proto,  
            xform = NULL_TAG;  
        if (!UF_ASSEM_is_occurrence(object)) return xform;  
        proto = UF_ASSEM_ask_prototype_of_occ(object);  
        if (UF_OBJ_is_object_a_promotion(proto))  
        {  
            base_tag = proto;  
            while (base_tag != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_prom_map_object_down(proto, &base_tag));  
                if (base_tag != NULL_TAG) proto = base_tag;  
            }  
            UF_CALL(UF_OBJ_ask_owning_part(proto, &prom_part));  
            part_occ = UF_ASSEM_ask_part_occurrence(object);  
            part_proto = UF_ASSEM_ask_prototype_of_occ(part_occ);  
            while (prom_part != part_proto)  
            {  
                UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
                part_proto = UF_ASSEM_ask_prototype_of_occ(part_occ);  
            }  
            UF_CALL(UF_SO_create_xform_assy_ctxt(object, part_occ, NULL_TAG,  
                &xform));  
        }  
        else  
            UF_CALL(UF_SO_create_xform_assy_ctxt(object,  
                UF_ASSEM_ask_part_occurrence(object), NULL_TAG, &xform));  
        return xform;  
    }

```

#### 代码解析

> 这段代码的主要功能是获取NX装配体中组件的变换矩阵。以下是对代码的详细解释：
>
> 1. 定义了几个tag_t类型的变量，用于存储组件的基类标签、零件出现、原型零件、零件原型、组件原型以及变换矩阵等。
> 2. 判断输入的对象是否为出现，如果不是，直接返回NULL_TAG。
> 3. 获取输入对象的组件原型。
> 4. 判断组件原型是否为提升零件，如果是，则遍历提升链，找到基类组件，并获取其拥有零件。
> 5. 获取输入对象对应的零件出现和零件原型。
> 6. 通过遍历父组件，找到提升零件对应的零件原型。
> 7. 使用零件出现和提升零件对应的零件原型，创建变换矩阵。
> 8. 如果组件原型不是提升零件，直接使用组件出现和组件原型创建变换矩阵。
> 9. 返回创建的变换矩阵。
>
> 这段代码主要用于处理提升零件，通过遍历提升链找到基类组件，并基于基类组件和零件出现创建变换矩阵，以获取组件在装配体中的实际位置和方向。
>
