### 【2568】select a drawing 选择一张图纸

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_drawings(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_view_type, UF_view_drawing_sheet_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_drawing(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            drawing,  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a drawing", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drawings, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            UF_CALL(UF_DRAW_ask_drawing_of_view(object, &drawing));  
            return drawing;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，主要功能是选择图纸中的视图，并获取对应的图纸对象。下面是代码的主要功能：
>
> 1. mask_for_drawings函数：该函数用于设置选择过滤条件，仅允许选择图纸视图类型的对象。
> 2. select_a_drawing函数：该函数用于弹出选择对话框，让用户选择图纸视图。在选择后，它取消高亮显示所选对象，并获取该视图对应的图纸对象，最后返回图纸对象标签。
>
> 通过这两个函数的配合，可以实现图纸视图的选择，并获取对应的图纸对象，从而为进一步的图纸操作提供了基础。
>
