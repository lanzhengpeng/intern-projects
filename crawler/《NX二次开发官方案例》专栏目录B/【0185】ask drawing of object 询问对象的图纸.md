### 【0185】ask drawing of object 询问对象的图纸

#### 代码

```cpp
    static tag_t ask_drawing_of_object(tag_t object)  
    {  
        int  
            status,  
            view_subtype,  
            view_type;  
        tag_t  
            drawing = NULL_TAG,  
            view;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(uc6409(object, &status, view_name));  
        if (status == 1)  
        {  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
            UF_CALL(UF_VIEW_ask_type(view, &view_type, &view_subtype));  
            if ((view_type == UF_VIEW_DRAWING_SHEET_TYPE) ||  
                (view_type == UF_VIEW_DRAWING_MEMBER_TYPE))  
                UF_CALL(UF_DRAW_ask_drawing_of_view(view, &drawing));  
        }  
        return drawing;  
    }

```

#### 代码解析

> 这段代码实现了以下功能：
>
> 1. 首先定义了用于存储返回的视图对象和绘图对象的数据类型。
> 2. 通过调用UF函数uc6409，获取指定对象(object)的视图名称，并将视图名称存储在view_name数组中。
> 3. 如果uc6409调用成功，则通过UF_VIEW_ask_tag_of_view_name函数获取视图的标签，并存储在view变量中。
> 4. 接着，调用UF_VIEW_ask_type函数获取视图的类型(view_type)和子类型(view_subtype)，并根据视图类型判断是否为绘图或绘图成员。
> 5. 如果视图是绘图或绘图成员，则调用UF_DRAW_ask_drawing_of_view函数，获取视图对应的绘图对象，并将其存储在drawing变量中。
> 6. 最后，返回绘图对象drawing的标签，如果视图不是绘图或绘图成员，则返回NULL_TAG。
>
> 总之，该函数用于根据给定的对象标签，查询并返回对应的绘图对象标签。
>
