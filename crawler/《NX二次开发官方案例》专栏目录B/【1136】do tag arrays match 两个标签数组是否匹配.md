### 【1136】do tag arrays match 两个标签数组是否匹配

#### 代码

```cpp
    static int ask_item_array_index(int n_items, tag_t *items, tag_t item)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n_items; ii++)  
            if (items[ii] == item) return ii;  
        return -1;  
    }  
    static logical do_tag_arrays_match(int n_one, tag_t *one, int n_two, tag_t *two)  
    {  
        int  
            ii;  
        if (n_one != n_two) return FALSE;  
        for (ii = 0; ii < n_one; ii++)  
        {  
            if ((ask_item_array_index(n_two, two, one[ii])) > -1) continue;  
            return FALSE;  
        }  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码主要用于比较两个标签数组是否匹配。
>
> 首先，ask_item_array_index函数用于在标签数组中查找指定标签的位置。其参数包括数组大小、数组本身以及要查找的标签。函数通过遍历数组，使用标签的等号比较操作符找到匹配的标签并返回其在数组中的索引。如果未找到匹配的标签，则返回-1。
>
> 接下来，do_tag_arrays_match函数用于比较两个标签数组是否匹配。其参数包括两个标签数组的大小、两个标签数组本身。函数首先比较两个数组的大小，如果不相等则直接返回FALSE。接着，函数遍历第一个标签数组，对于每个标签，使用ask_item_array_index函数在第二个标签数组中查找是否存在匹配的标签。如果所有标签都能在第二个数组中找到匹配，则函数返回TRUE，表示两个数组匹配。如果在查找过程中发现第一个数组中的标签在第二个数组中不存在，则函数立即返回FALSE。
>
> 总体来说，这段代码提供了一种有效的方法来比较两个标签数组是否完全匹配，通过查找每个标签在另一个数组中的位置来实现。如果所有标签都能匹配，则认为两个数组匹配，否则不匹配。
>
