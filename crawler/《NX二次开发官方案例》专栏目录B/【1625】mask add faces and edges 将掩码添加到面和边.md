### 【1625】mask add faces and edges 将掩码添加到面和边

#### 代码

```cpp
    /* 里海 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }

```

#### 代码解析

> 这是NX二次开发中设置选择掩码的示例代码，其主要功能是允许用户在选择特征时仅选择实体类型为UF_solid_type的面和边。
>
> 代码的主要步骤包括：
>
> 1. 定义了一个包含两个UF_UI_mask_t结构体的数组，每个结构体用于定义一个选择掩码。掩码中的第一个元素指定选择类型为UF_solid_type，第二个元素设置为0，表示不区分子类型，第三个元素指定选择的对象类型，分别是UF_UI_SEL_FEATURE_ANY_FACE和UF_UI_SEL_FEATURE_ANY_EDGE，表示选择任意面和任意边。
> 2. 调用UF_UI_set_sel_mask函数，将定义的选择掩码应用到指定的选择集select上。函数的第二个参数设置为UF_UI_SEL_MASK_ENABLE_SPECIFIC，表示启用特定的选择掩码。第三个参数指定选择掩码的个数，这里是2。第四个参数传递定义的选择掩码数组。
> 3. 检查UF_CALL宏的返回值。如果返回0，表示调用成功，函数返回UF_UI_SEL_SUCCESS。如果返回非0，表示调用失败，函数返回UF_UI_SEL_FAILURE。
> 4. 该函数的作用是，在NX中定义了一个选择集，并设置了选择掩码，允许用户仅选择实体类型为UF_solid_type的面和边，从而实现了对用户选择范围的限制。
>
