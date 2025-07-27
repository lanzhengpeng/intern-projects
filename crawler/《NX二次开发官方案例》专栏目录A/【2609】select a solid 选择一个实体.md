### 【2609】select a solid 选择一个实体

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_solid_bodies( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_SOLID_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_solid_bodies, NULL, &resp,  
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

> 这段代码是一个用于在NX中获取用户选择实体的二次开发代码。主要功能包括：
>
> 1. mask_for_solid_bodies函数：用于设置选择过滤条件，仅允许选择实体类型为“固体”的实体。
> 2. select_a_solid函数：用于打开一个选择对话框，提示用户选择一个实体。在选择时，会应用上一步定义的选择过滤条件。如果用户选择了一个实体，则返回该实体的tag，否则返回NULL_TAG。
> 3. 在选择完成后，会关闭高亮显示。
> 4. 整个代码通过NX的UF/UI接口实现，利用NX提供的UI选择机制来获取用户选择的实体。
>
> 总的来说，这段代码提供了在NX中获取用户选择的实体的一个简单示例，通过设置选择过滤条件，可以方便地控制用户的选择范围。
>
