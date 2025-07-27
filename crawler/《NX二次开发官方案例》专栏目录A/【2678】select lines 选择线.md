### 【2678】select lines 选择线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_line_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_lines(char *prompt, tag_t **lines)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select lines", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_lines, NULL, &resp, &cnt, lines));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*lines)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的用户交互模块，主要实现了以下功能：
>
> 1. 定义了一个名为mask_for_lines的函数，用于设置选择线的类型为UF_line_type，并启用该类型的选择。该函数接受选择对象select和类型指针type作为参数。
> 2. 定义了一个名为select_lines的函数，用于实现选择线的功能。该函数接受提示信息prompt和线数组指针lines作为参数。在函数内部，首先调用UF_UI_select_with_class_dialog函数，使用mask_for_lines作为选择类型过滤函数，实现选择线的对话框，并获取选择的线的数量和数组。然后，遍历选择的线数组，关闭高亮显示。最后，返回选择线的数量。
> 3. select_lines函数可以接受提示信息，弹出选择线的对话框，用户可以在对话框中选择要操作的线，并返回选择的线的数组。
> 4. 这段代码实现了NX二次开发中常见的交互式选择线功能，为后续处理选择线提供了便利。
>
