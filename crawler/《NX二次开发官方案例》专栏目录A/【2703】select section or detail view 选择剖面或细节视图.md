### 【2703】select section or detail view 选择剖面或细节视图

#### 代码

```cpp
    static tag_t select_section_or_detail_view(void)  
    {  
        tag_t  
            view_tag;  
        double  
            rr2[3];  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        if (uc1615("Select section or detail view", rr2) == 5)  
        {  
            uc1653(view_name);  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag)))  
            UF_VIEW_ask_type(view_tag, &type, &subtype);  
            if (     (type = UF_VIEW_DRAWING_MEMBER_TYPE) &&   
                    ((subtype == UF_VIEW_SECTION_SUBTYPE) ||  
                    (subtype == UF_VIEW_DETAIL_SUBTYPE)) )  
                return view_tag;  
            else  
            {  
                uc1601("This is not a detail or section view", TRUE);  
                select_section_or_detail_view();  
            }   
        }  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现选择剖视图或详细视图的功能。
>
> 主要功能包括：
>
> 1. 调用UF_UI_set_cursor_view(FALSE)将光标设置为不可见。
> 2. 使用uc1615函数提示用户选择一个视图，并获取选中视图的名称和坐标。
> 3. 调用UF_VIEW_ask_tag_of_view_name获取选中视图的标签。
> 4. 使用UF_VIEW_ask_type获取选中视图的类型和子类型。
> 5. 判断视图类型是否为UF_VIEW_DRAWING_MEMBER_TYPE，子类型是否为UF_VIEW_SECTION_SUBTYPE或UF_VIEW_DETAIL_SUBTYPE，如果是则返回视图标签，否则提示错误并重新选择。
> 6. select_section_or_detail_view()函数递归调用自身，直到选择正确的视图为止。
>
> 总体来说，这段代码实现了选择剖视图或详细视图的功能，并具有错误处理和递归选择视图的能力。
>
