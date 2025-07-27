### 【2614】select a udo 选择一个用户定义对象

#### 代码

```cpp
    static int filter_udo_class(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        UF_UDOBJ_class_t  
            *desired_class_id = (UF_UDOBJ_class_t *) data,  
            this_class_id;  
        UF_UDOBJ_all_data_t  
            udo_data;  
        if (type[0] != UF_user_defined_object_type) return (UF_UI_SEL_REJECT);  
        UF_CALL(UF_UDOBJ_ask_udo_data (object, &udo_data));  
        this_class_id = udo_data.class_id;  
        UF_CALL(UF_UDOBJ_free_udo_data(&udo_data));  
        if ((this_class_id == *desired_class_id) || (*desired_class_id == 0))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_udo_class(UF_UI_selection_p_t select, void *class)  
    {  
        UF_UI_mask_t  
            mask = { UF_user_defined_object_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_udo_class, NULL,  
                class)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_udo(char *prompt, UF_UDOBJ_class_t desired_class)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_udo_class, &desired_class,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码用于在NX中实现用户自定义对象(UDO)的选择功能。
>
> 主要步骤包括：
>
> 1. 定义了一个过滤函数filter_udo_class，用于在用户选择时过滤出特定类别的UDO。它检查选中的对象是否为UDO，并判断其类别是否符合预期。
> 2. 定义了一个设置选择掩码的函数mask_udo_class，用于设置选择范围仅限于UDO。
> 3. 定义了一个选择UDO的函数select_a_udo，它显示一个对话框，让用户选择一个UDO。在选择前，通过mask_udo_class设置了选择掩码，并通过filter_udo_class设置了过滤条件。
> 4. 在select_a_udo中，用户选择后，获取选中的UDO的tag，并取消其高亮显示。
> 5. 如果用户选择了一个UDO，select_a_udo返回该UDO的tag；否则返回NULL_TAG。
> 6. 通过设置过滤条件和选择掩码，代码实现了精确地选择特定类别的UDO的功能。
>
