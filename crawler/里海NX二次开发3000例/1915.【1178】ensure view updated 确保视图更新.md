### 【1178】ensure view updated 确保视图更新

#### 代码

```cpp
    static void ensure_view_updated(tag_t object)  
    {  
        logical  
            out_of_date;  
        int  
            status;  
        tag_t  
            dwg_tag,  
            view_tag;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        if (!uc6409(object, &status, view_name) && (status == 1))  
        {  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
            UF_CALL(UF_VIEW_ask_type(view_tag, &type, &subtype));  
            if (type == UF_VIEW_DRAWING_MEMBER_TYPE)  
            {  
                UF_CALL(UF_DRF_is_object_out_of_date(view_tag, &out_of_date));  
                if (out_of_date)  
                {  
                    UF_CALL(UF_DRAW_ask_drawing_of_view(view_tag, &dwg_tag));  
                    UF_CALL(UF_DRAW_update_one_view(dwg_tag, view_tag));  
                }  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是检查和更新NX视图。代码流程如下：
>
> 1. 获取传入参数object的视图名称。
> 2. 查询视图名称对应的视图tag。
> 3. 查询视图的类别和子类别，判断是否为绘图成员视图。
> 4. 如果是绘图成员视图，检查视图是否过时。
> 5. 如果视图过时，查询视图对应的绘图tag。
> 6. 调用函数更新视图。
>
> 该代码主要用于在二次开发中检查和更新NX绘图成员视图，确保视图内容与模型保持一致。
>
