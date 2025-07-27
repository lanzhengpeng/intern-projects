### 【2240】report object layer plaintext报告对象图层

#### 代码

```cpp
    static void report_object_layer(tag_t object)  
    {  
        int  
            component_layer,  
            new_layer,  
            object_layer,  
            prototype_layer;  
        tag_t  
            component,  
            prototype;  
        UF_OBJ_disp_props_t  
            disp_props;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        object_layer = disp_props.layer;  
        sprintf(msg, "object_layer = %d", object_layer);  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            component = UF_ASSEM_ask_part_occurrence(object);  
            UF_CALL(UF_OBJ_ask_display_properties(component, &disp_props));  
            component_layer = disp_props.layer;  
            prototype = UF_ASSEM_ask_prototype_of_occ(object);  
            UF_CALL(UF_OBJ_ask_display_properties(prototype, &disp_props));  
            prototype_layer = disp_props.layer;  
            if (object_layer == component_layer)  
            {  
                UF_CALL(UF_DISP_set_display(FALSE));  
                new_layer = object_layer + 1;  
                if (new_layer > UF_LAYER_MAX_LAYER) new_layer = 1;  
                UF_CALL(UF_OBJ_set_layer(component, new_layer));  
                UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
                if (disp_props.layer == new_layer)  
                    strcat(msg, ", inherited from component\n");  
                else  
                    strcat(msg, ", set on component member\n");  
                UF_CALL(UF_OBJ_set_layer(component, component_layer));  
                UF_CALL(UF_DISP_set_display(TRUE));  
            }  
            else if (object_layer == prototype_layer)  
            {  
                UF_CALL(UF_DISP_set_display(FALSE));  
                new_layer = object_layer + 1;  
                if (new_layer > UF_LAYER_MAX_LAYER) new_layer = 1;  
                UF_CALL(UF_OBJ_set_layer(prototype, new_layer));  
                UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
                if (disp_props.layer == new_layer)  
                    strcat(msg, ", inherited from original object\n");  
                else  
                    strcat(msg, ", set on component member\n");  
                UF_CALL(UF_OBJ_set_layer(prototype, prototype_layer));  
                UF_CALL(UF_DISP_set_display(TRUE));  
            }  
            else  
                strcat(msg, ", set on component member\n");  
        }  
        else  
            strcat(msg, "\n");  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能是获取并报告NX对象所在的图层信息。
>
> 代码首先定义了一些变量，用于存储组件、原型、图层等相关的信息。接着，通过UF_OBJ_ask_display_properties函数获取指定NX对象的显示属性，并从中提取出图层信息。然后，根据对象是组件实例还是原型，分别获取组件和原型的图层信息，并判断对象图层是否与组件或原型的图层相同。如果相同，则尝试修改组件或原型的图层，并判断对象图层是否发生变化。最后，将对象图层的判断结果输出到列表窗口中。
>
> 通过这段代码，可以有效地判断NX对象的图层信息来源，是继承自组件还是原型，还是单独设置在组件实例上的。这对于理解和调试NX模型中图层的使用非常有帮助。
>
