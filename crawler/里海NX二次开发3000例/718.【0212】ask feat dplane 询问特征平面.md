### 【0212】ask feat dplane 询问特征平面

#### 代码

```cpp
    static tag_t ask_next_dplane(tag_t part, tag_t dplane)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_datum_plane_type, &dplane));  
        return (dplane);  
    }  
    static tag_t ask_feat_dplane(tag_t feat)  
    {  
        tag_t  
            dfeat,  
            dplane = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((dplane = ask_next_dplane(part, dplane)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_object_feat(dplane, &dfeat));  
            if (dfeat == feat) return dplane;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API进行二次开发的示例代码，其主要功能是遍历当前显示部件中的所有基准平面，并找出与指定特征关联的基准平面。
>
> 代码中定义了两个函数：
>
> 1. ask_next_dplane：该函数接受当前部件和基准平面tag作为参数，使用UF_OBJ_cycle_objs_in_part函数遍历当前部件中的所有基准平面，并返回下一个基准平面的tag。当没有更多基准平面时，返回NULL_TAG。
> 2. ask_feat_dplane：该函数接受一个特征tag作为参数。首先使用UF_PART_ask_display_part获取当前显示的部件。然后循环调用ask_next_dplane，获取当前部件中的所有基准平面。对于每个基准平面，使用UF_MODL_ask_object_feat获取与其关联的特征tag。如果找到与参数特征tag匹配的基准平面，则返回该基准平面的tag。如果遍历完所有基准平面都没有找到匹配的特征，则返回NULL_TAG。
>
> 总体来说，这段代码通过遍历部件中的基准平面，并检查每个基准平面关联的特征，实现了根据特征tag查询与之关联的基准平面的功能。
>
