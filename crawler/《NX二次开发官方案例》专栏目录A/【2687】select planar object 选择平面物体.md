### 【2687】select planar object 选择平面物体

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_planar_object(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_planar_object(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART_AND_OCC, mask_for_planar_object, NULL, &resp,  
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

> 这段NX Open C++代码用于在NX中创建一个对话框，以便用户选择一个平面几何体（平面、基准平面或平面特征）。代码的主要功能如下：
>
> 1. 定义了一个mask数组，用于指定要选择的几何体的类型。mask数组包含三个元素，分别表示平面特征、平面和基准平面。
> 2. 定义了一个函数mask_for_planar_object，用于设置对话框的选择掩码。该函数清除了所有选择掩码，并启用了mask数组中指定的几何体类型。
> 3. 定义了一个函数select_a_planar_object，用于打开对话框让用户选择一个平面几何体。该函数会一直等待用户选择，直到用户点击确定或取消。如果用户选择了一个平面几何体，则返回其tag；如果用户取消选择，则返回NULL_TAG。
> 4. select_a_planar_object函数会使用mask_for_planar_object函数设置选择掩码。如果用户选择了一个平面几何体，则返回该几何体的tag。
> 5. 代码中使用了UF_UI_select_with_single_dialog函数来打开对话框。该函数会一直等待用户选择或取消，直到返回响应码。
> 6. 根据返回的响应码，判断用户是否成功选择了一个平面几何体。如果成功，则取消高亮显示并返回该几何体的tag。
> 7. 如果用户取消选择，则返回NULL_TAG。
>
> 这段代码实现了在NX中通过对话框选择平面几何体的功能。
>
