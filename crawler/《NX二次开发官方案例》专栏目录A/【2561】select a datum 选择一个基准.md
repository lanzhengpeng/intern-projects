### 【2561】select a datum 选择一个基准

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_datum_plane_type, 0, 0 },  
                        { UF_datum_axis_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane or axis",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datums, NULL, &resp,  
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

> 这段代码实现了在NX中选取一个基准平面或基准轴的功能，具体步骤如下：
>
> 1. 定义一个函数mask_for_datums，用于设置选取掩码，仅允许选取基准平面或基准轴。
> 2. 定义一个函数select_a_datum，用于在对话框中提示用户选取一个基准平面或基准轴，并返回选取对象的标签。
> 3. 在select_a_datum函数中，使用UF_UI_select_with_single_dialog函数打开一个单选对话框，并设置提示文字、选取范围、选取掩码等参数。
> 4. 用户在对话框中选择一个对象后，UF_UI_select_with_single_dialog函数返回选取响应和对象标签等信息。
> 5. 如果选取成功，关闭高亮显示，并返回选取对象的标签。
> 6. 如果选取失败，则返回NULL_TAG。
> 7. 在其他地方调用select_a_datum函数，传入提示文字，即可实现选取一个基准平面或基准轴的功能。
>
> 总体来说，这段代码通过自定义选取掩码和单选对话框，实现了在NX中方便地选取基准平面或基准轴的功能。
>
