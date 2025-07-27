### 【1770】process warnings 处理警告

#### 代码

```cpp
    static void process_warnings(int n_warnings, int **warnings)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (n_warnings < 1) return;  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window("Warnings:\n");  
        for (ii = 0; ii < n_warnings; ii++)  
        {  
            UF_UI_write_listing_window("  ");  
            UF_get_fail_message((*warnings)[ii], msg);  
            UF_UI_write_listing_window(msg);  
            UF_UI_write_listing_window("\n");  
        }  
        UF_UI_write_listing_window("\n");  
        UF_free(*warnings);  
    }

```

#### 代码解析

> 这是NX二次开发代码中的一段，用于处理NX中产生的警告信息。主要功能包括：
>
> 1. 打开NX的列表窗口，用于显示警告信息。
> 2. 检查警告数量，如果少于1，则直接返回，不处理。
> 3. 遍历所有警告信息，逐条获取每条警告的文本信息，并写入NX的列表窗口。
> 4. 每条警告信息前面添加空格，以便在列表窗口中格式化显示。
> 5. 使用UF_get_fail_message()函数从警告码中获取具体的警告文本。
> 6. 将每条警告信息写入NX的列表窗口，并在每条信息后添加换行符。
> 7. 处理完所有警告后，关闭NX的列表窗口。
> 8. 释放警告信息数组占用的内存空间。
>
> 该代码的主要目的是在NX中获取并显示产生的警告信息，以便开发者和用户可以了解代码运行中可能存在的问题。
>
