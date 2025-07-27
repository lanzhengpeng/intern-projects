### 【2662】select drf line 选择DRF线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_drf_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_line_type, 0, 0 },  
                       {UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                       { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_line(char *prompt, UF_DRF_object_p_t line)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_lines, NULL, &resp,  
            &line->object_tag, cp, &line->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(line->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于在NX中选取一条线的二次开发代码。主要步骤如下：
>
> 1. 定义一个UF_UI_mask_t类型的数组mask，用于设置选取线的类型，包括UF_line_type、UF_solid_type和UF_section_edge_type。
> 2. 定义一个函数mask_for_drf_lines，用于设置选取线的类型。它接受一个选择对象select和一个void类型指针type，并返回一个整型值。在这个函数中，它首先调用UF_UI_set_sel_mask来设置选取线的类型为mask数组指定的类型，然后返回成功标志。
> 3. 定义一个函数select_drf_line，用于选取线。它接受一个字符串提示和一个UF_DRF_object_p_t类型的线对象指针。在函数中，首先设置光标视图，然后调用UF_UI_select_with_single_dialog函数弹出一个对话框，等待用户选取一条线。如果用户选取了线，则返回真。
> 4. 在select_drf_line函数中，如果用户选取了线，则调用UF_DISP_set_highlight函数高亮显示选取的线，然后返回真。
> 5. 如果用户没有选取线，则select_drf_line函数返回假。
> 6. 这段代码的主要作用是，通过定义选取线的类型和弹出一个对话框，让用户在NX中选取一条指定的线，并返回这条线的信息。
> 7. 这段代码是用于二次开发中实现用户交互式选取线的功能。
>
