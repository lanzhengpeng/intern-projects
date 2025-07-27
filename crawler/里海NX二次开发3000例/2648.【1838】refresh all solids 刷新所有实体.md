### 【1838】refresh all solids 刷新所有实体

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

```

#### 代码解析

> 这段代码是一个NX Open C++ API编写的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个ask_next_visible_solid函数，用于遍历当前活动部件中的所有实体，并返回下一个可见的实体。它接受一个实体tag作为参数，返回下一个可见实体的tag。
> 2. 定义了一个refresh_all_solids函数，用于遍历当前活动部件中的所有实体，并调用UF_DISP_add_item_to_display函数将它们添加到显示列表中，从而刷新显示。
> 3. ask_next_visible_solid函数中，通过调用UF_OBJ_ask_type_and_subtype函数获取实体的类型和子类型，如果找到类型为UF_solid_type且子类型为UF_solid_body_subtype的实体，则返回该实体。如果遍历完所有实体都没有找到，则返回NULL_TAG。
> 4. refresh_all_solids函数中，通过循环调用ask_next_visible_solid函数，获取所有可见实体，并调用UF_DISP_add_item_to_display函数将它们添加到显示列表，从而实现刷新显示的效果。
> 5. 使用了UF_CALL宏来封装UF函数调用，简化错误处理。
> 6. 通过遍历实体树的方式实现遍历，而不是直接获取实体列表，因为实体树中包含了实体之间的父子关系。
> 7. 该代码可以用于在NX Open二次开发中实现实体显示的刷新，如配合菜单命令或事件处理函数使用。
>
