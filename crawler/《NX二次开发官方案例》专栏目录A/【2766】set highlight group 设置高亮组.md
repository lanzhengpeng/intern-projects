### 【2766】set highlight group 设置高亮组

#### 代码

```cpp
    static void set_highlight_group(tag_t group, int flag)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *objects;  
        UF_CALL(UF_GROUP_ask_group_data(group, &objects, &n));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        if (n > 0) UF_free(objects);  
    }

```

#### 代码解析

> 这段代码是用于NX Open API的二次开发代码，其主要功能是设置指定组的对象的高亮显示。
>
> 具体步骤包括：
>
> 1. 首先通过UF_GROUP_ask_group_data函数获取指定组的对象数组及其数量。
> 2. 然后通过一个循环遍历数组，对每个对象调用UF_DISP_set_highlight函数，根据flag参数设置其高亮显示状态。
> 3. 最后释放获取的对象数组内存。
>
> 通过该函数，我们可以方便地设置NX中某个组的对象的高亮显示状态，从而在图形界面中突出显示这些对象。
>
