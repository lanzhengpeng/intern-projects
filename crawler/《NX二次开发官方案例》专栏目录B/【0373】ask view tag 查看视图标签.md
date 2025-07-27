### 【0373】ask view tag 查看视图标签

#### 代码

```cpp
    /* See UF_VIEW_ask_tag_of_view_name - this function is obsolete. 译:查看UF_VIEW_ask_tag_of_view_name - 这个函数已经过时了。 */  
    static tag_t ask_view_tag(char *view_name)  
    {  
        tag_t  
            view = NULL_TAG;  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_by_name(view_name, &view)) && view)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(view, &type, &subtype));  
            if (type == UF_view_type) return view;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其功能是查询NX中的视图标签(tag)。
>
> 具体步骤如下：
>
> 1. 定义视图名称为view_name的字符数组。
> 2. 初始化视图标签view为NULL_TAG。
> 3. 循环调用UF_OBJ_cycle_by_name函数，根据视图名称view_name查询视图标签，直到查询完毕。在循环体内，使用UF_OBJ_ask_type_and_subtype函数查询视图标签的类型和子类型。
> 4. 如果类型等于UF_view_type，表示这是一个视图，则返回视图标签view。
> 5. 循环结束后，如果未能找到匹配的视图，则返回NULL_TAG。
> 6. 该函数的返回值是视图标签view。
>
> 需要注意的是，代码注释指出这个函数已经过时，建议使用UF_VIEW_ask_tag_of_view_name函数来查询视图标签。
>
