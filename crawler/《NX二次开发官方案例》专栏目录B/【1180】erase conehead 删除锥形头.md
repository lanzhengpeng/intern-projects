### 【1180】erase conehead 删除锥形头

#### 代码

```cpp
    static tag_t ask_next_visible_solid(tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(uc6457("", 1, &body)) && (body != NULL_TAG))  
        {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                    return body;  
        }  
        return NULL_TAG;  
    }  
    static void refresh_all_solids(void)  
    {  
        tag_t  
            solid = NULL_TAG;  
        while ((solid = ask_next_visible_solid(solid)) != NULL_TAG)  
            UF_CALL(UF_DISP_add_item_to_display(solid));  
    }  
    static void erase_conehead(double origin[3], double vec[3])  
    {  
        int  
            old_color;  
        UF_DISP_conehead_attrb_s  
            attributes;  
        UF_VIEW_visualization_t  
            view_data;  
        UF_DISP_get_conehead_attrb(&attributes);  
        old_color = attributes.color;  
        attributes.color = -1;  
        UF_DISP_set_conehead_attrb(&attributes);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_CALL(UF_VIEW_ask_visualization(NULL_TAG, &view_data));  
        if (view_data.display_mode != UF_VIEW_NOT_SHADED) refresh_all_solids();  
        attributes.color = old_color;  
        UF_DISP_set_conehead_attrb(&attributes);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要用于NX二次开发中的图形显示控制。
>
> 代码的主要功能包括：
>
> 1. ask_next_visible_solid函数：遍历模型中的实体，查找下一个可见的实体，并返回其tag。如果当前实体不是实体，则返回NULL_TAG。
> 2. refresh_all_solids函数：遍历模型中的所有实体，将它们添加到显示列表中，以刷新实体在视图中的显示。
> 3. erase_conehead函数：首先获取当前圆锥头的属性，然后将其颜色设置为透明，并绘制一个圆锥头。之后，根据当前视图的显示模式，决定是否刷新所有实体的显示。最后恢复圆锥头原来的颜色。
> 4. 使用NX Open C++ API中的UF_CALL宏来调用UF函数，并检查调用是否成功。
> 5. 通过UF_OBJ_ask_type_and_subtype函数判断实体的类型和子类型。
> 6. 通过UF_DISP_add_item_to_display函数将实体添加到显示列表中。
> 7. 通过UF_DISP_conehead函数绘制圆锥头。
> 8. 通过UF_DISP_get_conehead_attrb和UF_DISP_set_conehead_attrb函数获取和设置圆锥头的属性。
> 9. 通过UF_VIEW_ask_visualization函数获取当前视图的显示模式。
>
> 总的来说，这段代码实现了遍历模型实体、刷新实体显示、绘制圆锥头并控制其颜色等功能。
>
