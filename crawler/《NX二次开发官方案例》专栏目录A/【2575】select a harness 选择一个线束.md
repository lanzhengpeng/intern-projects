### 【2575】select a harness 选择一个线束

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_harnesses(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_route_route_type, UF_route_harness_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_harness(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a Routing Harness", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_harnesses, NULL, &resp,  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，其主要功能是实现选择Harness的操作。下面是对代码的详细介绍：
>
> 1. mask_for_harnesses函数：该函数用于设置选择集的选择掩码，仅允许选择Harness类型的对象。
> 2. select_a_harness函数：该函数通过弹出一个选择对话框，让用户选择一个Harness对象。在选择对话框中，只允许选择Harness类型的对象。如果用户选择了一个对象，则返回该对象的tag，否则返回NULL_TAG。
> 3. 使用方法：在NX中加载这段代码，调用select_a_harness函数，并传入提示字符串，就可以让用户选择一个Harness对象，并返回该对象的tag。
> 4. 注意点：在选择过程中，利用UF_DISP_set_highlight函数高亮显示用户选择的对象，以提高用户体验。
> 5. 适用场景：这段代码适用于需要让用户选择Harness对象的NX二次开发场景。
>
> 总的来说，这段代码实现了选择Harness对象的功能，通过设置选择掩码和弹窗选择的方式，实现了对特定类型对象的精确选择。
>
