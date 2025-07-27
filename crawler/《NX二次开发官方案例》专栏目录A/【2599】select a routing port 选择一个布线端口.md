### 【2599】select a routing port 选择一个布线端口

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_routing_ports(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_route_port_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_routing_port(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a port", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_routing_ports, NULL, &resp,  
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

> 这段代码是用于在NX软件中进行路由端口选择的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了一个名为mask_for_routing_ports的函数，用于设置选择掩码，只允许选择路由端口。
> 2. 定义了一个名为select_a_routing_port的函数，用于通过对话框提示用户选择一个路由端口。
> 3. 在select_a_routing_port函数中，使用UF_UI_select_with_single_dialog函数弹出对话框，提示用户选择。对话框的标题为"Select a port"，提示信息为传入的prompt参数。
> 4. 选择掩码由mask_for_routing_ports函数提供，只允许选择类型为UF_route_port_type的实体。
> 5. 用户选择后，根据响应码判断是否成功选择了一个对象，如果是，则返回该对象的tag。
> 6. 在选择对象后，使用UF_DISP_set_highlight函数取消高亮显示。
> 7. 如果用户没有选择任何对象，则返回NULL_TAG。
>
> 综上所述，这段代码通过二次开发实现了在NX中方便地选择路由端口的功能。
>
