### 【2612】select a tolerance feature inst 选择一个公差特征实例

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_tol_feat_insts( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tol_feature_instance_type, 0, 0};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_tolerance_feature_inst(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt,  
            "Select GDT feature instance",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_tol_feat_insts, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open中的二次开发代码，主要功能是选择一个公差特征实例。
>
> 关键点包括：
>
> 1. mask_for_tol_feat_insts函数：设置选择掩码，只允许选择公差特征实例。
> 2. select_a_tolerance_feature_inst函数：使用单选对话框，提示用户选择一个公差特征实例。用户选择后，将高亮该特征实例。
> 3. UF_UI_select_with_single_dialog函数：弹出单选对话框，让用户选择对象，并返回所选对象的tag。
> 4. UF_DISP_set_highlight函数：取消高亮所选特征实例。
> 5. 返回值：如果用户选择了特征实例，则返回其tag，否则返回NULL_TAG。
>
> 总体来说，这段代码实现了在NX中只选择公差特征实例的功能，并返回所选特征实例的tag。
>
