### 【0293】ask next selectable solid 询问下一个可选实体

#### 代码

```cpp
    static tag_t ask_next_selectable_solid(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        UF_OBJ_disp_props_t  
            disp_props;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body)) &&  
                (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY)  
                {  
                    UF_CALL(UF_OBJ_ask_display_properties(body, &disp_props));  
                    UF_CALL(UF_LAYER_ask_status(disp_props.layer, &type));  
                    if ((disp_props.blank_status == UF_OBJ_NOT_BLANKED) &&  
                        (type != UF_LAYER_INACTIVE_LAYER)) return body;  
                }  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码实现了在NX模型中遍历所有可选择的实体，并找出下一个可选择的实体的功能。具体来说，代码逻辑如下：
>
> 1. 使用UF_OBJ_cycle_objs_in_part()函数遍历指定零件中的所有实体，直到遍历结束。
> 2. 对于每个实体，调用UF_OBJ_ask_type_and_subtype()函数获取其类型和子类型。
> 3. 如果子类型为UF_solid_body_subtype，则进一步调用UF_MODL_ask_body_type()确认其是否为实体体。
> 4. 如果是实体体，则调用UF_OBJ_ask_display_properties()和UF_LAYER_ask_status()获取其显示属性和图层状态。
> 5. 如果实体体既未被隐藏，也不在非活动图层上，则将其作为下一个可选择的实体返回。
> 6. 如果遍历结束仍未找到可选择的实体，则返回NULL_TAG。
>
> 总的来说，这段代码实现了遍历指定零件中的所有实体，并找出下一个可选择的实体的功能。
>
