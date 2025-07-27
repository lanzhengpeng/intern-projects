### 【2600】select a routing segment 选择一个布线段

#### 代码

```cpp
    /*  This fails prior to NX 1.0.3.1 - see PR 4733117 译:这个在NX 1.0.3.1之前就失败了 - 见PR 4733117 */  
    /* 里海 */  
    static int mask_for_routing_segments(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { 0, 0, UF_UI_SEL_FEATURE_ROUTING_CURVE_ONLY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_routing_segment(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a routing segment", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_routing_segments, NULL, &resp,  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了一个函数mask_for_routing_segments，用于设置选择掩码，使得在选择对话框中只能选择线特征，用于后续的路由线段选择。
> 2. 定义了一个函数select_a_routing_segment，该函数显示一个选择对话框，提示用户选择一个路由线段。在选择前，会调用mask_for_routing_segments来设置选择掩码。用户选择后，函数会返回选中的线段对象。
> 3. select_a_routing_segment函数首先调用NX的UF_UI_select_with_single_dialog函数来显示选择对话框，然后根据返回的选择结果，如果选择了对象，则返回对象标签，否则返回NULL_TAG。
> 4. 注意代码注释提到，这段代码在NX 1.0.3.1版本之前会失败，因为之前版本的选择对话框API有bug，在1.0.3.1版本得到了修复。
> 5. 整体而言，这段代码实现了在NX中选择一个路由线段的功能，并且通过选择掩码限制只能选择线特征。选择完成后，返回线段对象标签。
>
