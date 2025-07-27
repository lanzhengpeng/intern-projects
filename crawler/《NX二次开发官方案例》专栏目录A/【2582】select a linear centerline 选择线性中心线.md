### 【2582】select a linear centerline 选择线性中心线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_linear_centerlines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_linear_cntrln_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_centerline(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a Linear Centerline", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_centerlines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这是NX Open C++的二次开发代码，主要用于选择线性中心线。代码主要包含以下部分：
>
> 1. mask_for_linear_centerlines函数：用于设置选择掩码，只允许选择线性中心线。
> 2. select_a_linear_centerline函数：用于显示选择对话框，让用户选择一个线性中心线。该函数首先调用mask_for_linear_centerlines来设置选择掩码，然后使用UF_UI_select_with_single_dialog函数显示选择对话框，用户选择后返回线性中心线的tag。
> 3. 使用UF_DISP_set_highlight来高亮显示用户选择的线性中心线。
> 4. 函数返回用户选择的线性中心线的tag，如果用户取消选择，则返回NULL_TAG。
>
> 这段代码的目的是为了在NX的二次开发应用程序中，通过选择对话框让用户能够方便地选择所需的线性中心线，并返回其tag以便后续处理。
>
