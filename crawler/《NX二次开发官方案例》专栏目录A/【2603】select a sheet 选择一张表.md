### 【2603】select a sheet 选择一张表

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_sheet_bodies( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_SHEET_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_sheet(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_sheet_bodies, NULL, &resp,  
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

> 这段代码是NX Open C++ API编写的二次开发代码，用于在NX中实现选择一个实体作为钣金体的功能。具体来说：
>
> 1. mask_for_sheet_bodies函数用于设置选择集的选择掩码，以仅允许选择实体类型的钣金体特征。
> 2. select_a_sheet函数使用NX的UI选择对话框，让用户选择一个实体。在选择前，会清除当前的选择集，并设置只允许选择实体类型的钣金体特征。选择完成后，会取消高亮显示，并返回所选实体的tag。
> 3. 使用UF_DISP_set_highlight函数取消高亮显示所选实体。
> 4. select_a_sheet函数会返回所选实体的tag，如果用户取消了选择，则返回NULL_TAG。
> 5. 整个代码流程是先设置选择掩码，然后让用户选择，最后取消选择的高亮显示。选择的结果返回给调用函数。
> 6. 通过设置选择掩码，实现了精确选择实体类型的钣金体特征的功能，避免用户选择到其他类型的特征。
>
