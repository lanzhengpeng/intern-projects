### 【2558】select a datum axis 选择一个基准轴

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_datum_axes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_axis_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_axes, NULL, &resp,  
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

> 根据提供的NX二次开发代码，这是用于选择基准轴的函数。
>
> 函数功能如下：
>
> 1. mask_for_datum_axes：用于设置选择过滤器，以便只选择基准轴。它将基准轴的类型添加到选择过滤器中。
> 2. select_a_datum_axis：使用NX提供的UF_UI_select_with_single_dialog函数，弹出一个对话框让用户选择一个基准轴。该函数首先清除所有选择过滤器，然后只启用基准轴类型的选择过滤器。用户选择后，函数返回基准轴的tag。
>
> 通过这段代码，用户可以在NX中方便地选择一个基准轴，并且返回基准轴的tag，以便后续操作。
>
