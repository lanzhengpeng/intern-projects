### 【2546】select a bsurf 选择一个边界曲面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_bsurfs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PARAMETRIC_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_bsurf(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a bsurf", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bsurfs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发的示例代码，其主要功能是使用户能够选择一个B曲面。
>
> 关键点包括：
>
> 1. 函数mask_for_bsurfs：设置选择掩码，以仅允许选择参数化面。
> 2. 函数select_a_bsurf：弹出一个对话框，提示用户选择一个B曲面。
> 3. UF_UI_select_with_single_dialog：创建单选对话框，设置选择掩码，并获取用户的选择结果。
> 4. 如果用户选择了一个对象，则取消其高亮显示，并返回该对象的tag。
> 5. 如果用户取消选择，则返回NULL_TAG。
> 6. 该代码通过自定义选择掩码和对话框，实现了仅选择B曲面的功能。
> 7. 代码使用了NX的UF和UI函数来实现选择和交互。
>
> 总的来说，这段代码实现了在NX中通过自定义选择掩码，让用户只选择B曲面的功能，实现了参数化面的特定选择交互。
>
