### 【2573】select a gdt 选择一个几何数据类型

#### 代码

```cpp
    /* 里海 */  
    static int mask_gdt(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            triples = { UF_tol_feature_instance_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &triples)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_gdt(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a GDT", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_gdt, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，主要功能是选择一个几何数据类型(GDT)的特征实例，并返回其tag。
>
> 主要步骤如下：
>
> 1. 定义了一个mask_gdt函数，用于设置选择过滤条件，仅选择几何数据类型为特征实例的类型。
> 2. 定义了一个select_a_gdt函数，用于使用对话框让用户选择一个特征实例。
> 3. 在select_a_gdt中，使用UF_UI_select_with_single_dialog函数显示对话框，让用户选择一个特征实例。
> 4. 如果用户成功选择了一个特征实例，则取消高亮，并返回该特征实例的tag。
> 5. 如果用户没有选择或选择失败，则返回NULL_TAG。
>
> 这段代码实现了简单的特征实例选择功能，为后续处理提供了基础。
>
