### 【2710】select solids 选择实体

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
    static int select_solids(char *prompt, tag_t **solids)  
    {  
        int  
            ii,  
            n,  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select solids", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp, &n, solids));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*solids)[ii], FALSE));  
        return n;  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了一个名为filter_body_type的函数，用于过滤出指定类型的实体。该函数接受实体、类型数组、数据指针和选择集作为参数，通过UF_MODL_ask_body_type函数获取实体的类型，然后与数据指针指向的类型进行比较，如果匹配则返回UF_UI_SEL_ACCEPT，否则返回UF_UI_SEL_REJECT。
> 2. 定义了一个名为mask_body_type的函数，用于设置选择集的过滤掩码。该函数接受选择集和数据指针作为参数，通过UF_UI_set_sel_mask函数清除选择集的掩码并设置特定的过滤条件，然后通过UF_UI_set_sel_procs函数设置选择集的过滤函数filter_body_type和数据指针。如果设置成功则返回UF_UI_SEL_SUCCESS，否则返回UF_UI_SEL_FAILURE。
> 3. 定义了一个名为select_solids的函数，用于选择指定类型的实体。该函数接受提示字符串、实体数组指针作为参数。首先设置过滤条件，然后调用UF_UI_select_with_class_dialog函数显示选择对话框，用户可以按照提示选择实体。选择完成后，关闭高亮显示，并返回选择实体的数量。
>
> 总体来说，这段代码通过设置选择集的过滤条件，实现了根据类型选择实体的功能。
>
