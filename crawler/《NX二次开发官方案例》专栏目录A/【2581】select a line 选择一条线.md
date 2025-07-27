### 【2581】select a line 选择一条线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },   
                        { UF_section_segment_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个名为mask_for_lines的函数，用于设置选择线的类型过滤条件。它接受选择对象和类型参数，并返回成功或失败标志。
> 2. 在mask_for_lines函数中，定义了一个包含4个UF_UI_mask_t结构体的数组，用于指定要选择的线的类型，包括UF_line_type、UF_solid_type等。
> 3. mask_for_lines函数使用UF_UI_set_sel_mask函数，清除并启用指定的类型过滤条件，从而实现对特定类型线的过滤选择。
> 4. 定义了一个名为select_a_line的函数，用于提示用户选择一条线，并返回选择的线的tag。
> 5. select_a_line函数首先设置光标视图为0，然后使用UF_UI_select_with_single_dialog函数弹出一个单选对话框，提示用户选择线。
> 6. select_a_line函数调用mask_for_lines函数设置类型过滤条件，然后等待用户选择线，并返回响应结果。
> 7. 如果用户成功选择了线，select_a_line函数会关闭高亮显示，并返回线的tag；否则，返回空tag。
> 8. 通过select_a_line函数，实现了让用户从图形界面选择一条特定类型的线，并返回该线的tag，以便后续处理。
>
