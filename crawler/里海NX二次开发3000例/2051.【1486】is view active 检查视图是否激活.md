### 【1486】is view active 检查视图是否激活

#### 代码

```cpp
    static logical is_view_active(char *view_name)  
    {  
        char  
            this_view[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (!uc6473("", this_view) && strcmp(this_view, ""))  
        {  
            if (!strcmp(this_view, view_name)) return TRUE;  
        }  
        while (!uc6499("", this_view) && strcmp(this_view, ""))  
        {  
            if (!strcmp(this_view, view_name)) return TRUE;  
        }  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中检查特定视图是否处于激活状态的二次开发函数。
>
> 具体来说，函数is_view_active接受一个视图名称view_name作为输入参数，并在NX内部遍历所有激活的视图，比较视图名称是否与输入的view_name相等。
>
> 函数中使用了两个内部函数uc6473和uc6499，它们分别用于遍历不同类型的视图。在遍历过程中，如果找到名称匹配的视图，则函数返回TRUE，表示该视图处于激活状态。如果遍历结束仍未找到匹配的视图，则函数返回FALSE。
>
> 需要注意的是，函数中使用了两个循环结构来遍历所有类型的视图，并使用了strcmp函数来比较视图名称。此外，在循环条件中使用了"!"逻辑非运算符来检查视图名称不为空。
>
> 总的来说，这段代码实现了在NX中检查特定视图是否激活的功能，可以用于二次开发中的视图管理或交互。
>
