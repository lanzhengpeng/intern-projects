### 【1260】filter body type 过滤体类型

#### 代码

```cpp
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

```

#### 代码解析

> 这段代码是NX Open C++二次开发代码，用于在NX中实现一个过滤函数。
>
> 函数名为filter_body_type，它实现了UF_UI_selection_p_t接口，可以在NX中选择实体时，根据实体类型进行过滤。
>
> 函数参数包括：
>
> 函数首先通过UF_MODL_ask_body_type函数获取object的实体类型，然后与data指针指向的整数进行比较。如果匹配，则返回UF_UI_SEL_ACCEPT表示接受该对象；否则返回UF_UI_SEL_REJECT表示拒绝该对象。
>
> 该函数可用于在NX选择操作中过滤出特定类型的实体，例如只选择面体或线体。通过自定义data中的类型值，可以灵活地设置过滤条件。
>
> 总的来说，这段代码实现了NX中选择实体的类型过滤功能，为二次开发提供了便利。
>
