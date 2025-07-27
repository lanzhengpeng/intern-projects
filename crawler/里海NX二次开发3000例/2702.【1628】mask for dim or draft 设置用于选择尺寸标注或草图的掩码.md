### 【1628】mask for dim or draft 设置用于选择尺寸标注或草图的掩码

#### 代码

```cpp
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 },  
                        { UF_tabular_note_type, UF_all_subtype, 0 },  
                        { UF_tol_feature_instance_type, 0, 0} };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }

```

#### 代码解析

> 这是NX二次开发中的一段代码，其功能是设置选择集的选择过滤条件。
>
> 代码的核心功能是通过调用UF_UI_set_sel_mask函数来设置选择集的选择过滤条件。该函数的第一个参数是选择集对象，第二个参数是设置模式，这里使用UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC表示清除当前过滤条件并设置指定的过滤条件。第三个参数指定了过滤条件的个数，这里是4个。第四个参数是一个数组，每个元素定义了一个过滤条件，包含类型、子类型和标记。
>
> 数组中的4个过滤条件分别表示：
>
> 1. UF_drafting_entity_type: 表示过滤条件为绘图实体类型
> 2. UF_dimension_type: 表示过滤条件为尺寸标注类型
> 3. UF_tabular_note_type: 表示过滤条件为表格注释类型
> 4. UF_tol_feature_instance_type: 表示过滤条件为公差特征实例类型
>
> 通过设置这些过滤条件，选择集只能选择符合这些类型和子类型的对象。如果调用UF_UI_set_sel_mask成功，则函数返回UF_UI_SEL_SUCCESS；如果失败，则返回UF_UI_SEL_FAILURE。
>
