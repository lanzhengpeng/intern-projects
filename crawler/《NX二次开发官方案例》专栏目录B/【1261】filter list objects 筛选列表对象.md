### 【1261】filter list objects 筛选列表对象

#### 代码

```cpp
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    /* 里海 */  
    static int filter_list_objects(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        uf_list_p_t  
            the_list = (uf_list_p_t)data;  
        if (is_item_on_list(the_list, object))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的UF_UI模块的一部分，用于在NX中选择对象。
>
> 主要功能包括：
>
> 1. is_item_on_list函数用于判断一个对象是否在指定的列表中。它遍历列表，检查对象的ID，返回布尔值。
> 2. filter_list_objects函数用于过滤选择集，仅返回在指定列表中的对象。它接收选择集的回调，并使用is_item_on_list函数来过滤不符合条件的对象。
> 3. 这段代码利用UF_UI模块提供的接口，通过自定义过滤函数，实现了根据特定条件选择对象的功能。
> 4. 代码通过遍历列表和回调机制，实现了高效的选择集过滤，具有良好的扩展性。
> 5. 使用了静态函数，避免全局作用域污染，体现了良好的编程习惯。
> 6. 注释采用中文，方便理解，但需要改进注释的准确性。
>
> 总体来说，这段代码简洁高效，实现了NX二次开发中基于特定条件过滤选择集的功能。代码结构清晰，注释详尽，具有良好的可读性和可维护性。
>
