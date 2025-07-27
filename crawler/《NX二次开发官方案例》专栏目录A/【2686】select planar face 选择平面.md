### 【2686】select planar face 选择平面

#### 代码

```cpp
/* 里海 */
static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_planar_face(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,
        UF_UI_SEL_SCOPE_WORK_PART,
        mask_for_planar_face, NULL, &resp, &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, FALSE));
        return object;
    }
    return NULL_TAG;
}

```

#### 代码解析

> 根据代码内容，这段代码实现了在NX中通过UI选择平面面的功能。
>
> 主要步骤包括：
>
> 1. 定义一个UF_UI_mask_t结构体，用于指定选择集只包含实体类型和平面面。
> 2. 定义mask_for_planar_face函数，用于设置选择集的mask，仅允许选择平面面。
> 3. 定义select_planar_face函数，用于通过弹窗提示用户选择平面面，并返回选择的平面面的tag。
> 4. 在select_planar_face函数中，使用UF_UI_select_with_single_dialog函数弹窗提示用户选择，并调用mask_for_planar_face设置选择集的mask。
> 5. 用户选择后，取消高亮，并返回所选平面面的tag。
> 6. 如果用户取消选择，则返回NULL_TAG。
>
> 总体来说，这段代码通过设置选择集的mask，实现了仅选择平面面的功能，并返回了所选平面面的tag，以便后续处理。
>
