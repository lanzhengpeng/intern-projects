### 【2705】select sheets 选择薄片

#### 代码

```cpp
    /* 里海 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_sheets(char *prompt, tag_t **sheets)  
    {  
        int  
            ii,  
            n,  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select sheet bodies", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp, &n, sheets));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*sheets)[ii], FALSE));  
        return n;  
    }

```

#### 代码解析

> 这段代码是用于在NX中进行二次开发，主要实现了选择钣金体的功能。以下是代码的主要功能介绍：
>
> 1. filter_body_type函数：用于过滤选择，只接受指定的钣金体类型。
> 2. mask_body_type函数：设置选择掩码，只允许选择钣金体。
> 3. select_sheets函数：显示选择对话框，让用户选择钣金体。它设置了只允许选择钣金体的掩码，并调用了NX的UF_UI_select_with_class_dialog函数来显示选择对话框。
> 4. 代码逻辑：首先清除了原有的选择，然后设置了只选择钣金体的掩码，接着调用了NX的UF_UI_select_with_class_dialog函数来让用户选择钣金体。用户选择后，获取选择的钣金体tag数组，并关闭了高亮显示。最后返回选择的钣金体数量。
>
> 综上所述，这段代码实现了在NX中通过对话框让用户选择特定类型的钣金体的功能，并屏蔽了其他类型的实体，以确保只选择钣金体。
>
