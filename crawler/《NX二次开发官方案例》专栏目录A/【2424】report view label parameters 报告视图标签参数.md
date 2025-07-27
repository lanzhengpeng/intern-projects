### 【2424】report view label parameters 报告视图标签参数

#### 代码

```cpp
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void report_view_label_parameters(tag_t view_label)  
    {  
        UF_DRAW_view_label_parms_t  
            parms;  
        if (UF_CALL(UF_DRAW_ask_view_label_parms(view_label, &parms))) return;  
        switch (parms.view_label_parm_type)  
        {  
            case UF_DRAW_view_label_other_view:  
                ECHO("parms.view_label_parm_type = UF_DRAW_view_label_other_view\n");  
                break;  
            case UF_DRAW_view_label_detail_view:  
                ECHO("parms.view_label_parm_type = UF_DRAW_view_label_detail_view\n");  
                break;  
            case UF_DRAW_view_label_section_view:  
                ECHO("parms.view_label_parm_type = UF_DRAW_view_label_section_view\n");  
                break;  
            default:  
                ECHO("parms.view_label_parm_type = unknown (%d)\n", parms.view_label_parm_type);  
                break;  
        }  
        switch (parms.view_label_type)  
        {  
            case UF_DRAW_view_label_view_name:  
                ECHO("parms.view_label_type = UF_DRAW_view_label_view_name\n");  
                break;  
            case UF_DRAW_view_label_prefix_and_letter:  
                ECHO("parms.view_label_type = UF_DRAW_view_label_prefix_and_letter\n");  
                break;  
            default:  
                ECHO("parms.view_label_type = unknown (%d)\n", parms.view_label_type);  
                break;  
        }  
        switch (parms.letter_format)  
        {  
            case UF_DRAW_view_label_single_letter:  
                ECHO("parms.letter_format = UF_DRAW_view_label_single_letter\n");  
                break;  
            case UF_DRAW_view_label_dashed_letter:  
                ECHO("parms.letter_format = UF_DRAW_view_label_dashed_letter\n");  
                break;  
            default:  
                ECHO("parms.letter_format = unknown (%d)\n", parms.letter_format);  
                break;  
        }  
        switch (parms.view_label_position)  
        {  
            case UF_DRAW_view_label_below_boundary:  
                ECHO("parms.view_label_position = UF_DRAW_view_label_below_boundary\n");  
                break;  
            case UF_DRAW_view_label_above_boundary:  
                ECHO("parms.view_label_position = UF_DRAW_view_label_above_boundary\n");  
                break;  
            default:  
                ECHO("parms.view_label_position = unknown (%d)\n", parms.view_label_position);  
                break;  
        }  
        switch (parms.view_scale_position)  
        {  
            case UF_DRAW_view_label_below:  
                ECHO("parms.view_scale_position = UF_DRAW_view_label_below\n");  
                break;  
            case UF_DRAW_view_label_above:  
                ECHO("parms.view_scale_position = UF_DRAW_view_label_above\n");  
                break;  
            case UF_DRAW_view_label_before:  
                ECHO("parms.view_scale_position = UF_DRAW_view_label_before\n");  
                break;  
            case UF_DRAW_view_label_after:  
                ECHO("parms.view_scale_position = UF_DRAW_view_label_after\n");  
                break;  
            default:  
                ECHO("parms.view_scale_position = unknown (%d)\n", parms.view_scale_position);  
                break;  
        }  
        switch (parms.view_scale_value_format)  
        {  
            case UF_DRAW_view_label_ratio:  
                ECHO("parms.view_scale_value_format = UF_DRAW_view_label_ratio\n");  
                break;  
            case UF_DRAW_view_label_vertical_fraction:  
                ECHO("parms.view_scale_value_format = UF_DRAW_view_label_vertical_fraction\n");  
                break;  
            case UF_DRAW_view_label_horizontal_fraction:  
                ECHO("parms.view_scale_value_format = UF_DRAW_view_label_horizontal_fraction\n");  
                break;  
            case UF_DRAW_view_label_Nx:  
                ECHO("parms.view_scale_value_format = UF_DRAW_view_label_Nx\n");  
                break;  
            default:  
                ECHO("parms.view_scale_value_format = unknown (%d)\n", parms.view_scale_value_format);  
                break;  
        }  
        switch (parms.view_label_text_to_stub_format)  
        {  
            case UF_DRAW_parent_view_label_text_before_stub:  
                ECHO("parms.view_label_text_to_stub_format = UF_DRAW_parent_view_label_text_before_stub\n");  
                break;  
            case UF_DRAW_parent_view_label_text_above_stub:  
                ECHO("parms.view_label_text_to_stub_format = UF_DRAW_parent_view_label_text_above_stub\n");  
            default:  
                ECHO("parms.view_label_text_to_stub_format = unknown (%d)\n", parms.view_label_text_to_stub_format);  
                break;  
        }  
        WRITE_F(parms.letter_size_factor);  
        WRITE_F(parms.view_scale_prefix_factor);  
        WRITE_F(parms.view_scale_text_factor);  
        WRITE_L(parms.view_label);  
        WRITE_L(parms.scale_label);  
        WRITE_L(parms.scale_parentheses);  
        WRITE_S(parms.view_label_prefix);  
        WRITE_S(parms.scale_label_prefix);  
        WRITE_S(parms.view_letter);  
        switch (parms.parent_label_type)  
        {  
            case UF_DRAW_parent_view_label_no_display:  
                ECHO("parms.parent_label_type = UF_DRAW_parent_view_label_no_display\n");  
                break;  
            case UF_DRAW_parent_view_label_boundary:  
                ECHO("parms.parent_label_type = UF_DRAW_parent_view_label_boundary (aka UF_DRAW_parent_view_label_circle)\n");  
                break;  
            case UF_DRAW_parent_view_label_note:  
                ECHO("parms.parent_label_type = UF_DRAW_parent_view_label_note\n");  
                break;  
            case UF_DRAW_parent_view_label_label:  
                ECHO("parms.parent_label_type = UF_DRAW_parent_view_label_label\n");  
                break;  
            case UF_DRAW_parent_view_label_embedded:  
                ECHO("parms.parent_label_type = UF_DRAW_parent_view_label_embedded\n");  
                break;  
            case UF_DRAW_parent_view_label_true_boundary:  
                ECHO("parms.parent_label_type = UF_DRAW_parent_view_label_true_boundary\n");  
                break;  
            default:  
                ECHO("parms.parent_label_type = unknown (%d)\n", parms.parent_label_type);  
                break;  
        }  
        WRITE_F(parms.text_to_gap_factor);  
        WRITE_S(parms.parent_vw_lbl_prefix);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是报告和输出视图标签的参数信息。具体包括以下内容：
>
> 1. 定义了三个宏WRITE_L、WRITE_F、WRITE_S，用于输出布尔、浮点数和字符串类型的变量。
> 2. 定义了一个函数write_string_to_listing_window，用于将字符串输出到列表窗口。
> 3. 定义了一个函数report_view_label_parameters，用于报告视图标签的参数信息。该函数首先调用UF_DRAW_ask_view_label_parms获取视图标签的参数信息，然后通过一系列switch语句，根据参数的类型输出不同的信息。这些参数包括视图标签的类型、字母格式、位置、比例尺位置、比例尺值格式等。
> 4. 最后，函数通过WRITE_F、WRITE_L、WRITE_S宏输出视图标签的前缀、父视图标签的前缀、字母、视图标签、比例尺标签等参数信息。
>
> 总的来说，这段代码实现了对NX视图标签参数的查询和报告功能。
>
