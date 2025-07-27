### 【0317】ask object view tag 查询对象视图标签

#### 代码

```cpp
    static tag_t ask_object_view_tag(tag_t object)  
    {  
        int  
            status;  
        tag_t  
            view = NULL_TAG;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(uc6409(object, &status, view_name));  
        if (status == 1)   
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
        return view;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要用于获取NX中给定对象的视图标签。具体步骤如下：
>
> 1. 定义了一个名为ask_object_view_tag的静态函数，输入参数为object，表示要查询的对象的标签，返回值为视图标签。
> 2. 在函数内部，首先声明了status变量，用于存储查询状态；view变量用于存储视图标签；view_name数组用于存储视图名称。
> 3. 调用uc6409函数，查询object对应的视图名称，并存储在view_name数组中。status变量存储查询状态。
> 4. 判断查询状态status，如果等于1，表示查询成功，视图名称存在。
> 5. 如果视图名称存在，则调用UF_VIEW_ask_tag_of_view_name函数，根据视图名称查询对应的视图标签，并存储在view变量中。
> 6. 函数返回视图标签view。
>
> 综上所述，这段代码的主要作用是根据NX对象获取其对应的视图标签，用于后续的视图操作等。
>
